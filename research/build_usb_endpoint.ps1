$ErrorActionPreference='Stop'
$kit=Join-Path ${env:ProgramFiles(x86)} 'Windows Kits/10'
$vswhere=Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio/Installer/vswhere.exe'
$install=& $vswhere -latest -products '*' -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
if($LASTEXITCODE -ne 0 -or !$install){throw 'Installed MSVC toolchain required'}
$msvc=Get-ChildItem -LiteralPath (Join-Path $install 'VC/Tools/MSVC') -Directory | Sort-Object {[version]$_.Name} -Descending | Select-Object -First 1
$sdk=Get-ChildItem -LiteralPath (Join-Path $kit 'Include') -Directory | Where-Object {Test-Path (Join-Path $_.FullName 'um/Windows.h')} | Sort-Object {[version]$_.Name} -Descending | Select-Object -First 1
if(!$msvc -or !$sdk){throw 'Installed Windows SDK/MSVC headers required'}
$out=Join-Path (Split-Path $PSScriptRoot -Parent) 'build-pairing-analysis'
if(!(Test-Path -LiteralPath $out)){throw 'Private output directory missing'}
$exe=Join-Path $out 'inspect_usb_endpoint.exe'
$argsForCompiler=@('/nologo','/W4','/WX','/O2','/std:c11',('/I'+(Join-Path $msvc.FullName 'include')))
foreach($part in @('ucrt','shared','um')){$argsForCompiler+=('/I'+(Join-Path $sdk.FullName $part))}
$argsForCompiler+=@((Join-Path $PSScriptRoot 'inspect_usb_endpoint.c'),('/Fe'+$exe),('/Fo'+(Join-Path $out 'inspect_usb_endpoint.obj')),'/link',('/LIBPATH:'+(Join-Path $msvc.FullName 'lib/x64')))
foreach($part in @('ucrt','um')){$argsForCompiler+=('/LIBPATH:'+(Join-Path $kit ('Lib/'+$sdk.Name+'/'+$part+'/x64')))}
$argsForCompiler+=@('setupapi.lib','kernel32.lib')
& (Join-Path $msvc.FullName 'bin/Hostx64/x64/cl.exe') @argsForCompiler
if($LASTEXITCODE -ne 0){throw 'USB inspector build failed'}
& $exe --self-test
if($LASTEXITCODE -ne 0){throw 'USB inspector self-test failed; no hardware run'}
Write-Output ('BUILT_AND_SELF_TESTED='+$exe)
