$ErrorActionPreference = "Stop"

# === Configuration ===
$BUILD_DIR = "build"
$MODULE_NAME = "fileops"
$PYTHON_GUI_DIR = "gui"

# Create build directory if it doesn't exist
if (-not (Test-Path -Path $BUILD_DIR)) {
    New-Item -ItemType Directory -Path $BUILD_DIR | Out-Null
}

Set-Location $BUILD_DIR

Write-Host "Running CMake configuration..."
try {
    cmake ..
} catch {
    Write-Error "CMake configuration failed"
    exit 1
}

try {
    cmake --build .
} catch {
    Write-Error "Build failed"
    exit 1
}

# Recursively find the .pyd file
$OUTPUT_FILE = Get-ChildItem -Recurse -Filter "$MODULE_NAME*.pyd" | Select-Object -First 1

if (-not $OUTPUT_FILE) {
    Write-Error "Could not find compiled module (.pyd)"
    exit 1
}

Write-Host "Copying module $($OUTPUT_FILE.Name) to $PYTHON_GUI_DIR/"
try {
    Copy-Item $OUTPUT_FILE.FullName -Destination "..\$PYTHON_GUI_DIR"
} catch {
    Write-Error "Failed to copy .pyd file"
    exit 1
}

Set-Location ..

if ($args.Count -ge 1 -and $args[0] -eq "run") {
    python "$PYTHON_GUI_DIR\main.py"
}
