param([string]$HidSharpPath="$PSScriptRoot\..\build-pairing-analysis\HidSharp.dll",[switch]$Wireless,[switch]$SelfTest)
$ErrorActionPreference='Stop'
$mouseVendorId=if($Wireless){0x2FE3}else{0x258A}
$mouseProductId=if($Wireless){0x0008}else{0x2018}
$buttonIds=@(1,3,2,5,4,0x14,0x10,0x11)

function New-ProfileBackupRequests {
    $specs=@(
        @{Name='Version';Group=0;Operation=0x81;Size=3},
        @{Name='ActiveProfile';Group=0;Operation=0x85;Size=1},
        @{Name='Polling';Group=1;Operation=0x80;Size=1},
        @{Name='LiftOff';Group=1;Operation=0x88;Size=1}
    )
    for($profile=1;$profile -le 3;$profile++) {
        $specs+=@(
            @{Name='DpiStages';Group=1;Operation=0x81;Size=1;Profile=$profile},
            @{Name='SelectedDpi';Group=1;Operation=0x82;Size=1;Profile=$profile},
            @{Name='DpiColors';Group=2;Operation=0x81;Size=19;Profile=$profile},
            @{Name='Debounce';Group=0;Operation=0x88;Size=2;Profile=$profile}
        )
        foreach($button in $buttonIds) { $specs+=@{Name='Binding';Group=3;Operation=0x80;Size=10;Profile=$profile;Button=$button} }
    }
    foreach($spec in $specs) {
        $packet=New-Object byte[] 65
        $packet[3]=2;$packet[4]=$spec.Size;$packet[5]=$spec.Group;$packet[6]=$spec.Operation
        if($spec.Profile) {$packet[7]=$spec.Profile}
        if($spec.Button) {$packet[8]=$spec.Button} # Slot0 only; CORE startup writes this slot.
        [pscustomobject]@{Name=$spec.Name;Profile=$spec.Profile;Button=$spec.Button;Packet=$packet}
    }
}

function Assert-BackupResponse($Spec,[byte[]]$Packet) {
    if($Packet.Length -ne 65 -or $Packet[0] -ne 0 -or $Packet[1] -ne 0xA1 -or
       $Packet[3] -ne 2 -or $Packet[5] -ne $Spec.Packet[5] -or $Packet[6] -ne $Spec.Packet[6] -or $Packet[4] -gt 58) {
        throw 'Invalid/busy/mismatched configuration backup response.'
    }
    if($Spec.Profile -and $Packet[7] -ne $Spec.Profile) {throw 'Backup profile mismatch.'}
    if($Spec.Button -and ($Packet[8] -ne $Spec.Button -or $Packet[9] -ne 0)) {throw 'Backup button/slot mismatch.'}
    $expected=$Spec.Packet[4]
    if($Spec.Name -eq 'SelectedDpi') {
        $expected=2
        if($Packet[8] -lt 1 -or $Packet[8] -gt 6) {throw 'Invalid saved DPI selection.'}
    }
    if($Spec.Name -eq 'DpiStages') {
        if($Packet[8] -lt 1 -or $Packet[8] -gt 6) {throw 'Invalid saved DPI stage count.'}
        $expected=2+4*$Packet[8]
    }
    if($Packet[4] -ne $expected -or ($Spec.Name -eq 'Binding' -and $Packet[11] -gt 5)) {throw 'Unexpected saved-field length.'}
}

if($SelfTest) {
    $specs=@(New-ProfileBackupRequests)
    if($specs.Count -ne 40 -or @($specs|Where-Object Name -eq 'Binding').Count -ne 24) {throw 'Backup coverage incomplete.'}
    foreach($spec in $specs) {
        $packet=$spec.Packet
        if($packet.Length -ne 65 -or $packet[3] -ne 2 -or $packet[6] -lt 0x80 -or
           @($packet[9..64]|Where-Object {$_ -ne 0}).Count) {throw 'Backup contains a non-read request.'}
        $reply=$packet.Clone();$reply[1]=0xA1
        if($spec.Name -eq 'DpiStages') {$reply[8]=1;$reply[4]=6}
        if($spec.Name -eq 'SelectedDpi') {$reply[8]=1;$reply[4]=2}
        Assert-BackupResponse $spec $reply
        $reply[6]=$reply[6] -bxor 1;$rejected=$false
        try {Assert-BackupResponse $spec $reply}catch{$rejected=$true}
        if(!$rejected) {throw 'Wrong backup command accepted.'}
    }
    $reply=New-Object byte[] 10;$rejected=$false
    try {Assert-BackupResponse $specs[0] $reply}catch{$rejected=$true}
    if(!$rejected) {throw 'Truncated backup response accepted.'}
    'Forty read-only backup requests/coverage/response checks passed; no hardware accessed.'
    return
}
if([IntPtr]::Size -ne 4) {throw 'Run in 32-bit SysWOW64 PowerShell.'}
if(Get-Process -Name 'Glorious Core' -ErrorAction SilentlyContinue) {throw 'Close CORE first.'}
if($Wireless) {
    $root=@(Get-CimInstance Win32_PnPEntity|Where-Object {$_.PNPDeviceID -match '^USB\\VID_2FE3&PID_0008\\' -and $_.Status -eq 'OK'})
    if($root.Count -ne 1 -or (Get-PnpDeviceProperty -InstanceId $root[0].PNPDeviceID -KeyName DEVPKEY_Device_BusReportedDeviceDesc).Data -ne 'DIY Mouse Config HID v24') {
        throw 'Unique v24 receiver identity not verified; no requests sent.'
    }
}
Add-Type -Path (Resolve-Path -LiteralPath $HidSharpPath).Path
$devices=@([HidSharp.DeviceList]::Local.GetHidDevices($mouseVendorId,$mouseProductId)|Where-Object {$_.GetMaxFeatureReportLength() -eq 65})
if($devices.Count -ne 1) {throw 'Expected one matching configuration interface.'}
$private=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\build-pairing-analysis'))
$file=Join-Path $private ('mouse-profile-backup-'+(Get-Date -Format yyyyMMdd-HHmmss)+'-'+[guid]::NewGuid().ToString('N').Substring(0,8)+'.json')
$result=[ordered]@{Scope='Read-only startup-touched fields; all three profiles, eight slot0 bindings each; no macro bodies';Wireless=[bool]$Wireless;Success=$false;Records=@()}
$stream=$devices[0].Open()
try {
    $stream.ReadTimeout=2000;$stream.WriteTimeout=2000
    foreach($spec in @(New-ProfileBackupRequests)) {
        $result.CurrentQuery=$spec.Name;$result.CurrentProfile=$spec.Profile;$result.CurrentButton=$spec.Button
        $stream.SetFeature($spec.Packet)
        $reply=New-Object byte[] 65
        for($attempt=0;$attempt -lt 20;$attempt++) {
            Start-Sleep -Milliseconds 100
            [Array]::Clear($reply,0,$reply.Length);$stream.GetFeature($reply)
            if($reply[1] -ne 0xA0) {break}
        }
        $result.LastResponseHex=[BitConverter]::ToString($reply)
        Assert-BackupResponse $spec $reply
        $result.Records+=[pscustomobject]@{Name=$spec.Name;Profile=$spec.Profile;Button=$spec.Button
            RequestHex=[BitConverter]::ToString($spec.Packet);ResponseHex=[BitConverter]::ToString($reply)}
    }
    if($result.Records.Count -ne 40) {throw 'Incomplete profile backup.'}
    $result.Success=$true
    Write-Output 'PROFILE_BACKUP_PASS: 40 read-only queries, three profiles, 24 slot0 bindings'
}catch{$result.Error=$_.Exception.Message;throw}finally{
    $stream.Dispose();$result.FinishedUtc=[DateTime]::UtcNow.ToString('o')
    $result|ConvertTo-Json -Depth 6|Set-Content -Encoding UTF8 $file
    if($result.Success) {$file|Set-Content -Encoding UTF8 (Join-Path $private 'current-core-profile-backup.txt')}
    Write-Output ('PRIVATE_RESULT='+$file)
}
