param([string]$HidSharpPath="$PSScriptRoot\..\build-pairing-analysis\HidSharp.dll",[switch]$SelfTest)
$ErrorActionPreference='Stop'
if([IntPtr]::Size -ne 4){throw 'Run in 32-bit SysWOW64 PowerShell.'}
Add-Type -Path (Resolve-Path -LiteralPath $HidSharpPath).Path

function Read-HidLayout($Descriptor) {
    [pscustomobject]@{
        ReportsUseID=$Descriptor.ReportsUseID
        Reports=@($Descriptor.Reports | ForEach-Object {
            $report=$_; $offset=0
            $fields=@(foreach($item in $report.DataItems){
                [pscustomobject]@{PayloadBitOffset=$offset;ElementBits=$item.ElementBits;ElementCount=$item.ElementCount;
                    Flags=[string]$item.Flags;LogicalMinimum=$item.LogicalMinimum;LogicalMaximum=$item.LogicalMaximum;
                    UsageCount=$item.Usages.Count;Usages=@($item.Usages.GetAllValues() | Select-Object -First 16 | ForEach-Object {'{0:X8}' -f $_})}
                $offset+=$item.TotalBits
            })
            [pscustomobject]@{Type=[string]$report.ReportType;Id=[int]$report.ReportID;Length=$report.Length;Fields=$fields}
        })
        Collections=@($Descriptor.DeviceItems | ForEach-Object {
            [pscustomobject]@{Usages=@($_.Usages.GetAllValues() | ForEach-Object {'{0:X8}' -f $_})}
        })
    }
}

if($SelfTest){
    # Synthetic keyboard report; this checks the parser, not the real mouse mapping.
    [byte[]]$raw=0x05,1,0x09,6,0xa1,1,0x85,3,0x05,7,0x19,0,0x29,0x65,0x15,0,0x25,0x65,0x75,8,0x95,8,0x81,0,0xc0
    $layout=Read-HidLayout ([HidSharp.Reports.ReportDescriptor]::new($raw))
    if(!$layout.ReportsUseID -or $layout.Reports.Count -ne 1 -or $layout.Reports[0].Id -ne 3 -or
       $layout.Reports[0].Length -ne 9 -or $layout.Reports[0].Type -ne 'Input' -or
       $layout.Collections[0].Usages -notcontains '00010006' -or $layout.Reports[0].Fields[0].PayloadBitOffset -ne 0 -or
       $layout.Reports[0].Fields[0].ElementBits -ne 8 -or $layout.Reports[0].Fields[0].ElementCount -ne 8){throw 'Descriptor summary self-test failed.'}
    'HID layout summary self-test passed; no hardware accessed.'
    return
}
if(Get-Process -Name 'Glorious Core' -ErrorAction SilentlyContinue){throw 'Close CORE first.'}
$roots=@(Get-CimInstance Win32_PnPEntity | Where-Object {$_.PNPDeviceID -match '^USB\\VID_258A&PID_2018\\' -and $_.ConfigManagerErrorCode -eq 0})
if($roots.Count -ne 1){throw 'Expected one healthy wired Glorious mouse; no interfaces inspected.'}
$devices=@([HidSharp.DeviceList]::Local.GetHidDevices(0x258A,0x2018))
if($devices.Count -lt 3 -or $devices.Count -gt 16){throw 'Unexpected wired HID collection count.'}
$result=[ordered]@{Scope='HidSharp Windows descriptor view, not raw bus capture; read-only, no streams or Feature requests';Success=$false;Collections=@()}
$private=[IO.Path]::GetFullPath((Join-Path $PSScriptRoot '..\build-pairing-analysis'))
$file=Join-Path $private ('wired-hid-layout-'+(Get-Date -Format yyyyMMdd-HHmmss)+'-'+[guid]::NewGuid().ToString('N').Substring(0,8)+'.json')
try {
    foreach($dev in $devices){
        $record=[ordered]@{InputLength=$dev.GetMaxInputReportLength();OutputLength=$dev.GetMaxOutputReportLength();FeatureLength=$dev.GetMaxFeatureReportLength()}
        try {
            $record.Layout=Read-HidLayout ($dev.GetReportDescriptor())
        } catch {$record.Error=$_.Exception.Message}
        $result.Collections += [pscustomobject]$record
    }
    if(@($result.Collections | Where-Object Error).Count){throw 'Some Windows HID layouts could not be read; see private partial evidence.'}
    $result.Success=$true
    'WIRED_HID_LAYOUT_PASS: '+$devices.Count+' collections; no settings requests sent.'
} catch {$result.Error=$_.Exception.Message;throw}
finally {
    $result.FinishedUtc=[DateTime]::UtcNow.ToString('o')
    $result | ConvertTo-Json -Depth 10 | Set-Content -Encoding UTF8 $file
    'PRIVATE_RESULT='+$file
}
