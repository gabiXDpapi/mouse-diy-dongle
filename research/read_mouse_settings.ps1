param(
    [string]$HidSharpPath = "$PSScriptRoot\..\build-pairing-analysis\HidSharp.dll",
    [switch]$SelfTest
)

$ErrorActionPreference = 'Stop'

. "$PSScriptRoot\mouse_settings_protocol.ps1"

if ($SelfTest) {
    foreach ($kind in @('Version','Polling','DpiStages','ActiveDpi')) {
        $packet = New-SettingsQuery $kind
        if ($packet.Length -ne 65 -or $packet[3] -ne 2 -or $packet[6] -lt 0x80 -or
            @($packet[8..64] | Where-Object { $_ -ne 0 }).Count) { throw 'Query construction failed.' }
        $packet[1] = 0xA1
        switch ($kind) {
            Version { $packet[7] = 1; $expected = '1.0.0.0' }
            Polling { $packet[7] = 1; $expected = 1000 }
            ActiveDpi { $packet[8] = 3; $expected = 3 }
            DpiStages {
                $packet[4] = 6; $packet[8] = 1
                $packet[9] = 6; $packet[10] = 64; $packet[11] = 6; $packet[12] = 64
            }
        }
        $value = Read-SettingsReply $kind $packet
        if ($kind -eq 'DpiStages') {
            if ($value.Count -ne 1 -or $value[0].X -ne 1600 -or $value[0].Y -ne 1600) { throw 'DPI parse failed.' }
        } elseif ($value -ne $expected) { throw 'Reply parse failed.' }
        foreach ($status in @(0,0xA0,0xA2,0xA3,0xA4)) {
            $packet[1] = $status
            $rejected = $false
            try { Read-SettingsReply $kind $packet | Out-Null } catch { $rejected = $true }
            if (-not $rejected) { throw 'Failed/busy reply accepted.' }
        }
    }
    $bad = New-SettingsQuery DpiStages; $bad[1] = 0xA1; $bad[8] = 7
    foreach ($packet in @($bad, (New-Object byte[] 10))) {
        $rejected = $false
        try { Read-SettingsReply DpiStages $packet | Out-Null } catch { $rejected = $true }
        if (-not $rejected) { throw 'Malformed reply accepted.' }
    }
    foreach ($case in @(@('Polling',7,0), @('Polling',6,0), @('ActiveDpi',8,7),
                        @('DpiStages',7,0), @('DpiStages',4,0))) {
        $kind = $case[0]
        $packet = New-SettingsQuery $kind; $packet[1] = 0xA1
        $packet[8] = 1; $packet[4] = 6
        $packet[9] = 6; $packet[10] = 64; $packet[11] = 6; $packet[12] = 64
        $packet[[int]$case[1]] = [byte]$case[2]
        $rejected = $false
        try { Read-SettingsReply $kind $packet | Out-Null } catch { $rejected = $true }
        if (-not $rejected) { throw 'Invalid field accepted.' }
    }
    'Settings query/reply self-tests passed; no hardware accessed.'
    return
}

if ([IntPtr]::Size -ne 4) { throw 'Run with 32-bit Windows PowerShell (SysWOW64).' }
if (Get-Process -Name 'Glorious Core' -ErrorAction SilentlyContinue) { throw 'Close Glorious CORE before the read-only query.' }
Add-Type -Path (Resolve-Path -LiteralPath $HidSharpPath).Path
$devices = @([HidSharp.DeviceList]::Local.GetHidDevices(0x258A, 0x2018) |
    Where-Object { $_.GetMaxFeatureReportLength() -eq 65 })
if ($devices.Count -ne 1) { throw "Expected one wired mouse configuration interface; found $($devices.Count)." }
$stream = $devices[0].Open()
try {
    $stream.ReadTimeout = 2000
    $stream.WriteTimeout = 2000
    $result = [ordered]@{ Device = '258A:2018'; Profile = 1; Queries = @() }
    foreach ($kind in @('Version','Polling','DpiStages','ActiveDpi')) {
        $request = New-SettingsQuery $kind
        $stream.SetFeature($request)
        $response = New-Object byte[] 65
        for ($attempt = 0; $attempt -lt 10; $attempt++) {
            Start-Sleep -Milliseconds 50
            [Array]::Clear($response,0,$response.Length)
            $stream.GetFeature($response)
            if ($response[1] -ne 0xA0) { break }
        }
        $value = Read-SettingsReply $kind $response
        $result[$kind] = $value
        $result.Queries += [pscustomobject]@{
            Kind = $kind; RequestHex = [BitConverter]::ToString($request)
            ResponseHex = [BitConverter]::ToString($response)
        }
    }
    if ($result.ActiveDpi -gt $result.DpiStages.Count) { throw 'Selected stage exceeds returned stage count.' }
    [pscustomobject]$result | ConvertTo-Json -Depth 6
} finally {
    $stream.Dispose()
}
