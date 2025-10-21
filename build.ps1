$ErrorActionPreference = "Stop"

$BUILD_DIR = "build"

$run = $false

foreach ($arg in $args) {
    switch ($arg) {
        "--run" { $run = $true }
        default { }
    }
}

if (-not (Test-Path $BUILD_DIR)) {
    New-Item -ItemType Directory -Path $BUILD_DIR | Out-Null
}

cmake -B $BUILD_DIR -S . -DCMAKE_BUILD_TYPE=Debug

$jobs = [Environment]::ProcessorCount
cmake --build $BUILD_DIR -j $jobs

if ($run) {
    & ".\$BUILD_DIR\QtFiles"
}
