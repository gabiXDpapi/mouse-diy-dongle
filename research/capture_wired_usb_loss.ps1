param([DateTimeOffset]$FinishBeforeUtc,[switch]$SelfTest)
$ErrorActionPreference='Stop'
$seconds=if($SelfTest){1}else{60}
if(!$SelfTest -and (!$PSBoundParameters.ContainsKey('FinishBeforeUtc') -or [DateTimeOffset]::UtcNow.AddSeconds($seconds+60) -ge $FinishBeforeUtc)){throw 'Insufficient time before user break;no capture started'}
$privateRoot=Join-Path (Split-Path $PSScriptRoot -Parent) 'build-pairing-analysis'
$prefix=if($SelfTest){'wired-usbloss-smoke-'}else{'wired-usbloss-'}
$job=Join-Path $privateRoot ($prefix+(Get-Date -Format 'yyyyMMdd-HHmmss')+'-'+[guid]::NewGuid().ToString('N').Substring(0,8))
if(-not('ReceiverMouseButtonProbe' -as [type])){
    Add-Type -TypeDefinition @'
using System.Runtime.InteropServices;
public static class ReceiverMouseButtonProbe {
 [DllImport("user32.dll")] public static extern short GetAsyncKeyState(int key);
}
'@
}
$journal=[IO.StreamWriter]::new("$job.host.jsonl",$false);$process=$null;$clock=[Diagnostics.Stopwatch]::StartNew()
$result=@{scope='wired USB-unplug reference;no serial/RF/PnP';self_test=[bool]$SelfTest;success=$false}
try{
    $process=Start-Process -FilePath (Join-Path $privateRoot 'capture_raw_mouse.exe') -ArgumentList @([string]$seconds,('"'+"$job.csv"+'"')) -WindowStyle Hidden -PassThru -RedirectStandardOutput "$job.raw.stdout.txt" -RedirectStandardError "$job.raw.stderr.txt"
    Write-Output "CAPTURE_BASE=$job"
    $shown=$false
    while(!$process.HasExited){
        if(!$shown -and (Test-Path "$job.raw.stdout.txt") -and (Get-Content "$job.raw.stdout.txt" -Raw) -match 'RAW_INPUT_READY'){$shown=$true;Write-Output 'WIRED_CAPTURE_READY: read-only,no arming'}
        $journal.WriteLine((@{host_utc=[DateTime]::UtcNow.ToString('o');elapsed_ms=$clock.ElapsedMilliseconds;left_down=(([ReceiverMouseButtonProbe]::GetAsyncKeyState(1) -band 0x8000) -ne 0)}|ConvertTo-Json -Compress));$journal.Flush()
        if($clock.ElapsedMilliseconds -gt $seconds*1000+5000){throw 'Wired logger exceeded bound'}
        Start-Sleep -Milliseconds 250
    }
    $result.raw_exit=$process.ExitCode
    $stdout=Get-Content "$job.raw.stdout.txt" -Raw
    if($process.ExitCode -ne 0 -or $stdout -notmatch 'RAW_INPUT_DONE.*read_errors=0 failed=0'){throw 'Raw logger did not complete cleanly'}
    if($SelfTest -and !(Get-Content "$job.csv" -TotalCount 1).StartsWith('time_us,source,dx,dy,')){throw 'Capture header missing'}
    $result.success=$true;Write-Output $stdout
    if($SelfTest){Write-Output 'PASS: bounded1s wrapper/native lifecycle and host-state log;no serial/arming/PnP'}
}catch{$result.error=$_.Exception.Message;throw}finally{
    if($process){if(!$process.HasExited){$process.Kill()};$process.Dispose()}
    $journal.Dispose();$result.finished_utc=[DateTime]::UtcNow.ToString('o');$result|ConvertTo-Json|Set-Content "$job.capture.json"
}
