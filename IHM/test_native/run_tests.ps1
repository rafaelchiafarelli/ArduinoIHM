<#
.SYNOPSIS
Builds and runs the host-native unit test suite for the hardware-independent
parts of this firmware (state machines, pure decoding/mapping logic, and any
code refactored behind the RegisterIO HAL).

Why this exists instead of `pio test`: PlatformIO's "native" test platform
needs a host gcc, and none is installed on this machine. MSVC (from Visual
Studio 2022 Build Tools/Community) is available, so we drive cl.exe directly.
Nothing here touches the real AVR build (platformio.ini / megaatmega2560 env)
-- this is a separate, host-only compile of test_native/*.cpp plus whichever
production headers/sources those tests #include.

Usage: pwsh -File test_native/run_tests.ps1
#>

# Native command stderr output (e.g. compiler diagnostics) must not become a
# terminating PowerShell error here -- we gate on exit codes explicitly below.
$ErrorActionPreference = "Continue"
$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$repoRoot = Split-Path -Parent $root

# --- Locate and load the MSVC x64 developer environment ---------------------
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
    # vcvarsall.bat itself sometimes emits a harmless "vswhere.exe not
    # recognized" line on stderr (an internal version-probe that fails
    # silently in real cmd.exe too) while still succeeding and printing the
    # env dump on stdout. Don't let ErrorActionPreference=Stop turn that
    # stderr noise into a terminating error -- check $LASTEXITCODE instead.
    $prevEap = $ErrorActionPreference
    $ErrorActionPreference = "Continue"
    $vcvarsallEsc = $vcvarsall
    $envDump = cmd.exe /c "`"$vcvarsallEsc`" x64 >nul 2>nul && set"
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

# --- Include dirs: test_native itself, plus an explicit allowlist of lib
# dirs that hold hardware-independent logic. Deliberately NOT auto-globbing
# all of lib/*/(src): several libs (ArduinoLib in particular) ship Arduino
# compatibility shims -- e.g. lib/ArduinoLib/src/new, a header with no
# extension -- that shadow real standard-library headers (<new>, ...) when
# blindly added to the include path, and most of this codebase touches AVR
# registers directly so it can't compile for a host target at all. Each
# feature branch adds its own natively-testable dir(s) here as needed.
$libAllowlist = @(
    "lib/StateMachine/src",
    "lib/HAL/src",
    "lib/MultiOutput/src",
    "lib/RotaryEncoder"
)
$includeDirs = @($root) + ($libAllowlist | ForEach-Object { Join-Path $repoRoot $_ })
foreach ($dir in $includeDirs) {
    if (-not (Test-Path $dir)) {
        Write-Error "Configured include dir does not exist: $dir"
        exit 1
    }
}

# --- Production .cpp files that are hardware-independent (no avr/*.h) and
# so can be compiled and linked into the native test binary directly. Also
# an explicit allowlist, for the same reason as $libAllowlist above -- most
# production .cpp files in this repo touch AVR registers directly and simply
# will not compile for a host target.
$prodSourceAllowlist = @(
    "lib/MultiOutput/src/PWMTiming.cpp",
    "lib/MultiOutput/src/PWMConfig.cpp",
    "lib/MultiOutput/src/PWMChannelConfig.cpp",
    "lib/MultiOutput/src/PWMLabelFormat.cpp",
    "lib/RotaryEncoder/ButtonMap.cpp"
)
$prodSources = $prodSourceAllowlist | ForEach-Object { Join-Path $repoRoot $_ }
foreach ($src in $prodSources) {
    if (-not (Test-Path $src)) {
        Write-Error "Configured production source does not exist: $src"
        exit 1
    }
}
$includeFlags = $includeDirs | ForEach-Object { "/I`"$_`"" }

# --- Gather sources: main.cpp + every test_*.cpp + the allowlisted prod .cpp's
$sources = @(Join-Path $root "main.cpp") + $prodSources
$sources += Get-ChildItem -Path $root -Filter "test_*.cpp" | Select-Object -ExpandProperty FullName
if ($sources.Count -le (1 + $prodSources.Count)) {
    Write-Warning "No test_*.cpp files found in $root -- nothing to run."
}

$outDir = Join-Path $root "_build"
New-Item -ItemType Directory -Force -Path $outDir | Out-Null
$exePath = Join-Path $outDir "native_tests.exe"

$sourceArgs = $sources | ForEach-Object { "`"$_`"" }
$clArgs = @("/nologo", "/EHsc", "/std:c++17", "/W4", "/wd4996", "/Fe:`"$exePath`"", "/Fo:`"$outDir\\`"") + $includeFlags + $sourceArgs

Write-Host "Compiling $($sources.Count) source file(s)..."
$compileCmd = "cl.exe " + ($clArgs -join " ")
cmd.exe /c $compileCmd
if ($LASTEXITCODE -ne 0) {
    Write-Error "Compilation failed."
    exit 1
}

Write-Host ""
Write-Host "Running native_tests.exe ..."
Write-Host "----------------------------------------"
& $exePath
$testExit = $LASTEXITCODE
Write-Host "----------------------------------------"
exit $testExit
