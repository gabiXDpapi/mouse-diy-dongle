# Shared fixed mouse configuration packets; importing this file never accesses hardware.
function New-SettingsQuery([ValidateSet('Version','Profile','Polling','DpiStages','ActiveDpi','SelectDpi')][string]$Kind, [int]$Stage=0) {
    $packet = New-Object byte[] 65
    $packet[3] = 2
    $packet[4] = 1
    $packet[5] = 1
    $packet[7] = 1 # Profile 1 only; queries never select/change a profile.
    switch ($Kind) {
        Version { $packet[4] = 3; $packet[5] = 0; $packet[6] = 0x81; $packet[7] = 0 }
        Profile { $packet[5] = 0; $packet[6] = 0x85; $packet[7] = 0 }
        Polling { $packet[6] = 0x80; $packet[7] = 0 }
        DpiStages { $packet[6] = 0x81 }
        ActiveDpi { $packet[6] = 0x82 }
        SelectDpi {
            if ($Stage -lt 1 -or $Stage -gt 4) { throw 'Only the four known profile-1 stages are supported.' }
            $packet[4] = 2; $packet[6] = 2; $packet[8] = $Stage
        }
    }
    return ,$packet
}

function Read-SettingsReply([string]$Kind, [byte[]]$Packet, [int]$Stage=0) {
    $request = New-SettingsQuery $Kind $Stage
    if ($Packet.Length -ne 65 -or $Packet[0] -ne 0 -or $Packet[1] -ne 0xA1 -or $Packet[3] -ne 2 -or
        $Packet[5] -ne $request[5] -or $Packet[6] -ne $request[6]) {
        throw 'Invalid, busy or mismatched settings reply; no settings accepted.'
    }
    switch ($Kind) {
        Version { return ($Packet[7..10] -join '.') }
        Profile {
            if ($Packet[4] -ne 1 -or $Packet[7] -lt 1 -or $Packet[7] -gt 3) { throw 'Invalid active profile.' }
            return [int]$Packet[7]
        }
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
        SelectDpi {
            if ($Packet[4] -ne 2 -or $Packet[7] -ne 1 -or $Packet[8] -ne $Stage) { throw 'Selected-stage acknowledgement mismatch.' }
            return $Stage
        }
    }
}

