param(
    [string]$HidSharpPath = "$PSScriptRoot\..\build-pairing-analysis\HidSharp.dll",
    [switch]$SelfTest
)
$ErrorActionPreference = 'Stop'
function New-ReceiverVersionQuery {
    $packet = New-Object byte[] 65
    $packet[3] = 2; $packet[4] = 3; $packet[6] = 0x81
    return ,$packet
}
function Read-ReceiverVersionReply([byte[]]$Packet) {
    if ($Packet.Length -ne 65 -or $Packet[0] -ne 0 -or $Packet[1] -ne 0xA1 -or
        $Packet[3] -ne 2 -or $Packet[4] -ne 3 -or $Packet[5] -ne 0 -or $Packet[6] -ne 0x81) {
        throw 'No successful matching mouse-version reply.'
    }
    return ($Packet[7..10] -join '.')
}
if ($SelfTest) {
    $packet = New-ReceiverVersionQuery
    if ($packet.Length -ne 65 -or @($packet | Where-Object { $_ -ne 0 }).Count -ne 3) { throw 'Query malformed.' }
    $packet[1] = 0xA1; $packet[7] = 1
    if ((Read-ReceiverVersionReply $packet) -ne '1.0.0.0') { throw 'Version parse failed.' }
    foreach ($at in @(0,1,3,4,5,6)) {
        $bad = $packet.Clone(); $bad[$at] = $bad[$at] -bxor 1
        $refused = $false
        try { Read-ReceiverVersionReply $bad | Out-Null } catch { $refused = $true }
        if (!$refused) { throw 'Mismatched reply accepted.' }
    }
    $refused = $false
    try { Read-ReceiverVersionReply (New-Object byte[] 10) | Out-Null } catch { $refused = $true }
    if (!$refused) { throw 'Short reply accepted.' }
    'Receiver version request/reply self-tests passed; no hardware accessed.'
    return
}
if ([IntPtr]::Size -ne 4) { throw 'Run in 32-bit SysWOW64 PowerShell.' }
if (Get-Process -Name 'Glorious Core' -ErrorAction SilentlyContinue) { throw 'Close CORE first.' }
$root = @(Get-CimInstance Win32_PnPEntity | Where-Object {
    $_.PNPDeviceID -match '^USB\\VID_2FE3&PID_0008\\' -and $_.Status -eq 'OK'
})
if ($root.Count -ne 1 -or (Get-PnpDeviceProperty -InstanceId $root[0].PNPDeviceID -KeyName DEVPKEY_Device_BusReportedDeviceDesc).Data -ne 'DIY Mouse Config HID v21') {
    throw 'Unique v21 diagnostic identity not verified; no request sent.'
}
Add-Type -Path (Resolve-Path -LiteralPath $HidSharpPath).Path
$devices = @([HidSharp.DeviceList]::Local.GetHidDevices(0x2FE3,0x0008) |
    Where-Object { $_.GetMaxFeatureReportLength() -eq 65 })
if ($devices.Count -ne 1) { throw 'Expected one prototype Feature-report interface.' }
$stream = $devices[0].Open()
$result = [ordered]@{ Product='DIY Mouse Config HID v21'; Success=$false; SetCount=0; GetCount=0 }
$job = Join-Path "$PSScriptRoot\..\build-pairing-analysis" ('config-hid-v21-'+(Get-Date -Format yyyyMMdd-HHmmss)+'-'+[guid]::NewGuid().ToString('N').Substring(0,8)+'.json')
try {
    $stream.ReadTimeout = 2000; $stream.WriteTimeout = 2000
    $response = New-Object byte[] 65; $stream.GetFeature($response); $result.GetCount++
    if ($response[1] -ne 0xA3) { throw 'Expected unused read-only mailbox; reboot receiver before retry.' }
    $request = New-ReceiverVersionQuery
    $stream.SetFeature($request); $result.SetCount++
    for ($attempt=0; $attempt -lt 100; $attempt++) {
        Start-Sleep -Milliseconds 100
        [Array]::Clear($response,0,$response.Length)
        $stream.GetFeature($response); $result.GetCount++
        if ($response[1] -ne 0xA0) { break }
    }
    $result.Version = Read-ReceiverVersionReply $response
    $result.Success = $true
    Write-Output ('USB_RF_VERSION_READ_PASS: '+$result.Version)
} catch {
    $result.Error = $_.Exception.Message
    throw
} finally {
    if ($request) { $result.RequestHex = [BitConverter]::ToString($request) }
    if ($response) { $result.ResponseHex = [BitConverter]::ToString($response) }
    $result.FinishedUtc = [DateTime]::UtcNow.ToString('o')
    $result | ConvertTo-Json | Set-Content -Encoding UTF8 $job
    $stream.Dispose()
    Write-Output ('PRIVATE_RESULT='+$job)
}
