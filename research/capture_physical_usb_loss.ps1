param([DateTimeOffset]$ArmBeforeUtc,[switch]$SelfTest)
$ErrorActionPreference='Stop'
function Get-PhysicalAction([bool]$Lost,[bool]$Cancelled,[bool]$Linked,[long]$Elapsed) {
    if ($Lost) {return 'observe'}
    if ($Cancelled -or ($Linked -and $Elapsed -ge 60000) -or (!$Linked -and $Elapsed -ge 35000)) {return 'stop'}
    return 'continue'
}
if($SelfTest){
    if((Get-PhysicalAction $false $false $true 59999) -ne 'continue' -or (Get-PhysicalAction $false $false $true 60000) -ne 'stop' -or (Get-PhysicalAction $false $false $false 35000) -ne 'stop' -or (Get-PhysicalAction $false $true $true 0) -ne 'stop' -or (Get-PhysicalAction $true $true $true 90000) -ne 'observe'){throw 'Physical-loss action gate failed'}
    $blocked=$false;try{& $PSCommandPath -ArmBeforeUtc ([DateTimeOffset]::UtcNow.AddMinutes(-1))}catch{$blocked=$_.Exception.Message -eq 'Future user-session UTC arm cutoff required;no hardware access'}
    if(!$blocked){throw 'Expired cutoff did not refuse before hardware access'}
    Write-Output 'PASS: live/timeout/no-link/cancel/disconnected gates;no hardware access';exit 0
}
if(!$PSBoundParameters.ContainsKey('ArmBeforeUtc') -or [DateTimeOffset]::UtcNow -ge $ArmBeforeUtc){throw 'Future user-session UTC arm cutoff required;no hardware access'}
$privateRoot=Join-Path (Split-Path $PSScriptRoot -Parent) 'build-pairing-analysis'
$job=Join-Path $privateRoot ('v12-physicalusb-'+(Get-Date -Format 'yyyyMMdd-HHmmss')+'-'+[guid]::NewGuid().ToString('N').Substring(0,8))
if(-not('ReceiverMouseButtonProbe' -as [type])){
    Add-Type -TypeDefinition @'
using System.Runtime.InteropServices;
public static class ReceiverMouseButtonProbe {
 [DllImport("user32.dll")] public static extern short GetAsyncKeyState(int key);
}
'@
}
$clock=[Diagnostics.Stopwatch]::StartNew();$log=[IO.StreamWriter]::new("$job.txt",$false);$journal=[IO.StreamWriter]::new("$job.host.jsonl",$false)
$port=$null;$raw=$null;$allText='';$pSent=$false;$lost=$false;$stopSent=$false;$armSeen=$false;$linkAt=$null
$result=@{success=$false;scope='physical receiver unplug;user observation pending';p_count=0;x_count=0}
function Record([string]$Phase,[string]$Text){$journal.WriteLine((@{host_utc=[DateTime]::UtcNow.ToString('o');elapsed_ms=$clock.ElapsedMilliseconds;phase=$Phase;text=$Text}|ConvertTo-Json -Compress));$journal.Flush()}
function Read-Part([string]$Phase){$chunk=$port.ReadExisting();if($chunk.Length){$log.Write($chunk);$log.Flush();Record $Phase $chunk;$script:allText+=$chunk}}
try{
    $cdc=@(Get-CimInstance Win32_PnPEntity|Where-Object{$_.Name -match '\(COM\d+\)' -and $_.PNPDeviceID -match 'VID_2FE3&PID_0008' -and $_.Status -eq 'OK'})
    if($cdc.Count -ne 1){throw 'Unique receiver CDC not verified;no arm'}
    $portName=[regex]::Match($cdc[0].Name,'\((COM\d+)\)').Groups[1].Value
    $port=[IO.Ports.SerialPort]::new($portName,115200);$port.DtrEnable=$true;$port.WriteTimeout=1000;$port.Open()
    $drainEnd=$clock.ElapsedMilliseconds+2000
    while($clock.ElapsedMilliseconds -lt $drainEnd){Read-Part 'prearm';Start-Sleep -Milliseconds 20}
    if(!$allText.Contains('Probe v12: startup neutral;') -or !$allText.Contains('HID startup neutral complete; no RF arming') -or $allText -match 'ERROR|RECEIVER armed:|RECEIVER link started:|RECEIVER disarmed:'){throw 'Fresh passive v12 startup not verified;no arm'}
    if([DateTimeOffset]::UtcNow -ge $ArmBeforeUtc){throw 'Past arm cutoff;no arm'}
    $raw=Start-Process -FilePath (Join-Path $privateRoot 'capture_raw_mouse.exe') -ArgumentList @('120',('"'+"$job.csv"+'"')) -WindowStyle Hidden -PassThru -RedirectStandardOutput "$job.raw.stdout.txt" -RedirectStandardError "$job.raw.stderr.txt"
    $readyEnd=$clock.ElapsedMilliseconds+3000
    while($clock.ElapsedMilliseconds -lt $readyEnd -and !$raw.HasExited){
        Read-Part 'prearm'
        if((Test-Path "$job.raw.stdout.txt") -and (Get-Content "$job.raw.stdout.txt" -Raw) -match 'RAW_INPUT_READY'){break}
        Start-Sleep -Milliseconds 20
    }
    if((Get-Content "$job.raw.stdout.txt" -Raw) -notmatch 'RAW_INPUT_READY' -or $allText -match 'RECEIVER armed:|RECEIVER link started:|ERROR'){throw 'Raw/prearm readiness failed;no arm'}
    if([DateTimeOffset]::UtcNow -ge $ArmBeforeUtc){throw 'Past arm cutoff;no arm'}
    $allText='';$pSent=$true;$result.p_count=1;$port.Write('p');Record 'arm_command' 'p';$armAt=$clock.ElapsedMilliseconds;$nextProbe=0
    Write-Output "CAPTURE_BASE=$job"
    while(!$raw.HasExited){
        if(!$lost){
            try{Read-Part 'active';if([IO.Ports.SerialPort]::GetPortNames() -notcontains $portName){throw 'Receiver port disappeared'}}catch{
                $lost=$true;Record 'serial_loss' $_.Exception.Message
                try{$port.Close()}catch{};$port.Dispose();$port=$null
                Write-Output 'SERIAL_LOSS_OBSERVED: user must confirm physical unplug;no rearm/reopen'
            }
        }
        if(!$armSeen -and $allText.Contains('RECEIVER armed: first discovery within 30000 ms; up to 8 discoveries and 327680 ACKs total in 300000 ms; gap=1000 ms')){$armSeen=$true;Write-Output 'ARMING_CONFIRMED: one v12 arm;physical unplug test,no UAC'}
        if(!$armSeen -and $clock.ElapsedMilliseconds -gt $armAt+3000){throw 'Arm confirmation absent;no retry'}
        if($null -eq $linkAt -and $allText.Contains('RECEIVER link started:')){$linkAt=$clock.ElapsedMilliseconds}
        if($clock.ElapsedMilliseconds -ge $nextProbe){
            Record 'host_button_state' "left_down=$(([ReceiverMouseButtonProbe]::GetAsyncKeyState(1) -band 0x8000) -ne 0);port_lost=$lost"
            $nextProbe=$clock.ElapsedMilliseconds+1000
        }
        $elapsed=$clock.ElapsedMilliseconds - $(if($null -eq $linkAt){$armAt}else{$linkAt})
        $action=Get-PhysicalAction $lost (Test-Path "$job.cancel") ($null -ne $linkAt) $elapsed
        if($action -eq 'stop' -and !$stopSent){$port.Write('x');$stopSent=$true;$result.x_count=1;Record 'fallback_x' 'timeout/cancel;physical test incomplete';Write-Output 'X_FALLBACK_SENT: physical trial incomplete'}
        if($clock.ElapsedMilliseconds -gt 130000){throw 'Capture exceeded bound'}
        Start-Sleep -Milliseconds 20
    }
    $result.success=$lost -and $armSeen -and $null -ne $linkAt -and $raw.ExitCode -eq 0
    Write-Output 'PHYSICAL_USB_CAPTURE_COMPLETE: hardware release result requires user observation'
}catch{$result.error=$_.Exception.Message;throw}finally{
    if($pSent -and !$lost -and !$stopSent -and $port -and $port.IsOpen){try{$port.Write('x');$result.x_count=1;Record 'cleanup_x' 'bounded capture end'}catch{Record 'cleanup_error' $_.Exception.Message}}
    if($port){if($port.IsOpen){$port.Close()};$port.Dispose()}
    if($raw){
        while(!$raw.HasExited -and $clock.ElapsedMilliseconds -lt 130000){Start-Sleep -Milliseconds 100}
        if(!$raw.HasExited){$raw.Kill();$result.raw_timeout=$true}else{$result.raw_exit=$raw.ExitCode}
        Get-Content "$job.raw.stdout.txt" -ErrorAction SilentlyContinue;$raw.Dispose()
    }
    $result.arm_confirmed=$armSeen;$result.link_seen=$null -ne $linkAt;$result.serial_loss=$lost;$result.finished_utc=[DateTime]::UtcNow.ToString('o')
    $result|ConvertTo-Json|Set-Content "$job.capture.json";$log.Dispose();$journal.Dispose()
}
