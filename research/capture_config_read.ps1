param([switch]$SelfTest)
$ErrorActionPreference = 'Stop'

function Read-ConfigSummary([string]$Text) {
    $match = [regex]::Match($Text, '\ACONFIG_READ_V1 started=([01]) offset=(\d+) attempts=(\d+) bytes=(\d+) complete=([01]) failed=([01])\r?\nCONFIG_REPLY ([0-9a-f]*)\r?\nCONFIG_END\r?\n\z')
    if (!$match.Success) { throw 'Incomplete or unexpected diagnostic summary.' }
    $size = [int]$match.Groups[4].Value
    $offset = [int]$match.Groups[2].Value
    $attempts = [int]$match.Groups[3].Value
    $hex = $match.Groups[7].Value
    if ($size -gt 64 -or $size % 8 -or $hex.Length -ne 2*$size -or
        $offset -gt 64 -or $offset % 8 -or $attempts -gt 512) { throw 'Invalid diagnostic counters.' }
    $complete = $match.Groups[5].Value -eq '1'
    $failed = $match.Groups[6].Value -eq '1'
    $version = $null
    if ($complete) {
        if ($failed -or $offset -ne 64 -or $size -lt 16 -or $hex -notmatch '^a10002030081') {
            throw 'Completed reply does not match the allowlisted version query.'
        }
        $version = (@(6..9 | ForEach-Object { [Convert]::ToInt32($hex.Substring(2*$_,2),16) }) -join '.')
    }
    [pscustomobject]@{ Started = $match.Groups[1].Value -eq '1'; Offset = $offset
        Attempts = $attempts; Bytes = $size; Complete = $complete; Failed = $failed; Version = $version }
}

if ($SelfTest) {
    $sample = "CONFIG_READ_V1 started=1 offset=64 attempts=8 bytes=16 complete=1 failed=0`nCONFIG_REPLY a1000203008101000000201800000000`nCONFIG_END`n"
    $value = Read-ConfigSummary $sample
    if (!$value.Complete -or $value.Version -ne '1.0.0.0') { throw 'Version parse failed.' }
    foreach ($bad in @($sample.TrimEnd(), $sample.Replace('bytes=16','bytes=64'),
                      $sample.Replace('failed=0','failed=1'), $sample.Replace('a10002030081','a10002030001'))) {
        $refused = $false
        try { Read-ConfigSummary $bad | Out-Null } catch { $refused = $true }
        if (!$refused) { throw 'Invalid summary accepted.' }
    }
    $empty = "CONFIG_READ_V1 started=0 offset=0 attempts=0 bytes=0 complete=0 failed=0`nCONFIG_REPLY `nCONFIG_END`n"
    if ((Read-ConfigSummary $empty).Complete) { throw 'No-traffic result marked successful.' }
    'Config capture parser self-tests passed; no hardware accessed.'
    return
}

$root = Join-Path (Split-Path $PSScriptRoot -Parent) 'build-pairing-analysis'
$job = Join-Path $root ('config-read-v20-'+(Get-Date -Format yyyyMMdd-HHmmss)+'-'+[guid]::NewGuid().ToString('N').Substring(0,8))
$port = $null; $stopped = $false; $text = ''
$result = [ordered]@{ Scope = 'One fixed wireless firmware-version read; no pointer output or settings writes'; Success = $false }
try {
    $devices = @(Get-CimInstance Win32_PnPEntity | Where-Object { $_.PNPDeviceID -match 'VID_2FE3&PID_0008' -and $_.Status -eq 'OK' })
    $usb = @($devices | Where-Object { $_.PNPDeviceID -match '^USB\\VID_2FE3&PID_0008\\' })
    $cdc = @($devices | Where-Object { $_.Name -match '\(COM\d+\)' })
    if ($usb.Count -ne 1 -or $cdc.Count -ne 1) { throw 'Unique receiver/CDC missing; no commands sent.' }
    if ((Get-PnpDeviceProperty -InstanceId $usb[0].PNPDeviceID -KeyName DEVPKEY_Device_BusReportedDeviceDesc).Data -ne 'DIY Mouse Config Read v20') {
        throw 'Read-only diagnostic product not verified; no commands sent.'
    }
    $name = [regex]::Match($cdc[0].Name,'\((COM\d+)\)').Groups[1].Value
    $port = [IO.Ports.SerialPort]::new($name,115200)
    $port.DtrEnable = $true; $port.WriteTimeout = 1000; $port.Open()
    Start-Sleep -Milliseconds 300
    if ($port.ReadExisting().Length) { throw 'Unexpected startup serial output.' }
    Write-Output "CAPTURE_BASE=$job"
    Write-Output 'CONFIG_CAPTURE_RUNNING: mouse may turn ON; no arming or pointer output'
    $clock = [Diagnostics.Stopwatch]::StartNew()
    while ($clock.ElapsedMilliseconds -lt 35000) {
        if ($port.ReadExisting().Length) { throw 'Unexpected serial text during active RF.' }
        Start-Sleep -Milliseconds 20
    }
    $port.Write('x'); $stopped = $true
    Start-Sleep -Milliseconds 300
    $port.Write('d')
    while ($clock.ElapsedMilliseconds -lt 40000) {
        $text += $port.ReadExisting()
        if ($text.EndsWith("CONFIG_END`n")) { break }
        Start-Sleep -Milliseconds 20
    }
    $result.Summary = Read-ConfigSummary $text
    $result.Success = $result.Summary.Complete -and !$result.Summary.Failed
    Write-Output ('CONFIG_CAPTURE_FINISHED: query_success='+$result.Success)
} catch {
    $result.Error = $_.Exception.Message
    throw
} finally {
    if ($port) {
        if ($port.IsOpen) {
            if (!$stopped) { try { $port.Write('x') } catch {} }
            $port.Close()
        }
        $port.Dispose()
    }
    $result.FinishedUtc = [DateTime]::UtcNow.ToString('o')
    $result | ConvertTo-Json -Depth 5 | Set-Content -Encoding UTF8 "$job.json"
    $text | Set-Content -Encoding UTF8 "$job.txt"
}
