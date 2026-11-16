param(
    [ValidateSet('Prepare','Capture','SelfTest')][string]$Mode,
    [ValidatePattern('^[a-z0-9-]{1,80}$')][string]$RunName
)
$ErrorActionPreference='Stop'
$privateRoot=Join-Path (Split-Path $PSScriptRoot -Parent) 'build-pairing-analysis'
function Assert-ReadyJob($Ready,$State,[string]$Name,[DateTime]$Now) {
    if ($Ready.run_name -ne $Name -or $State.run_name -ne $Name -or $State.armed -isnot [bool] -or $State.armed -ne $false -or ([DateTimeOffset]$Ready.expires_utc).UtcDateTime -le $Now.AddSeconds(75)) {
        throw 'Unused matching job with at least75s helper lifetime required;no arm'
    }
}
if ($Mode -eq 'SelfTest') {
    $now=[DateTime]::UtcNow
    $ready=[pscustomobject]@{run_name='test';expires_utc=$now.AddSeconds(180).ToString('o')}
    $state=[pscustomobject]@{run_name='test';armed=$false}
    Assert-ReadyJob $ready $state 'test' $now
    foreach ($case in @('used','wrong-job','expired','too-late')) {
        $state.armed=$case -eq 'used';$ready.run_name=if($case -eq 'wrong-job'){'other'}else{'test'}
        $ready.expires_utc=$now.AddSeconds($(if($case -eq 'expired'){-1}elseif($case -eq 'too-late'){75}else{180})).ToString('o')
        $rejected=$false;try { Assert-ReadyJob $ready $state 'test' $now } catch { $rejected=$true }
        if (!$rejected) { throw "Unsafe job accepted:$case" }
    }
    $fixtureRun='activeusb-selftest-'+[guid]::NewGuid().ToString('N')
    $fixture=Join-Path $privateRoot $fixtureRun
    try {
        @{run_name=$fixtureRun;armed=$false}|ConvertTo-Json|Set-Content "$fixture.state.json"
        @{run_name=$fixtureRun;pid=-1;expires_utc=[DateTime]::UtcNow.AddSeconds(180).ToString('o')}|ConvertTo-Json|Set-Content "$fixture.ready.json"
        [IO.File]::WriteAllText("$fixture.txt",'preserve')
        $rejected=$false;try { & $PSCommandPath -Mode Capture -RunName $fixtureRun } catch { $rejected=$_.Exception.Message -eq 'Capture files already exist;use a new job' }
        if (!$rejected -or [IO.File]::ReadAllText("$fixture.txt") -ne 'preserve') {throw 'Existing capture protection failed'}
    } finally {
        Remove-Item -LiteralPath "$fixture.state.json","$fixture.ready.json","$fixture.txt","$fixture.cancel" -ErrorAction SilentlyContinue
    }
    Write-Output 'PASS: unused/matching job,lifetime and existing-file protection;no hardware access'
    exit 0
}
if ($Mode -eq 'Prepare') {
    if ($RunName) { throw 'Prepare generates a unique run name' }
    $RunName='v12-activeusb-' + (Get-Date -Format 'yyyyMMdd-HHmmss') + '-' + [guid]::NewGuid().ToString('N').Substring(0,8)
    $job=Join-Path $privateRoot $RunName
    if (Test-Path "$job.state.json") { throw 'Job already exists' }
    @{run_name=$RunName;armed=$false} | ConvertTo-Json | Set-Content "$job.state.json"
    Write-Output "RUN_NAME=$RunName"
    $helper=Join-Path $PSScriptRoot 'restart_receiver_usb_deferred.ps1'
    $exe=(Get-Process -Id $PID).Path
    try {
        $process=Start-Process -FilePath $exe -ArgumentList @('-NoProfile','-ExecutionPolicy','Bypass','-File',('"'+$helper+'"'),'-RunName',$RunName) -Verb RunAs -WindowStyle Hidden -PassThru
        $wait=[Diagnostics.Stopwatch]::StartNew()
        while (!(Test-Path "$job.ready.json")) {
            if ($process.HasExited -or $wait.ElapsedMilliseconds -ge 15000) { throw 'Deferred helper did not become ready' }
            Start-Sleep -Milliseconds 100
        }
        $ready=Get-Content "$job.ready.json" -Raw | ConvertFrom-Json
        Write-Output "HELPER_READY expires_utc=$($ready.expires_utc);no restart yet"
    } catch {
        [IO.File]::WriteAllText("$job.cancel",'cancel')
        throw
    }
    exit 0
}
if ($Mode -ne 'Capture' -or !$RunName) { throw 'Use Prepare,Capture -RunName,or SelfTest' }
$job=Join-Path $privateRoot $RunName
$ready=Get-Content "$job.ready.json" -Raw | ConvertFrom-Json
$state=Get-Content "$job.state.json" -Raw | ConvertFrom-Json
try {
    Assert-ReadyJob $ready $state $RunName ([DateTime]::UtcNow)
    if ((Test-Path "$job.result.json") -or (Test-Path "$job.cancel")) { throw 'Helper already finished/cancelled;no arm' }
    if ((Test-Path "$job.txt") -or (Test-Path "$job.csv") -or (Test-Path "$job.capture.json")) { throw 'Capture files already exist;use a new job' }
    Get-Process -Id $ready.pid -ErrorAction Stop | Out-Null
} catch {
    [IO.File]::WriteAllText("$job.cancel",'cancel')
    throw
}
if (-not ('ReceiverMouseButtonProbe' -as [type])) {
    Add-Type -TypeDefinition @'
using System.Runtime.InteropServices;
public static class ReceiverMouseButtonProbe {
 [DllImport("user32.dll")] public static extern short GetAsyncKeyState(int key);
}
'@
}
$clock=[Diagnostics.Stopwatch]::StartNew()
$log=[IO.StreamWriter]::new("$job.txt",$false)
$journal=[IO.StreamWriter]::new("$job.host.jsonl",$false)
$port=$null;$raw=$null;$allText='';$pSent=$false;$recovered=$false
$result=@{run_name=$RunName;success=$false;p_count=0;scope='active receiver-only USB restart;physical observations pending'}
function Record([string]$Phase,[string]$Text) {
    $journal.WriteLine((@{host_utc=[DateTime]::UtcNow.ToString('o');elapsed_ms=$clock.ElapsedMilliseconds;phase=$Phase;text=$Text}|ConvertTo-Json -Compress));$journal.Flush()
}
function Read-Chunk([string]$Phase) {
    $chunk=$port.ReadExisting()
    if ($chunk.Length) { $log.Write($chunk);$log.Flush();Record $Phase $chunk;$script:allText+=$chunk }
}
function Open-ReceiverPort {
    $wait=[Diagnostics.Stopwatch]::StartNew()
    do {
        $cdc=@(Get-CimInstance Win32_PnPEntity | Where-Object { $_.Name -match '\(COM\d+\)' -and $_.PNPDeviceID -match 'VID_2FE3&PID_0008' -and $_.Status -eq 'OK' })
        if ($cdc.Count -eq 1) {
            $name=[regex]::Match($cdc[0].Name,'\((COM\d+)\)').Groups[1].Value
            $serial=[IO.Ports.SerialPort]::new($name,115200);$serial.DtrEnable=$true
            $serial.Open();return $serial
        }
        if ($cdc.Count -gt 1) { throw 'Ambiguous receiver CDC;no serial access' }
        Start-Sleep -Milliseconds 250
    } while ($wait.ElapsedMilliseconds -lt 12000)
    throw 'Receiver CDC did not re-enumerate'
}
try {
    $port=Open-ReceiverPort
    $drainEnd=$clock.ElapsedMilliseconds+2000
    while ($clock.ElapsedMilliseconds -lt $drainEnd) { Read-Chunk 'prearm';Start-Sleep -Milliseconds 20 }
    if (!$allText.Contains('DIY Mouse Wireless HID Probe v12: startup neutral; passive USB restart;') -or !$allText.Contains('HID startup neutral complete; no RF arming') -or $allText -match 'ERROR|RECEIVER armed:|RECEIVER link started:|RECEIVER disarmed:') { throw 'Fresh passive v12 boot not verified;no arm' }
    $raw=Start-Process -FilePath (Join-Path $privateRoot 'capture_raw_mouse.exe') -ArgumentList @('150',('"'+"$job.csv"+'"')) -WindowStyle Hidden -PassThru -RedirectStandardOutput "$job.raw.stdout.txt" -RedirectStandardError "$job.raw.stderr.txt"
    $rawReadyEnd=$clock.ElapsedMilliseconds+3000
    while ($clock.ElapsedMilliseconds -lt $rawReadyEnd -and !$raw.HasExited) {
        if ((Test-Path "$job.raw.stdout.txt") -and (Get-Content "$job.raw.stdout.txt" -Raw) -match 'RAW_INPUT_READY') { break }
        Start-Sleep -Milliseconds 20
    }
    if ((Get-Content "$job.raw.stdout.txt" -Raw) -notmatch 'RAW_INPUT_READY') { throw 'Raw logger not ready;no arm' }
    Assert-ReadyJob $ready $state $RunName ([DateTime]::UtcNow)
    if (Test-Path "$job.result.json") { throw 'Helper exited;no arm' }
    $state.armed=$true;$state|ConvertTo-Json|Set-Content "$job.state.json"
    $allText='';$pSent=$true;$result.p_count=1;$port.Write('p')
    Record 'arm_command' 'p'
    $armAt=$clock.ElapsedMilliseconds;$armSeen=$false;$linkAt=$null
    Write-Output "CAPTURE_BASE=$job"
    while ($true) {
        Read-Chunk 'active'
        if (Test-Path "$job.cancel") { throw 'Capture cancelled' }
        if (!$armSeen -and $allText.Contains('RECEIVER armed: first discovery within 30000 ms; up to 8 discoveries and 327680 ACKs total in 300000 ms; gap=1000 ms')) {
            $armSeen=$true;Write-Output 'ARMING_CONFIRMED: fresh v12 response;UAC already completed'
        }
        if (!$armSeen -and $clock.ElapsedMilliseconds -gt $armAt+3000) { throw 'No arming confirmation;no retry' }
        if ($null -eq $linkAt -and $allText.Contains('RECEIVER link started:')) { $linkAt=$clock.ElapsedMilliseconds }
        if ($null -ne $linkAt -and $clock.ElapsedMilliseconds -ge $linkAt+25000) { break }
        if ($null -eq $linkAt -and $clock.ElapsedMilliseconds -gt $armAt+35000) { throw 'No link;cancel reset test' }
        Start-Sleep -Milliseconds 20
    }
    $port.Close();$port.Dispose();$port=$null
    Record 'host_button_state_before_reset' "left_down=$(([ReceiverMouseButtonProbe]::GetAsyncKeyState(1) -band 0x8000) -ne 0)"
    Record 'usb_reset_signal' 'restart'
    [IO.File]::WriteAllText("$job.signal.tmp",'restart');[IO.File]::Move("$job.signal.tmp","$job.signal")
    Write-Output 'USB_RESTART_SIGNALLED: one receiver restart;no further arm command'
    $resetWait=[Diagnostics.Stopwatch]::StartNew()
    while (!(Test-Path "$job.result.json")) {
        if ($resetWait.ElapsedMilliseconds -gt 25000) { throw 'Helper result timed out;no retry' }
        Start-Sleep -Milliseconds 100
    }
    $helperResult=Get-Content "$job.result.json" -Raw | ConvertFrom-Json
    if (!$helperResult.success) { throw 'Receiver restart failed;inspect private result' }
    Record 'usb_reset_result' 'success'
    $allText='';$port=Open-ReceiverPort
    $postEnd=$clock.ElapsedMilliseconds+9000
    while ($clock.ElapsedMilliseconds -lt $postEnd) { Read-Chunk 'post_reset';Start-Sleep -Milliseconds 20 }
    if (!$allText.Contains('DIY Mouse Wireless HID Probe v12: startup neutral; passive USB restart;') -or !$allText.Contains('HID startup neutral complete; no RF arming') -or ([regex]::Matches($allText,'Booting Zephyr OS')).Count -ne 1 -or $allText -match 'ERROR|RECEIVER armed:|RECEIVER link started:') { throw 'Fresh unarmed restart not verified' }
    $recovered=$true;Write-Output 'FRESH_PASSIVE_BOOT_SEEN: no second p sent'
    Record 'host_button_state_after_reset' "left_down=$(([ReceiverMouseButtonProbe]::GetAsyncKeyState(1) -band 0x8000) -ne 0)"
    while (!$raw.HasExited) {
        Read-Chunk 'post_reset';Start-Sleep -Milliseconds 20
        if ($clock.ElapsedMilliseconds -gt 160000) { throw 'Raw logger exceeded deadline' }
    }
    if ($allText -match 'ERROR|RECEIVER armed:|RECEIVER link started:|ACKs=[1-9]' -or ([regex]::Matches($allText,'Booting Zephyr OS')).Count -ne 1) { throw 'Unexpected post-reset activity/boot loop' }
    if ($raw.ExitCode -ne 0) { throw 'Raw logger reported failure;inspect stdout' }
    $result.success=$true;$result.recovered_unarmed=$true
    Write-Output 'ACTIVE_USB_CAPTURE_COMPLETE: fresh passive boot;physical button/no-resume observations pending'
} catch {
    $result.error=$_.Exception.Message
    throw
} finally {
    [IO.File]::WriteAllText("$job.cancel",'cancel')
    if ($pSent -and !$recovered) {
        try {
            if (!$port -or !$port.IsOpen) { $port=Open-ReceiverPort }
            $port.Write('x');Record 'abort_command' 'x'
            $end=$clock.ElapsedMilliseconds+1500
            while ($clock.ElapsedMilliseconds -lt $end) {Read-Chunk 'abort';Start-Sleep -Milliseconds 20}
        } catch { Record 'abort_error' $_.Exception.Message }
    }
    if ($port) {if($port.IsOpen){$port.Close()};$port.Dispose()}
    if ($raw) {
        while (!$raw.HasExited -and $clock.ElapsedMilliseconds -lt 160000) {Start-Sleep -Milliseconds 100}
        if (!$raw.HasExited) {$raw.Kill();$result.raw_timeout=$true}
        else {$result.raw_exit=$raw.ExitCode}
        Get-Content "$job.raw.stdout.txt" -ErrorAction SilentlyContinue
        $raw.Dispose()
    }
    $result.finished_utc=[DateTime]::UtcNow.ToString('o')
    $result|ConvertTo-Json|Set-Content "$job.capture.json"
    $log.Dispose();$journal.Dispose()
}
