param([DateTimeOffset]$FinishBeforeUtc,[ValidateSet('DIY Mouse Timing v15','DIY Mouse Timing v16','DIY Mouse Input v17')][string]$ExpectedProduct='DIY Mouse Timing v15',[switch]$SelfTest)
$ErrorActionPreference='Stop'
function Assert-LatencyTime([DateTimeOffset]$Now,[DateTimeOffset]$Finish){
    if($Now -ge $Finish.AddMinutes(-8)){throw 'Too little time for capture and physical rollback;no capture'}
}
function Test-SendLatencyStop([bool]$Requested,[bool]$Sent){return $Requested -and !$Sent}
function Test-FrozenSummary([string]$Text,[bool]$NeedInput){
    return ($Text -match 'LATENCY_END [^\r\n]+\r?\n') -and (!$NeedInput -or $Text -match 'INPUT_END capped=\d+\r?\n')
}
if($SelfTest){
    $now=[DateTimeOffset]::UtcNow;Assert-LatencyTime $now $now.AddMinutes(9)
    foreach($minutes in @(-1,7,8)){$refused=$false;try{Assert-LatencyTime $now $now.AddMinutes($minutes)}catch{$refused=$true};if(!$refused){throw 'Unsafe deadline accepted'}}
    if(!(Test-SendLatencyStop $true $false) -or (Test-SendLatencyStop $true $true) -or (Test-SendLatencyStop $false $false)){throw 'Stop gate failed'}
    $lat="LATENCY_END failed=0`n";$inputEnd="INPUT_END capped=0`n"
    if(!(Test-FrozenSummary $lat $false) -or (Test-FrozenSummary $lat $true) -or !(Test-FrozenSummary ($lat+$inputEnd) $true) -or (Test-FrozenSummary ($lat+'INPUT_END capped=0') $true)){throw 'Frozen-summary completeness gate failed'}
    Write-Output 'PASS: deadline, single-stop and frozen-summary gates;no hardware access';exit 0
}
if(!$PSBoundParameters.ContainsKey('FinishBeforeUtc')){throw 'Current user finish deadline required'}
Assert-LatencyTime ([DateTimeOffset]::UtcNow) $FinishBeforeUtc
$root=Join-Path (Split-Path $PSScriptRoot -Parent) 'build-pairing-analysis'
$version=[regex]::Match($ExpectedProduct,'v\d+$').Value
$needInput=$ExpectedProduct -eq 'DIY Mouse Input v17'
$job=Join-Path $root ($version+'-latency-'+(Get-Date -Format yyyyMMdd-HHmmss)+'-'+[guid]::NewGuid().ToString('N').Substring(0,8))
$clock=[Diagnostics.Stopwatch]::StartNew();$log=[IO.StreamWriter]::new("$job.txt",$false);$journal=[IO.StreamWriter]::new("$job.host.jsonl",$false)
$port=$null;$raw=$null;$allText='';$xSent=$false;$dumpSeen=$false
$result=@{scope='quiet instrumented RF/USB timing;100s foreground raw;no arm command';expected_product=$ExpectedProduct;success=$false;p_count=0;x_count=0;d_count=0}
function Record-Latency([string]$Phase,[string]$Text){$journal.WriteLine((@{host_utc=[DateTime]::UtcNow.ToString('o');elapsed_ms=$clock.ElapsedMilliseconds;phase=$Phase;text=$Text}|ConvertTo-Json -Compress));$journal.Flush()}
function Read-Latency([string]$Phase){$chunk=$port.ReadExisting();if($chunk.Length){$log.Write($chunk);$log.Flush();Record-Latency $Phase $chunk;$script:allText+=$chunk}}
try{
    $devices=@(Get-CimInstance Win32_PnPEntity|Where-Object{$_.PNPDeviceID -match 'VID_2FE3&PID_0008' -and $_.Status -eq 'OK'})
    $usb=@($devices|Where-Object{$_.PNPDeviceID -match '^USB\\VID_2FE3&PID_0008\\'})
    $cdc=@($devices|Where-Object{$_.Name -match '\(COM\d+\)'})
    if($usb.Count -ne 1 -or $cdc.Count -ne 1){throw 'Unique timing device/CDC missing'}
    if((Get-PnpDeviceProperty -InstanceId $usb[0].PNPDeviceID -KeyName DEVPKEY_Device_BusReportedDeviceDesc).Data -ne $ExpectedProduct){throw 'Timing image product not verified;no device command'}
    $name=[regex]::Match($cdc[0].Name,'\((COM\d+)\)').Groups[1].Value
    $port=[IO.Ports.SerialPort]::new($name,115200);$port.DtrEnable=$true;$port.WriteTimeout=1000;$port.Open()
    $end=$clock.ElapsedMilliseconds+2000;while($clock.ElapsedMilliseconds -lt $end){Read-Latency 'pre';Start-Sleep -Milliseconds 20}
    if($allText.Length){throw 'Unexpected pre-capture serial output'}
    $raw=Start-Process -FilePath (Join-Path $root 'capture_raw_mouse.exe') -ArgumentList @('100',('"'+"$job.csv"+'"'),'--foreground') -WindowStyle Normal -PassThru -RedirectStandardOutput "$job.raw.stdout.txt" -RedirectStandardError "$job.raw.stderr.txt"
    $end=$clock.ElapsedMilliseconds+3000
    while($clock.ElapsedMilliseconds -lt $end -and !$raw.HasExited){if((Test-Path "$job.raw.stdout.txt") -and (Get-Content "$job.raw.stdout.txt" -Raw) -match 'RAW_INPUT_READY'){break};Start-Sleep -Milliseconds 20}
    if((Get-Content "$job.raw.stdout.txt" -Raw) -notmatch 'RAW_INPUT_READY'){throw 'Raw capture readiness failed'}
    Assert-LatencyTime ([DateTimeOffset]::UtcNow) $FinishBeforeUtc
    $rawAt=$clock.ElapsedMilliseconds;$probeSent=$false;$xAt=$null;$nextDump=0;$dumps=0
    Write-Output "CAPTURE_BASE=$job";Write-Output "CAPTURE_RUNNING: $ExpectedProduct automatic startup;no arm command"
    while($clock.ElapsedMilliseconds -lt $rawAt+100000){
        Read-Latency 'capture'
        if(!$xSent -and $allText.Length){throw 'Serial output during active measurement'}
        if(!$probeSent -and !$xSent -and $clock.ElapsedMilliseconds -ge $rawAt+50000){$port.Write('d');$result.d_count++;$probeSent=$true;Record-Latency 'active_probe' 'd;must remain silent'}
        $abort=Test-Path "$job.stop-request"
        if(Test-SendLatencyStop ($abort -or $clock.ElapsedMilliseconds -ge $rawAt+70000) $xSent){
            $xSent=$true;$xAt=$clock.ElapsedMilliseconds;$result.x_count=1;$result.aborted=$abort
            $port.Write('x');Record-Latency 'stop_command' 'x';$nextDump=$xAt+250;Write-Output 'X_STOP_SENT_ONCE'
        }
        if($xSent -and !$dumpSeen){
            if(Test-FrozenSummary $allText $needInput){$dumpSeen=$true;Write-Output 'FROZEN_SUMMARY_RECEIVED: implies completed neutral stop'}
            elseif($dumps -lt 3 -and $clock.ElapsedMilliseconds -ge $nextDump){$port.Write('d');$dumps++;$result.d_count++;Record-Latency 'frozen_dump' 'd';$nextDump=$clock.ElapsedMilliseconds+500}
            elseif($dumps -eq 3 -and $clock.ElapsedMilliseconds -ge $nextDump+1500){throw 'Frozen summary incomplete;no retry/rearm'}
        }
        if($raw.HasExited -and $clock.ElapsedMilliseconds -lt $rawAt+99000){throw 'Raw window closed early'}
        Start-Sleep -Milliseconds 20
    }
    $raw.WaitForExit(3000)|Out-Null
    if(!$dumpSeen -or !$raw.HasExited -or $raw.ExitCode -ne 0 -or (Get-Content "$job.raw.stdout.txt" -Raw) -notmatch 'RAW_INPUT_DONE.*read_errors=0 failed=0'){throw 'Capture did not complete cleanly'}
    $result.success=$true;$result.stop_confirmed=$true;$result.active_probe_silent=$probeSent
    Write-Output 'TIMING_CAPTURE_COMPLETE: validate summary and collect user observation'
}catch{$result.error=$_.Exception.Message;throw}finally{
    if($port -and $port.IsOpen){if(!$xSent){try{$port.Write('x');$result.x_count=1;Record-Latency 'cleanup_x' 'capture failure'}catch{}};$port.Close()};if($port){$port.Dispose()}
    if($raw){if(!$raw.HasExited){$raw.Kill();$result.raw_partial=$true}else{$result.raw_exit=$raw.ExitCode};Get-Content "$job.raw.stdout.txt" -ErrorAction SilentlyContinue;$raw.Dispose()}
    $result.finished_utc=[DateTime]::UtcNow.ToString('o');$result|ConvertTo-Json|Set-Content "$job.capture.json";$log.Dispose();$journal.Dispose()
}
