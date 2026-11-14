param(
    [string]$HidSharpPath = "$PSScriptRoot\..\build-pairing-analysis\HidSharp.dll",
    [switch]$SelfTest
)

$ErrorActionPreference = 'Stop'

function New-AddressRequest {
    $packet = New-Object byte[] 65
    $packet[4] = 4
    $packet[6] = 0x8A
    return ,$packet
}

function Read-AddressResponse([byte[]]$Packet) {
    if ($Packet.Length -ne 65 -or $Packet[1] -notin @(0xA0, 0xA1)) {
        throw 'Unexpected address response; no address accepted.'
    }
    return (($Packet[7..10] | ForEach-Object { $_.ToString('X2') }) -join ':')
}

if ($SelfTest) {
    $request = New-AddressRequest
    if ($request.Length -ne 65 -or $request[4] -ne 4 -or $request[6] -ne 0x8A -or
        @($request | Where-Object { $_ -ne 0 }).Count -ne 2) {
        throw 'Address request test failed.'
    }
    foreach ($status in @(0xA0, 0xA1)) {
        $response = New-Object byte[] 65
        $response[1] = $status
        $response[7] = 0x12; $response[8] = 0x34
        $response[9] = 0x56; $response[10] = 0x78
        if ((Read-AddressResponse $response) -ne '12:34:56:78') {
            throw 'Address parsing test failed.'
        }
    }
    foreach ($bad in @((New-Object byte[] 65), (New-Object byte[] 10))) {
        $rejected = $false
        try { Read-AddressResponse $bad | Out-Null } catch { $rejected = $true }
        if (-not $rejected) { throw 'Invalid response accepted.' }
    }
    'Address request and response self-tests passed; no hardware accessed.'
    return
}

# The utility bundles a 32-bit HidSharp assembly; use Windows SysWOW64 PowerShell.
if ([IntPtr]::Size -ne 4) { throw 'Run with 32-bit Windows PowerShell (SysWOW64).' }
Add-Type -Path (Resolve-Path -LiteralPath $HidSharpPath).Path
$devices = @([HidSharp.DeviceList]::Local.GetHidDevices(0x258A, 0x2018) |
    Where-Object { $_.GetMaxFeatureReportLength() -eq 65 })
if ($devices.Count -ne 1) { throw "Expected one matching mouse interface; found $($devices.Count)." }
$stream = $devices[0].Open()
try {
    $stream.ReadTimeout = 2000
    $stream.WriteTimeout = 2000
    $request = New-AddressRequest
    $response = New-Object byte[] 65
    # Observed utility query: SET_FEATURE request, 200 ms, then GET_FEATURE response.
    $stream.SetFeature($request)
    Start-Sleep -Milliseconds 200
    $stream.GetFeature($response)
    [pscustomobject]@{
        Device = '258A:2018'
        Interface = $devices[0].DevicePath
        Address = Read-AddressResponse $response
        RequestHex = [BitConverter]::ToString($request)
        ResponseHex = [BitConverter]::ToString($response)
    } | ConvertTo-Json
} finally {
    $stream.Dispose()
}
