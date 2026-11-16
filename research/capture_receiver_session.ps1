param([DateTimeOffset]$FinishBeforeUtc,[switch]$ForegroundMotion,[switch]$SelfTest)
$ErrorActionPreference='Stop'
function Assert-SessionTime([DateTimeOffset]$Now,[DateTimeOffset]$Finish){
    if($Now -ge $Finish.AddMinutes(-18)){throw 'Insufficient time for full935s capture and wrap-up;no arm'}
}
function Test-SendStop([bool]$Requested,[bool]$AlreadySent){return $Requested -and !$AlreadySent}
function Test-MotionStop([bool]$Enabled,$LinkAt,[long]$Now){return $Enabled -and $null -ne $LinkAt -and $Now -ge $LinkAt+60000}
if($SelfTest){
    $now=[DateTimeOffset]::UtcNow;Assert-SessionTime $now $now.AddMinutes(30)
    foreach($minutes in @(-1,17,18)){$rejected=$false;try{Assert-SessionTime $now $now.AddMinutes($minutes)}catch{$rejected=$true};if(!$rejected){throw 'Unsafe deadline accepted'}}
    if(!(Test-SendStop $true $false) -or (Test-SendStop $true $true) -or (Test-SendStop $false $false)){throw 'One-stop gate failed'}
    if((Test-MotionStop $true $null 90000) -or (Test-MotionStop $false 100 60100) -or (Test-MotionStop $true 100 60099) -or !(Test-MotionStop $true 100 60100)){throw 'Motion stop deadline failed'}
    Write-Output 'PASS: full-session deadline, motion-stop deadline and single-stop gates;no hardware access';exit 0
}
if(!$PSBoundParameters.ContainsKey('FinishBeforeUtc')){throw 'Explicit user finish deadline required'}
Assert-SessionTime ([DateTimeOffset]::UtcNow) $FinishBeforeUtc
$privateRoot=Join-Path (Split-Path $PSScriptRoot -Parent) 'build-pairing-analysis'
$mode=if($ForegroundMotion){'motion'}else{'session'}
$seconds=if($ForegroundMotion){100}else{935}
$job=Join-Path $privateRoot ('v13-'+$mode+'-'+(Get-Date -Format 'yyyyMMdd-HHmmss')+'-'+[guid]::NewGuid().ToString('N').Substring(0,8))
$clock=[Diagnostics.Stopwatch]::StartNew();$log=[IO.StreamWriter]::new("$job.txt",$false);$journal=[IO.StreamWriter]::new("$job.host.jsonl",$false)
$port=$null;$raw=$null;$allText='';$pSent=$false;$xSent=$false;$cleanStop=$false;$aborted=$false
$scope=if($ForegroundMotion){'foreground motion timing; x after60s link; user observations pending'}else{'finite15min normal-use session;user observations pending'}
$result=@{scope=$scope;success=$false;p_count=0;x_count=0;serial_seconds=$seconds;raw_seconds=$seconds}
function Record([string]$Phase,[string]$Text){$journal.WriteLine((@{host_utc=[DateTime]::UtcNow.ToString('o');elapsed_ms=$clock.ElapsedMilliseconds;phase=$Phase;text=$Text}|ConvertTo-Json -Compress));$journal.Flush()}
function Read-Part([string]$Phase){$chunk=$port.ReadExisting();if($chunk.Length){$log.Write($chunk);$log.Flush();Record $Phase $chunk;$script:allText+=$chunk}}
try{
    $cdc=@(Get-CimInstance Win32_PnPEntity|Where-Object{$_.Name -match '\(COM\d+\)' -and $_.PNPDeviceID -match 'VID_2FE3&PID_0008' -and $_.Status -eq 'OK'})
    if($cdc.Count -ne 1){throw 'Unique receiver CDC not verified;no arm'}
    $portName=[regex]::Match($cdc[0].Name,'\((COM\d+)\)').Groups[1].Value
    $port=[IO.Ports.SerialPort]::new($portName,115200);$port.DtrEnable=$true;$port.WriteTimeout=1000;$port.Open()
    $drainEnd=$clock.ElapsedMilliseconds+2000
    while($clock.ElapsedMilliseconds -lt $drainEnd){Read-Part 'prearm';Start-Sleep -Milliseconds 20}
    if(!$allText.Contains('DIY Mouse Wireless HID Probe v13: bounded fifteen minutes;') -or $allText -match 'ERROR|RECEIVER armed:|RECEIVER link started:|RECEIVER disarmed:'){throw 'Fresh passive v13 boot not verified;no arm'}
    Assert-SessionTime ([DateTimeOffset]::UtcNow) $FinishBeforeUtc
    $rawArgs=@([string]$seconds,('"'+"$job.csv"+'"'));$windowStyle='Hidden'
    if($ForegroundMotion){$rawArgs+='--foreground';$windowStyle='Normal'}
    $raw=Start-Process -FilePath (Join-Path $privateRoot 'capture_raw_mouse.exe') -ArgumentList $rawArgs -WindowStyle $windowStyle -PassThru -RedirectStandardOutput "$job.raw.stdout.txt" -RedirectStandardError "$job.raw.stderr.txt"
    $readyEnd=$clock.ElapsedMilliseconds+3000
    while($clock.ElapsedMilliseconds -lt $readyEnd -and !$raw.HasExited){
        Read-Part 'prearm'
        if((Test-Path "$job.raw.stdout.txt") -and (Get-Content "$job.raw.stdout.txt" -Raw) -match 'RAW_INPUT_READY'){break}
        Start-Sleep -Milliseconds 20
    }
    if((Get-Content "$job.raw.stdout.txt" -Raw) -notmatch 'RAW_INPUT_READY' -or $allText -match 'ERROR|RECEIVER armed:|RECEIVER link started:'){throw 'Raw/prearm readiness failed;no arm'}
    Assert-SessionTime ([DateTimeOffset]::UtcNow) $FinishBeforeUtc
    $allText='';$pSent=$true;$result.p_count=1;$port.Write('p');Record 'arm_command' 'p';$armAt=$clock.ElapsedMilliseconds
    $armSeen=$false;$nextStopCheck=0;$nextProgress=60000;$xAt=$null;$linkAt=$null
    Write-Output "CAPTURE_BASE=$job"
    while($clock.ElapsedMilliseconds -lt $armAt+$seconds*1000){
        Read-Part 'session'
        if(!$armSeen -and $allText.Contains('RECEIVER armed: first discovery within 30000 ms; up to 8 discoveries and 983040 ACKs total in 900000 ms; gap=1000 ms')){$armSeen=$true;Write-Output 'ARMING_CONFIRMED: finite15min v13 session;full host capture active'}
        if(!$armSeen -and $clock.ElapsedMilliseconds -gt $armAt+3000){throw 'Arming confirmation absent;no retry'}
        if($allText.Contains('ERROR')){throw 'Firmware error logged;stop trial'}
        if($null -eq $linkAt -and $allText.Contains('RECEIVER link started:')){$linkAt=$clock.ElapsedMilliseconds}
        if(Test-SendStop (Test-MotionStop $ForegroundMotion $linkAt $clock.ElapsedMilliseconds) $xSent){
            $xSent=$true;$result.x_count=1;$xAt=$clock.ElapsedMilliseconds
            $port.Write('x');Record 'stop_command' 'x after60s motion';Write-Output 'X_STOP_SENT_ONCE: motion window complete'
        }
        if($clock.ElapsedMilliseconds -ge $nextStopCheck){
            $nextStopCheck=$clock.ElapsedMilliseconds+250
            if(Test-SendStop (Test-Path "$job.stop-request") $xSent){
                $xSent=$true;$aborted=$true;$result.x_count=1;$xAt=$clock.ElapsedMilliseconds
                $port.Write('x');Record 'stop_command' 'x';Write-Output 'X_STOP_SENT_ONCE'
            }
        }
        if($xSent -and !$cleanStop){
            if($allText.Contains('RECEIVER disarmed: neutral complete; reboot required')){
                $cleanStop=$true;Write-Output 'STOP_CONFIRMED: neutral complete'
                if($aborted){Write-Output 'EARLY_ABORT: raw data is partial';break}
            }
            if(!$cleanStop -and $clock.ElapsedMilliseconds -gt $xAt+3000){throw 'x stop not confirmed'}
        }
        if($clock.ElapsedMilliseconds -ge $armAt+$nextProgress){Write-Output "CAPTURE_PROGRESS postarm_seconds=$([int](($clock.ElapsedMilliseconds-$armAt)/1000))";$nextProgress+=60000}
        Start-Sleep -Milliseconds 20
    }
    if(!$aborted){
        if($ForegroundMotion -and (!$xSent -or !$cleanStop)){throw 'Motion x-stop not confirmed'}
        $lastStart=$allText.LastIndexOf('RECEIVER link started:');$lastStop=$allText.LastIndexOf('RECEIVER link stopped:')
        if($lastStart -lt 0 -or $lastStop -le $lastStart -or !$allText.Substring($lastStop).Contains('neutral complete')){throw 'Final link neutral stop not observed'}
        $cleanStop=$true
        $raw.WaitForExit(3000)|Out-Null
        if(!$raw.HasExited -or $raw.ExitCode -ne 0 -or (Get-Content "$job.raw.stdout.txt" -Raw) -notmatch 'RAW_INPUT_DONE.*read_errors=0 failed=0'){throw 'Full raw capture not completed cleanly'}
        $result.success=$true;Write-Output 'FULL_SESSION_CAPTURE_COMPLETE: user consistency/deadline observations pending'
    }
}catch{$result.error=$_.Exception.Message;throw}finally{
    if($pSent -and !$cleanStop -and !$xSent -and $port -and $port.IsOpen){try{$xSent=$true;$result.x_count=1;$port.Write('x');Record 'cleanup_x' 'capture failure';$end=$clock.ElapsedMilliseconds+1500;while($clock.ElapsedMilliseconds -lt $end){Read-Part 'cleanup';Start-Sleep -Milliseconds 20}}catch{Record 'cleanup_error' $_.Exception.Message}}
    if($port){if($port.IsOpen){$port.Close()};$port.Dispose()}
    if($raw){
        if(!$raw.HasExited){$raw.Kill();$result.raw_partial=$true}else{$result.raw_exit=$raw.ExitCode}
        Get-Content "$job.raw.stdout.txt" -ErrorAction SilentlyContinue;$raw.Dispose()
    }
    $result.aborted=$aborted;$result.stop_confirmed=$cleanStop;$result.finished_utc=[DateTime]::UtcNow.ToString('o')
    $result|ConvertTo-Json|Set-Content "$job.capture.json";$log.Dispose();$journal.Dispose()
}
