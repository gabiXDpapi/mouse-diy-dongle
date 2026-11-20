param(
    [switch]$ExerciseDpi,
    [switch]$RestoreWired,
    [string]$BackupFile,
    [string]$HidSharpPath = "$PSScriptRoot\..\build-pairing-analysis\HidSharp.dll",
    [switch]$SelfTest
)
$ErrorActionPreference = 'Stop'
$mouseVendorId = if ($RestoreWired) { 0x258A } else { 0x2FE3 }
$mouseProductId = if ($RestoreWired) { 0x2018 } else { 0x0008 }
. "$PSScriptRoot\mouse_settings_protocol.ps1"

function Invoke-SettingsTransaction($Stream, [string]$Kind, [int]$Stage=0) {
    $request = New-SettingsQuery $Kind $Stage
    $response = New-Object byte[] 65
    $script:lastSettingsExchange = [ordered]@{ Kind=$Kind; Stage=$Stage; RequestHex=[BitConverter]::ToString($request); Gets=0 }
    $Stream.SetFeature($request)
    for ($attempt=0; $attempt -lt 60; $attempt++) {
        Start-Sleep -Milliseconds 100
        [Array]::Clear($response,0,$response.Length)
        $Stream.GetFeature($response)
        $script:lastSettingsExchange.Gets++
        $script:lastSettingsExchange.ResponseHex = [BitConverter]::ToString($response)
        if ($response[1] -ne 0xA0) { break }
    }
    $value = Read-SettingsReply $Kind $response $Stage
    [pscustomobject]@{ Kind=$Kind; Value=$value; RequestHex=[BitConverter]::ToString($request)
        ResponseHex=[BitConverter]::ToString($response); Gets=$script:lastSettingsExchange.Gets }
}

function Assert-DpiBackup($Backup) {
    $original=[int]$Backup.OriginalStage
    if ($Backup.Schema -ne 1 -or $Backup.Initial.Version -ne '1.0.0.0' -or
        $Backup.Initial.Profile -ne 1 -or $Backup.OriginalStage -ne $original -or $Backup.OriginalStage -lt 1 -or
        $Backup.OriginalStage -gt 4 -or $Backup.Initial.ActiveDpi -ne $Backup.OriginalStage -or
        @($Backup.Initial.DpiStages).Count -gt 4 -or
        @($Backup.Initial.DpiStages).Count -lt $Backup.OriginalStage) {
        throw 'Invalid profile-1 selected-DPI backup; no restore command allowed.'
    }
}

function Invoke-DpiSelectionTrial([scriptblock]$Select, [scriptblock]$Read, [int]$Original, [int]$Alternate, $Outcome) {
    try {
        & $Select $Alternate | Out-Null
        if ((& $Read) -ne $Alternate) { throw 'Changed selection readback mismatch.' }
        $Outcome.Changed=$true
    } catch {
        $Outcome.ChangeError=$_.Exception.Message
        $Outcome.ChangeExchange=$script:lastSettingsExchange
        throw
    } finally {
        & $Select $Original | Out-Null
        if ((& $Read) -ne $Original) { throw 'Radio restore readback mismatch; use saved wired recovery.' }
        $Outcome.Restored=$true
    }
}

if ($SelfTest) {
    $reply = New-SettingsQuery Version; $reply[1]=0xA1; $reply[7]=1
    $busy = $reply.Clone(); $busy[1]=0xA0
    $fake = [pscustomobject]@{ Sets=0; Gets=0; Replies=[Collections.Generic.Queue[byte[]]]::new() }
    $fake | Add-Member ScriptMethod SetFeature { param([byte[]]$Packet) $this.Sets++ }
    $fake | Add-Member ScriptMethod GetFeature { param([byte[]]$Packet) $this.Gets++; [Array]::Copy($this.Replies.Dequeue(),$Packet,65) }
    $fake.Replies.Enqueue($busy); $fake.Replies.Enqueue($reply)
    $r = Invoke-SettingsTransaction $fake Version
    if ($r.Value -ne '1.0.0.0' -or $fake.Sets -ne 1 -or $fake.Gets -ne 2) { throw 'Busy polling must not resend SET.' }
    $failed=$reply.Clone(); $failed[1]=0xA2; $fake.Replies.Enqueue($failed)
    $refused=$false
    try { Invoke-SettingsTransaction $fake Version | Out-Null } catch { $refused=$true }
    if (!$refused -or $fake.Sets -ne 2 -or $fake.Gets -ne 3) { throw 'Failed transaction accepted/retried.' }
    $select=New-SettingsQuery SelectDpi 3; $select[1]=0xA1
    if ((Read-SettingsReply SelectDpi $select 3) -ne 3) { throw 'Selection acknowledgement failed.' }
    foreach ($stage in @(0,5)) {
        $refused=$false; try { New-SettingsQuery SelectDpi $stage | Out-Null } catch { $refused=$true }
        if (!$refused) { throw 'Invalid selected stage accepted.' }
    }
    $select[8]=2; $refused=$false
    try { Read-SettingsReply SelectDpi $select 3 | Out-Null } catch { $refused=$true }
    if (!$refused) { throw 'Wrong selection acknowledgement accepted.' }
    $profile=New-SettingsQuery Profile; $profile[1]=0xA1; $profile[7]=1
    if ((Read-SettingsReply Profile $profile) -ne 1) { throw 'Profile parse failed.' }
    $backup=[pscustomobject]@{Schema=1;OriginalStage=2;Initial=[pscustomobject]@{Version='1.0.0.0';Profile=1;ActiveDpi=2;DpiStages=@(400,800,1600,3200)}}
    Assert-DpiBackup $backup; $backup.OriginalStage=5; $refused=$false
    try { Assert-DpiBackup $backup } catch { $refused=$true }
    if (!$refused) { throw 'Unsafe backup accepted.' }
    foreach ($fault in @('none','read','set','restore')) {
        $script:mockTrial=[pscustomobject]@{Stage=2;Fault=$fault;History=[Collections.Generic.List[int]]::new()}
        $selectAction={param($stage)
            $script:mockTrial.History.Add($stage)
            if ($script:mockTrial.Fault -eq 'restore' -and $stage -eq 2) { throw 'Restore transport failed.' }
            $script:mockTrial.Stage=$stage
            if ($script:mockTrial.Fault -eq 'set' -and $stage -eq 3) { throw 'Ambiguous change write.' }
        }
        $readAction={
            if ($script:mockTrial.Fault -eq 'read' -and $script:mockTrial.Stage -eq 3) { return 4 }
            return $script:mockTrial.Stage
        }
        $outcome=[ordered]@{Changed=$false;Restored=$false}; $threw=$false
        try { Invoke-DpiSelectionTrial $selectAction $readAction 2 3 $outcome } catch { $threw=$true }
        if (($script:mockTrial.History -join ',') -ne '3,2') { throw 'Restore must be attempted after every change attempt.' }
        if ($fault -eq 'none') {
            if ($threw -or !$outcome.Changed -or !$outcome.Restored) { throw 'Happy-path selection trial failed.' }
        } elseif (!$threw) { throw 'Fault was reported as success.' }
        if ($fault -eq 'restore') {
            if ($outcome.Restored -or $script:mockTrial.Stage -ne 3) { throw 'Failed restore must stay pending.' }
        } elseif (!$outcome.Restored -or $script:mockTrial.Stage -ne 2) { throw 'Original selection not restored after change fault.' }
    }
    'Reusable transaction, selection, backup and failure checks passed; no hardware accessed.'
    return
}

if ($ExerciseDpi -and $RestoreWired) { throw 'Choose a receiver trial OR wired recovery.' }
if ([IntPtr]::Size -ne 4) { throw 'Run in 32-bit SysWOW64 PowerShell.' }
if (Get-Process -Name 'Glorious Core' -ErrorAction SilentlyContinue) { throw 'Close CORE first.' }
$private = [IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\build-pairing-analysis'))
$pointer = Join-Path $private 'current-config-hid-backup.txt'
$savedBackup=$null
if ($RestoreWired) {
    if (!$BackupFile) { $BackupFile=Get-Content -LiteralPath $pointer -Raw }
    $BackupFile=[IO.Path]::GetFullPath($BackupFile.Trim())
    if (!$BackupFile.StartsWith($private+[IO.Path]::DirectorySeparatorChar,[StringComparison]::OrdinalIgnoreCase)) { throw 'Backup must be in private project analysis directory.' }
    $savedBackup=Get-Content -LiteralPath $BackupFile -Raw | ConvertFrom-Json
    Assert-DpiBackup $savedBackup
    if (!$savedBackup.NeedsWiredRestore) { throw 'Backup is already restored; no replay allowed.' }
} else {
    if ($ExerciseDpi -and (Test-Path -LiteralPath $pointer)) {
        $priorPath=[IO.Path]::GetFullPath((Get-Content -LiteralPath $pointer -Raw).Trim())
        if (!$priorPath.StartsWith($private+[IO.Path]::DirectorySeparatorChar,[StringComparison]::OrdinalIgnoreCase)) { throw 'Invalid prior backup location.' }
        $prior=Get-Content -LiteralPath $priorPath -Raw | ConvertFrom-Json
        Assert-DpiBackup $prior
        if ($prior.NeedsWiredRestore) { throw 'Previous selection recovery is pending; resolve it before another write.' }
    }
    $root=@(Get-CimInstance Win32_PnPEntity | Where-Object { $_.PNPDeviceID -match '^USB\\VID_2FE3&PID_0008\\' -and $_.Status -eq 'OK' })
    if ($root.Count -ne 1 -or (Get-PnpDeviceProperty -InstanceId $root[0].PNPDeviceID -KeyName DEVPKEY_Device_BusReportedDeviceDesc).Data -ne 'DIY Mouse Config HID v23') {
        throw 'Unique v23 receiver identity not verified; no request sent.'
    }
}
Add-Type -Path (Resolve-Path -LiteralPath $HidSharpPath).Path
$devices=@([HidSharp.DeviceList]::Local.GetHidDevices($mouseVendorId,$mouseProductId) | Where-Object { $_.GetMaxFeatureReportLength() -eq 65 })
if ($devices.Count -ne 1) { throw 'Expected one matching configuration Feature interface.' }
$stream=$devices[0].Open()
$job=Join-Path $private ('config-v23-'+(Get-Date -Format yyyyMMdd-HHmmss)+'-'+[guid]::NewGuid().ToString('N').Substring(0,8))
$result=[ordered]@{Success=$false;WiredRecovery=[bool]$RestoreWired;ExerciseDpi=[bool]$ExerciseDpi;NeedsWiredRestore=[bool]$RestoreWired;Transactions=@()}
function Invoke-Checked([string]$Kind,[int]$Stage=0) {
    $r=Invoke-SettingsTransaction $stream $Kind $Stage
    $result.Transactions+= $r
    return $r.Value
}
try {
    $stream.ReadTimeout=2000; $stream.WriteTimeout=2000
    if ($RestoreWired) {
        if ((Invoke-Checked Version) -ne $savedBackup.Initial.Version) { throw 'Wired mouse firmware does not match backup.' }
        Invoke-Checked SelectDpi ([int]$savedBackup.OriginalStage) | Out-Null
        if ((Invoke-Checked ActiveDpi) -ne $savedBackup.OriginalStage) { throw 'Wired selected-stage restore not verified.' }
        $savedBackup.NeedsWiredRestore=$false
        $savedBackup | ConvertTo-Json -Depth 6 | Set-Content -Encoding UTF8 $BackupFile
        $result.NeedsWiredRestore=$false
        $result.Success=$true
        Write-Output 'WIRED_SELECTED_DPI_RESTORE_VERIFIED'
    } else {
        $initial=[ordered]@{Version=Invoke-Checked Version;Profile=Invoke-Checked Profile;Polling=Invoke-Checked Polling
            DpiStages=Invoke-Checked DpiStages;ActiveDpi=Invoke-Checked ActiveDpi}
        $result.Initial=$initial
        if ($initial.Version -ne '1.0.0.0' -or $initial.Profile -ne 1 -or
            @($initial.DpiStages).Count -gt 4 -or $initial.ActiveDpi -gt @($initial.DpiStages).Count) {
            throw 'Unexpected mouse firmware/profile/stages; no selection write allowed.'
        }
        if ($ExerciseDpi) {
            if (@($initial.DpiStages).Count -lt 2) { throw 'Need two existing DPI stages for reversible trial.' }
            $backup=[ordered]@{Schema=1;OriginalStage=$initial.ActiveDpi;Initial=$initial;NeedsWiredRestore=$true}
            Assert-DpiBackup ([pscustomobject]$backup)
            $BackupFile="$job.backup.json"
            $backup | ConvertTo-Json -Depth 6 | Set-Content -Encoding UTF8 $BackupFile
            $BackupFile | Set-Content -Encoding UTF8 $pointer
            $result.BackupFile=$BackupFile; $result.NeedsWiredRestore=$true
            $alternate=($initial.ActiveDpi % @($initial.DpiStages).Count)+1
            $outcome=[ordered]@{Changed=$false;Restored=$false}
            try {
                Invoke-DpiSelectionTrial {param($stage) Invoke-Checked SelectDpi $stage} {Invoke-Checked ActiveDpi} ([int]$initial.ActiveDpi) $alternate $outcome
                $result.ChangedStage=$alternate
            } finally {
                $result.DpiTrial=$outcome
                if ($outcome.Restored) {
                    $backup.NeedsWiredRestore=$false
                    $backup | ConvertTo-Json -Depth 6 | Set-Content -Encoding UTF8 $BackupFile
                    $result.NeedsWiredRestore=$false; $result.RestoredStage=$initial.ActiveDpi
                }
            }
        }
        $result.Success=$true
        Write-Output ('CONFIG_READBACK_PASS: profile='+$initial.Profile+' polling='+$initial.Polling+'Hz selected_stage='+$initial.ActiveDpi)
        if ($ExerciseDpi) { Write-Output 'SELECTED_DPI_CHANGE_AND_RESTORE_VERIFIED' }
    }
} catch {
    $result.Error=$_.Exception.Message
    $result.LastExchange=$script:lastSettingsExchange
    throw
} finally {
    $stream.Dispose()
    $result.FinishedUtc=[DateTime]::UtcNow.ToString('o')
    $result | ConvertTo-Json -Depth 8 | Set-Content -Encoding UTF8 "$job.json"
    Write-Output ('PRIVATE_RESULT='+"$job.json")
    if ($result.NeedsWiredRestore) { Write-Output ('WIRED_RECOVERY_REQUIRED='+$BackupFile) }
}
