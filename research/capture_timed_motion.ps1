param([ValidateSet('receiver','wired')][string]$Source='receiver',[switch]$SelfTest)
$ErrorActionPreference='Stop'
function Get-TimedSourcePattern([string]$Type){
    if($Type -eq 'receiver'){return '^USB\\VID_2FE3&PID_0008\\'}
    if($Type -eq 'wired'){return '^USB\\VID_258A&PID_2018\\'}
    throw 'Unsupported source'
}
function Test-TimedMotionComplete([int]$Code,[string]$Text){
    return $Code -eq 0 -and $Text -match 'MOTION_DONE time_us=\d+\r?\n' -and $Text -match 'RAW_INPUT_DONE records=\d+ read_errors=0 failed=0\r?\n' -and $Text -notmatch 'MOTION_ABORT'
}
if($SelfTest){
    $valid="MOTION_DONE time_us=40000000`nRAW_INPUT_DONE records=100 read_errors=0 failed=0`n"
    if(!(Test-TimedMotionComplete 0 $valid) -or (Test-TimedMotionComplete 1 $valid) -or (Test-TimedMotionComplete 0 ($valid+'MOTION_ABORT')) -or (Test-TimedMotionComplete 0 ($valid.Replace('read_errors=0','read_errors=1'))) -or (Test-TimedMotionComplete 0 ($valid.Replace('MOTION_DONE','PARTIAL')))){throw 'Timed completion guard failed'}
    if('USB\VID_2FE3&PID_0008\fixture' -notmatch (Get-TimedSourcePattern 'receiver') -or 'USB\VID_258A&PID_2018\fixture' -notmatch (Get-TimedSourcePattern 'wired') -or 'USB\VID_2FE3&PID_0008\fixture' -match (Get-TimedSourcePattern 'wired')){throw 'Timed source identity guard failed'}
    Write-Output 'PASS: timed completion/error/abort gates; no hardware access';exit 0
}
$root=Join-Path (Split-Path $PSScriptRoot -Parent) 'build-pairing-analysis'
$exe=Join-Path $root 'capture_raw_mouse_timed.exe'
$validation=Get-Content (Join-Path $root 'timed-raw-software-validation.json') -Raw | ConvertFrom-Json
if(!$validation.self_test_pass -or (Get-FileHash $exe -Algorithm SHA256).Hash -ne $validation.binary_sha256 -or (Get-FileHash (Join-Path $PSScriptRoot 'capture_raw_mouse.c') -Algorithm SHA256).Hash -ne $validation.source_sha256 -or (Get-FileHash (Join-Path $PSScriptRoot 'motion_epoch.h') -Algorithm SHA256).Hash -ne $validation.header_sha256){throw 'Timed capture needs a current validated build'}
$pattern=Get-TimedSourcePattern $Source
$devices=@(Get-CimInstance Win32_PnPEntity | Where-Object {$_.PNPDeviceID -match $pattern -and $_.Status -eq 'OK'})
if($devices.Count -ne 1){throw 'Unique requested mouse source missing'}
if($Source -eq 'receiver' -and (Get-PnpDeviceProperty -InstanceId $devices[0].PNPDeviceID -KeyName DEVPKEY_Device_BusReportedDeviceDesc).Data -ne 'DIY Mouse Dongle v14'){throw 'Normal v14 USB identity required'}
$label=if($Source -eq 'receiver'){'v14'}else{'wired'}
$job=Join-Path $root ($label+'-timed-motion-'+(Get-Date -Format yyyyMMdd-HHmmss)+'-'+[guid]::NewGuid().ToString('N').Substring(0,8))
$result=@{success=$false;source=$Source;raw_binary_sha256=$validation.binary_sha256;serial_opened=$false;capture_limit_seconds=180;start_utc=[DateTime]::UtcNow.ToString('o')};$raw=$null
try{
    $raw=Start-Process -FilePath $exe -ArgumentList @('180',('"'+$job+'.csv"'),'--timed-motion') -WindowStyle Normal -PassThru -RedirectStandardOutput ($job+'.stdout.txt') -RedirectStandardError ($job+'.stderr.txt')
    $clock=[Diagnostics.Stopwatch]::StartNew()
    do{Start-Sleep -Milliseconds 20;$ready=(Get-Content ($job+'.stdout.txt') -Raw) -match 'TIMED_MOTION_READY'}while(!$ready -and !$raw.HasExited -and $clock.ElapsedMilliseconds -lt 3000)
    if(!$ready){throw 'Timed window readiness failed'}
    Write-Output ('TIMED_CAPTURE_BASE='+$job);Write-Output 'TIMED_MOTION_CAPTURE_RUNNING: click window,follow5s countdown/30s MOVE/5s STOP; no arm or firmware stop'
    while(!$raw.HasExited -and $clock.ElapsedMilliseconds -lt 185000){Start-Sleep -Milliseconds 100}
    if(!$raw.HasExited){throw 'Timed recording exceeded its bounded limit'}
    $raw.WaitForExit()
    $text=Get-Content ($job+'.stdout.txt') -Raw
    if(!(Test-TimedMotionComplete $raw.ExitCode $text)){throw 'Motion epoch incomplete/invalid; no automatic retry'}
    $result.raw_exit=$raw.ExitCode
    $summary=& python (Join-Path $PSScriptRoot 'analyze_motion_delivery.py') ($job+'.stdout.txt') ($job+'.csv') --source $Source
    if($LASTEXITCODE -ne 0){throw 'Timed delivery analysis rejected the closed capture'}
    $summary | Set-Content ($job+'.summary.json');$result.success=$true
    Write-Output 'TIMED_MOTION_COMPLETE: validate user continuous-motion observation';$summary
}catch{$result.error=$_.Exception.Message;throw}finally{
    if($raw){if(!$raw.HasExited){$raw.Kill();$result.raw_partial=$true};$raw.Dispose()}
    $result.finished_utc=[DateTime]::UtcNow.ToString('o');$result | ConvertTo-Json | Set-Content ($job+'.session.json')
}
