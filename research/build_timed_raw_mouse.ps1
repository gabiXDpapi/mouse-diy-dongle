$ErrorActionPreference='Stop'
$out=Join-Path (Split-Path $PSScriptRoot -Parent) 'build-pairing-analysis'
if(!(Test-Path -LiteralPath $out)){throw 'Private output directory missing'}
$exe=Join-Path $out 'capture_raw_mouse_timed.exe'
& gcc -std=gnu11 -Wall -Wextra -Werror -O2 (Join-Path $PSScriptRoot 'capture_raw_mouse.c') -o $exe -luser32 -lshell32
if($LASTEXITCODE -ne 0){throw 'Timed raw logger build failed'}
& $exe --self-test
if($LASTEXITCODE -ne 0){throw 'Timed raw logger self-test failed; do not capture'}
@{source_sha256=(Get-FileHash -LiteralPath (Join-Path $PSScriptRoot 'capture_raw_mouse.c') -Algorithm SHA256).Hash;header_sha256=(Get-FileHash -LiteralPath (Join-Path $PSScriptRoot 'motion_epoch.h') -Algorithm SHA256).Hash;binary_sha256=(Get-FileHash -LiteralPath $exe -Algorithm SHA256).Hash;self_test_pass=$true} | ConvertTo-Json | Set-Content (Join-Path $out 'timed-raw-software-validation.json')
Write-Output ('BUILT_AND_SELF_TESTED='+$exe)
