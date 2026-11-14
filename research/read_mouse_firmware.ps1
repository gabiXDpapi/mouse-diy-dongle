param([switch]$SelfTest)

$ErrorActionPreference = 'Stop'
# Protocol source and limitations: PAIRING_FINDINGS.md. This tool has no flash-write commands.
$StartAddress = 0x6000
$EndAddress = 0x1CCA0

function New-ReadPacket([int]$Address) {
    if ($Address -lt $StartAddress -or $Address -ge $EndAddress -or $Address % 32) {
        throw 'Read address is outside the fixed, aligned application window.'
    }
    $packet = New-Object byte[] 65
    $packet[4] = 32; $packet[5] = 0xB0; $packet[6] = 0x83; $packet[7] = 32
    $packet[8] = ($Address -shr 24) -band 255
    $packet[9] = ($Address -shr 16) -band 255
    $packet[10] = ($Address -shr 8) -band 255
    $packet[11] = $Address -band 255
    return ,$packet
}

function Decode-ReadResponse([byte[]]$Packet) {
    if ($Packet.Length -ne 65 -or $Packet[1] -notin @(0xA1, 0x02)) {
        throw 'Firmware read failed or returned an invalid response.'
    }
    return ,([byte[]]($Packet[12..43] | ForEach-Object { $_ -bxor 0x55 }))
}

if ($SelfTest) {
    $packet = New-ReadPacket 0x6000
    if ([BitConverter]::ToString($packet[0..11]) -ne '00-00-00-00-20-B0-83-20-00-00-60-00') {
        throw 'Read packet mismatch.'
    }
    foreach ($address in @(0, 0x6001, $EndAddress)) {
        $rejected = $false
        try { New-ReadPacket $address | Out-Null } catch { $rejected = $true }
        if (-not $rejected) { throw 'Invalid address accepted.' }
    }
    $response = New-Object byte[] 65
    $response[1] = 0xA1
    for ($i = 0; $i -lt 32; $i++) { $response[12 + $i] = $i -bxor 0x55 }
    $decoded = Decode-ReadResponse $response
    if ($decoded.Length -ne 32 -or $decoded[0] -ne 0 -or $decoded[31] -ne 31) {
        throw 'Wire decoding mismatch.'
    }
    $response[1] = 0xA2
    $rejected = $false
    try { Decode-ReadResponse $response | Out-Null } catch { $rejected = $true }
    if (-not $rejected) { throw 'Failed read accepted.' }
    'Firmware packet, bounds, and response self-tests passed; no hardware accessed.'
    return
}

if ([IntPtr]::Size -ne 4) { throw 'Run with 32-bit Windows PowerShell (SysWOW64).' }
$analysisDir = (Resolve-Path "$PSScriptRoot\..\build-pairing-analysis").Path
Add-Type -Path "$analysisDir\HidSharp.dll"

function Find-Mouse([int]$ProductId) {
    $devices = @([HidSharp.DeviceList]::Local.GetHidDevices(0x258A, $ProductId) |
        Where-Object { $_.GetMaxFeatureReportLength() -eq 65 })
    if ($devices.Count -gt 1) { throw 'Multiple matching devices; cannot select safely.' }
    if ($devices.Count -eq 1) { return $devices[0] }
}

function Query-Device($Stream, [byte[]]$Packet) {
    $Stream.SetFeature($Packet)
    for ($attempt = 0; $attempt -lt 5; $attempt++) {
        Start-Sleep -Milliseconds 20
        $response = New-Object byte[] 65
        $Stream.GetFeature($response)
        if ($response[1] -in @(0xA1, 0x02)) { return ,$response }
        if ($response[1] -in @(0xA2, 0xA3, 0xA4)) { throw "Device returned status $($response[1])." }
    }
    throw 'No successful response within five polls.'
}

$mouse = Find-Mouse 0x2018
if ($null -eq $mouse) { throw 'Expected wired mouse 258A:2018; no mode change attempted.' }
$app = $mouse.Open()
try {
    $versionRequest = New-Object byte[] 65
    $versionRequest[4] = 6; $versionRequest[6] = 0x81
    $version = Query-Device $app $versionRequest
    if ([BitConverter]::ToString($version[7..12]) -ne '01-00-00-00-20-18') {
        throw 'Unexpected firmware/version identity; no mode change attempted.'
    }
    # CreateNew protects earlier snapshots and checks output access before restarting the mouse.
    $outputPath = "$analysisDir\mouse-app-6000-1cc9f.bin"
    $file = [IO.File]::Open($outputPath, [IO.FileMode]::CreateNew, [IO.FileAccess]::Write)
    $boot = $null
    $entryAttempted = $false
    $complete = $false
    try {
        $entry = New-Object byte[] 65
        $entry[4] = 1; $entry[7] = 0xB0
        $entryAttempted = $true
        try { $app.SetFeature($entry) } catch { Write-Host 'Entry transfer failed; checking for bootloader re-enumeration.' }
        $app.Dispose()
        $deadline = [DateTime]::UtcNow.AddSeconds(15)
        do {
            Start-Sleep -Milliseconds 200
            $bootDevice = Find-Mouse 0x2103
        } while ($null -eq $bootDevice -and [DateTime]::UtcNow -lt $deadline)
        if ($null -eq $bootDevice) { throw 'Bootloader not found; reconnect mouse and check normal operation.' }
        $boot = $bootDevice.Open()
        $versionRequest = New-Object byte[] 65
        $versionRequest[4] = 6; $versionRequest[5] = 0xB0; $versionRequest[6] = 0x80
        $bootVersion = Query-Device $boot $versionRequest
        Write-Host ('Bootloader version: ' + [BitConverter]::ToString($bootVersion[9..12]))
        for ($address = $StartAddress; $address -lt $EndAddress; $address += 32) {
            $reply = Query-Device $boot (New-ReadPacket $address)
            $bytes = Decode-ReadResponse $reply
            $file.Write($bytes, 0, $bytes.Length)
            if (($address - $StartAddress) % 8192 -eq 0) { Write-Host ('Read through 0x{0:X}' -f $address) }
        }
        $complete = $true
    } finally {
        $file.Dispose()
        if ($entryAttempted) {
            if ($null -eq $boot) {
                $bootDevice = Find-Mouse 0x2103
                if ($null -ne $bootDevice) { $boot = $bootDevice.Open() }
            }
            if ($null -ne $boot) {
                $exitPacket = New-Object byte[] 65
                $exitPacket[4] = 1; $exitPacket[5] = 0xB0; $exitPacket[6] = 4; $exitPacket[7] = 1
                try { $boot.SetFeature($exitPacket) } catch { Write-Host 'Exit transfer failed; checking for application re-enumeration.' }
                $boot.Dispose()
            }
            $deadline = [DateTime]::UtcNow.AddSeconds(15)
            do {
                Start-Sleep -Milliseconds 200
                $restored = Find-Mouse 0x2018
            } while ($null -eq $restored -and [DateTime]::UtcNow -lt $deadline)
            if ($null -eq $restored) { Write-Warning 'Mouse application not detected. Reconnect the mouse; do not flash or erase it.' }
            else { Write-Host 'Mouse application 258A:2018 detected again.' }
        }
        if (-not $complete) { Write-Warning "Incomplete snapshot retained at $outputPath; do not treat it as a backup." }
    }
    Get-FileHash -LiteralPath $outputPath -Algorithm SHA256
} finally {
    $app.Dispose()
}
