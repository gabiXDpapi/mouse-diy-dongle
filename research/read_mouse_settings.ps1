param(
    [string]$HidSharpPath = "$PSScriptRoot\..\build-pairing-analysis\HidSharp.dll",
    [switch]$SelfTest
)

$ErrorActionPreference = 'Stop'

function New-SettingsQuery([ValidateSet('Version','Polling','DpiStages','ActiveDpi')][string]$Kind) {
    $packet = New-Object byte[] 65
    $packet[3] = 2
    $packet[4] = 1
    $packet[5] = 1
    $packet[7] = 1 # Profile 1 only; queries never select/change a profile.
    switch ($Kind) {
        Version { $packet[4] = 3; $packet[5] = 0; $packet[6] = 0x81; $packet[7] = 0 }
        Polling { $packet[6] = 0x80; $packet[7] = 0 }
        DpiStages { $packet[6] = 0x81 }
        ActiveDpi { $packet[6] = 0x82 }
    }
    return ,$packet
}

function Read-SettingsReply([string]$Kind, [byte[]]$Packet) {
    $request = New-SettingsQuery $Kind
    if ($Packet.Length -ne 65 -or $Packet[0] -ne 0 -or $Packet[1] -ne 0xA1 -or
        $Packet[5] -ne $request[5] -or $Packet[6] -ne $request[6]) {
        throw 'Invalid, busy or mismatched settings reply; no settings accepted.'
    }
    switch ($Kind) {
        Version { return ($Packet[7..10] -join '.') }
        Polling {
            if ($Packet[7] -notin @(1,2,4,8)) { throw 'Unknown polling divider.' }
            return (1000 / $Packet[7])
        }
        DpiStages {
            $count = [int]$Packet[8]
            if ($Packet[7] -ne 1 -or $count -lt 1 -or $count -gt 6 -or $Packet[4] -ne (2 + 4 * $count)) {
                throw 'Invalid DPI profile/stage count or payload length.'
            }
            $stages = @(for ($i = 0; $i -lt $count; $i++) {
                $at = 9 + 4 * $i
                $x = 256 * [int]$Packet[$at] + $Packet[$at+1]
                $y = 256 * [int]$Packet[$at+2] + $Packet[$at+3]
                if ($x -lt 50 -or $x -gt 19000 -or $y -lt 50 -or $y -gt 19000) { throw 'DPI outside sensor range.' }
                [pscustomobject]@{ X = $x; Y = $y }
            })
            return ,$stages
        }
        ActiveDpi {
            if ($Packet[7] -ne 1 -or $Packet[8] -lt 1 -or $Packet[8] -gt 6) { throw 'Invalid selected DPI stage.' }
            return [int]$Packet[8]
        }
    }
}

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
