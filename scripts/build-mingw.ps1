param(
    [string]$BuildType = "Debug",
    [switch]$ConfigureOnly
)

$ErrorActionPreference = "Stop"

$repoRoot = Split-Path -Parent $PSScriptRoot
$cmakeExe = "D:/Tools/QT/Tools/CMake_64/bin/cmake.exe"
$ninjaExe = "D:/Tools/QT/Tools/Ninja/ninja.exe"
$gppExe = "D:/Tools/QT/Tools/mingw1310_64/bin/g++.exe"
$qtRoot = "D:/Tools/QT/6.11.1/mingw_64"
$buildDir = Join-Path $repoRoot "build-mingw"
$cacheFile = Join-Path $buildDir "CMakeCache.txt"

# Ensure MinGW, Ninja and Qt runtime are on PATH for compiler subprocesses.
$env:PATH = "D:/Tools/QT/Tools/mingw1310_64/bin;D:/Tools/QT/Tools/Ninja;D:/Tools/QT/6.11.1/mingw_64/bin;" + $env:PATH

if (Test-Path $cacheFile) {
    $cacheContent = Get-Content -Raw $cacheFile
    if ($cacheContent -match [regex]::Escape('CMAKE_MAKE_PROGRAM:') -and $cacheContent -match [regex]::Escape('$ninjaExe')) {
        Remove-Item -Recurse -Force (Join-Path $buildDir "CMakeFiles") -ErrorAction SilentlyContinue
        Remove-Item -Force $cacheFile -ErrorAction SilentlyContinue
    }
}

$cmakeArgs = @(
    "-S", "$repoRoot/main/NZFTC_EMS/UI/src",
    "-B", "$buildDir",
    "-G", "Ninja",
    "-DCMAKE_MAKE_PROGRAM=$ninjaExe",
    "-DCMAKE_BUILD_TYPE=$BuildType",
    "-DCMAKE_PREFIX_PATH=$qtRoot",
    "-DCMAKE_CXX_COMPILER=$gppExe"
)

& $cmakeExe @cmakeArgs

if (-not $ConfigureOnly) {
    & $cmakeExe --build $buildDir --config $BuildType
}
