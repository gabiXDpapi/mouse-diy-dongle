param(
    [ValidatePattern('^[a-z0-9-]{1,80}$')][string]$RunName,
    [switch]$SelfTest
)
$ErrorActionPreference = 'Stop'

function Get-VerifiedReceiver($Candidates, [string]$ExpectedId) {
    if ($ExpectedId -notmatch '^USB\\VID_2FE3&PID_0008\\[a-z0-9&_-]+$') { throw 'Invalid receiver instance format' }
    $targets = @($Candidates | Where-Object { $_.PNPDeviceID -match '^USB\\VID_2FE3&PID_0008\\' })
    if ($targets.Count -ne 1 -or $targets[0].PNPDeviceID -ne $ExpectedId -or $targets[0].Status -ne 'OK') {
        throw 'Exact unique healthy receiver USB root not verified;no restart'
    }
    return $targets[0]
}
function Test-RestartGate([bool]$Cancelled, [string]$Signal, [DateTime]$Now, [DateTime]$Expires) {
    return !$Cancelled -and $Signal -ceq 'restart' -and $Now -lt $Expires
}
if ($SelfTest) {
    $id = 'USB\VID_2FE3&PID_0008\TEST'
    $device = [pscustomobject]@{PNPDeviceID=$id;Status='OK'}
    if ((Get-VerifiedReceiver @($device) $id).PNPDeviceID -ne $id) { throw 'Valid target rejected' }
    foreach ($fixture in @(@(), @($device,$device), @([pscustomobject]@{PNPDeviceID='USB\VID_258A&PID_2018\TEST';Status='OK'}))) {
        $rejected=$false;try { Get-VerifiedReceiver $fixture $id | Out-Null } catch { $rejected=$true }
        if (!$rejected) { throw 'Invalid target accepted' }
    }
    $rejected=$false;try { Get-VerifiedReceiver @($device) 'different' | Out-Null } catch { $rejected=$true }
    if (!$rejected) { throw 'Mismatched instance accepted' }
    $rejected=$false;try { Get-VerifiedReceiver @($device) ($id+'" /force') | Out-Null } catch { $rejected=$true }
    if (!$rejected) { throw 'Invalid argument characters accepted' }
    $now=[DateTime]::UtcNow;$expires=$now.AddSeconds(10)
    if (!(Test-RestartGate $false 'restart' $now $expires)) { throw 'Valid signal rejected' }
    if ((Test-RestartGate $true 'restart' $now $expires) -or (Test-RestartGate $false 'other' $now $expires) -or (Test-RestartGate $false 'restart' $expires $expires)) { throw 'Unsafe signal accepted' }
    Write-Output 'PASS: exact target,duplicate/foreign/mismatch rejection,cancel/signal/expiry gates;no hardware access'
    exit 0
}
if (!$RunName) { throw 'RunName required' }
$privateRoot = Join-Path (Split-Path $PSScriptRoot -Parent) 'build-pairing-analysis'
$job = Join-Path $privateRoot $RunName
$result = @{run_name=$RunName;success=$false;requested_utc=[DateTime]::UtcNow.ToString('o')}
try {
    if ((Test-Path "$job.ready.json") -or (Test-Path "$job.result.json")) { throw 'Job already used' }
    $principal=[Security.Principal.WindowsPrincipal]::new([Security.Principal.WindowsIdentity]::GetCurrent())
    if (!$principal.IsInRole([Security.Principal.WindowsBuiltInRole]::Administrator)) { throw 'Windows administrator token required' }
    $expected=Get-Content (Join-Path $privateRoot 'receiver-usb-restart-target.json') -Raw | ConvertFrom-Json
    Get-VerifiedReceiver @(Get-CimInstance Win32_PnPEntity) $expected.instance_id | Out-Null
    $expires=[DateTime]::UtcNow.AddSeconds(180)
    if (Test-Path "$job.cancel") { throw 'Job cancelled' }
    @{run_name=$RunName;pid=$PID;expires_utc=$expires.ToString('o')} | ConvertTo-Json | Set-Content "$job.ready.tmp"
    [IO.File]::Move("$job.ready.tmp","$job.ready.json")
    while (!(Test-Path "$job.signal")) {
        if ((Test-Path "$job.cancel") -or [DateTime]::UtcNow -ge $expires) { throw 'Job cancelled or expired;no restart' }
        Start-Sleep -Milliseconds 125
    }
    $target=Get-VerifiedReceiver @(Get-CimInstance Win32_PnPEntity) $expected.instance_id
    if (!(Test-RestartGate (Test-Path "$job.cancel") ([IO.File]::ReadAllText("$job.signal")) ([DateTime]::UtcNow) $expires)) { throw 'Restart gate rejected;no restart' }
    $result.restart_utc=[DateTime]::UtcNow.ToString('o')
    $native = Start-Process -FilePath (Join-Path $env:WINDIR 'System32/pnputil.exe') -ArgumentList @('/restart-device',('"' + $target.PNPDeviceID + '"')) -WindowStyle Hidden -PassThru -RedirectStandardOutput "$job.pnputil.txt" -RedirectStandardError "$job.pnputil.stderr.txt"
    if (!$native.WaitForExit(15000)) { $native.Kill(); throw 'PnP command timed out;do not retry automatically' }
    $result.exit_code=$native.ExitCode
    $result.success=$native.ExitCode -eq 0
    $native.Dispose()
} catch {
    $result.error=$_.Exception.Message
} finally {
    $result.finished_utc=[DateTime]::UtcNow.ToString('o')
    $result | ConvertTo-Json | Set-Content "$job.result.json"
}
if (!$result.success) { exit 1 }
