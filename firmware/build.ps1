param([ValidateSet('hid', 'radio', 'probe', 'receiver', 'dongle', 'latency')][string]$App = 'hid')

$ErrorActionPreference = 'Stop'
$workspace = Join-Path $env:USERPROFILE 'zephyrproject'
$stage = Join-Path $workspace "mouse-$App-source"
$build = Join-Path $workspace "build-mouse-$App"
$source = if ($App -ne 'hid') { Join-Path $PSScriptRoot 'radio_logger' } else { $PSScriptRoot }
$env:VIRTUAL_ENV = Join-Path $workspace '.venv'
$env:ZEPHYR_BASE = Join-Path $workspace 'zephyr'
$env:ZEPHYR_TOOLCHAIN_VARIANT = 'zephyr'
$userPath = [Environment]::GetEnvironmentVariable('Path', 'User')
$env:Path = "$env:VIRTUAL_ENV\Scripts;C:\Program Files\CMake\bin;C:\Program Files\7-Zip;$userPath;$env:Path"
Get-Command python, west, cmake, ninja -ErrorAction Stop | Out-Null

python -m unittest discover -s $PSScriptRoot -p 'test_*.py'
if ($LASTEXITCODE -ne 0) { throw 'UF2 validator tests failed.' }

# Zephyr 4.2 Kconfig splits source paths containing spaces.
New-Item -ItemType Directory -Force -Path "$stage\src" | Out-Null
Copy-Item -LiteralPath "$source\CMakeLists.txt", "$source\prj.conf" -Destination $stage
Copy-Item -LiteralPath "$source\src\main.c" -Destination "$stage\src"
if ($App -ne 'hid') {
    Copy-Item -LiteralPath "$source\src\discovery_probe.h" -Destination "$stage\src"
    Copy-Item -LiteralPath "$source\src\probe_timing.h" -Destination "$stage\src"
    Copy-Item -LiteralPath "$source\src\wireless_hid.h" -Destination "$stage\src"
    Copy-Item -LiteralPath "$source\src\latency_metrics.h", "$source\src\latency_dump.h" -Destination "$stage\src"
    Copy-Item -LiteralPath "$source\receiver.conf" -Destination $stage
    Copy-Item -LiteralPath "$source\dongle.conf" -Destination $stage
    Copy-Item -LiteralPath "$source\latency.conf" -Destination $stage
    python "$PSScriptRoot\generate_radio_header.py" "$PSScriptRoot\..\build-pairing-analysis" "$stage\src\radio_parameters.h"
    if ($LASTEXITCODE -ne 0) { throw 'Radio evidence validation failed.' }
}

Push-Location $env:ZEPHYR_BASE
try {
    if ($App -eq 'hid') {
        west build -p auto -b promicro_nrf52840/nrf52840/uf2 -d $build $stage
    } else {
        $probe = if ($App -in @('probe', 'receiver', 'dongle', 'latency')) { 'ON' } else { 'OFF' }
        $wirelessHid = if ($App -in @('receiver', 'dongle', 'latency')) { 'ON' } else { 'OFF' }
        $normal = if ($App -in @('dongle', 'latency')) { 'ON' } else { 'OFF' }
        $timing = if ($App -eq 'latency') { 'ON' } else { 'OFF' }
        $extraConf = if ($App -eq 'latency') { 'receiver.conf;dongle.conf;latency.conf' } elseif ($App -eq 'dongle') { 'receiver.conf;dongle.conf' } elseif ($App -eq 'receiver') { 'receiver.conf' } else { '' }
        west build -p auto -b promicro_nrf52840/nrf52840/uf2 -d $build $stage -- "-DDISCOVERY_PROBE=$probe" "-DWIRELESS_HID=$wirelessHid" "-DNORMAL_DONGLE=$normal" "-DLATENCY_DIAGNOSTICS=$timing" "-DEXTRA_CONF_FILE=$extraConf"
    }
    if ($LASTEXITCODE -ne 0) { throw 'Firmware build failed; do not flash an old UF2.' }
    python "$PSScriptRoot\check_uf2.py" "$build\zephyr\zephyr.uf2"
    if ($LASTEXITCODE -ne 0) { throw 'UF2 validation failed; do not flash.' }
    Write-Host "Validated firmware: $build\zephyr\zephyr.uf2"
} finally {
    Pop-Location
}
