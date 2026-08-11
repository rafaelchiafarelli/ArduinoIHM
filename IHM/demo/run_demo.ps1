<#
.SYNOPSIS
Builds and runs the host-side PWM walkthrough (demo_pwm_walkthrough.cpp),
printing a readable log of every channel/frequency/edge combination and the
exact register bytes the real production code computes for it.

Same host-MSVC approach as test_native/run_tests.ps1 (see that script's
header comment for why) -- this is a separate driver because a demo isn't a
pass/fail suite the way test_native/ is, and doesn't belong mixed into that
harness's output.

Usage: powershell -File demo/run_demo.ps1
#>

$ErrorActionPreference = "Continue"
$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = Split-Path -Parent $root

$vswhere = "${env:ProgramFiles(x86)}\Microsoft Visual Studio\Installer\vswhere.exe"
$vcvarsall = $null
if (Test-Path $vswhere) {
    $vsPath = & $vswhere -latest -products * -requires Microsoft.VisualStudio.Component.VC.Tools.x86.x64 -property installationPath
    if ($vsPath) { $vcvarsall = Join-Path $vsPath "VC\Auxiliary\Build\vcvarsall.bat" }
}
if (-not $vcvarsall -or -not (Test-Path $vcvarsall)) {
    $fallback = "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvarsall.bat"
    if (Test-Path $fallback) { $vcvarsall = $fallback }
}
if (-not $vcvarsall -or -not (Test-Path $vcvarsall)) {
    Write-Error "Could not find vcvarsall.bat. Install the 'Desktop development with C++' workload for Visual Studio, or edit this script to point at your compiler."
    exit 1
}

if (-not (Get-Command cl.exe -ErrorAction SilentlyContinue)) {
    $prevEap = $ErrorActionPreference
    $ErrorActionPreference = "Continue"
    $envDump = cmd.exe /c "`"$vcvarsall`" x64 >nul 2>nul && set"
    $vcvarsExit = $LASTEXITCODE
    $ErrorActionPreference = $prevEap
    if ($vcvarsExit -ne 0) {
        Write-Error "vcvarsall.bat x64 failed (exit $vcvarsExit)."
        exit 1
    }
    foreach ($line in $envDump) {
        if ($line -match '^([^=]+)=(.*)$') {
            [System.Environment]::SetEnvironmentVariable($Matches[1], $Matches[2], 'Process')
        }
    }
}

if (-not (Get-Command cl.exe -ErrorAction SilentlyContinue)) {
    Write-Error "cl.exe still not on PATH after loading vcvarsall.bat -- aborting."
    exit 1
}

$includeDirs = @(
    (Join-Path $repoRoot "lib\HAL\src"),
    (Join-Path $repoRoot "lib\MultiOutput\src")
)
foreach ($dir in $includeDirs) {
    if (-not (Test-Path $dir)) {
        Write-Error "Configured include dir does not exist: $dir"
        exit 1
    }
}
$includeFlags = $includeDirs | ForEach-Object { "/I`"$_`"" }

$sources = @(
    (Join-Path $root "demo_pwm_walkthrough.cpp"),
    (Join-Path $repoRoot "lib\MultiOutput\src\PWMTiming.cpp"),
    (Join-Path $repoRoot "lib\MultiOutput\src\PWMConfig.cpp"),
    (Join-Path $repoRoot "lib\MultiOutput\src\PWMChannelConfig.cpp"),
    (Join-Path $repoRoot "lib\MultiOutput\src\PWMLabelFormat.cpp")
)
$sourceArgs = $sources | ForEach-Object { "`"$_`"" }

$outDir = Join-Path $root "_build"
New-Item -ItemType Directory -Force -Path $outDir | Out-Null
$exePath = Join-Path $outDir "pwm_walkthrough.exe"

$clArgs = @("/nologo", "/EHsc", "/std:c++17", "/W4", "/wd4996", "/Fe:`"$exePath`"", "/Fo:`"$outDir\\`"") + $includeFlags + $sourceArgs
$compileCmd = "cl.exe " + ($clArgs -join " ")
cmd.exe /c $compileCmd
if ($LASTEXITCODE -ne 0) {
    Write-Error "Compilation failed."
    exit 1
}

Write-Host ""
& $exePath
exit $LASTEXITCODE
