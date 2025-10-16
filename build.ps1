param(
    [ValidateSet('Debug','Release')]
    [string]$Configuration = 'Release'
)

$root = Split-Path -Parent $MyInvocation.MyCommand.Path
$native = Join-Path $root 'native'
$build = Join-Path $native '_build'

if (!(Test-Path $build)) { New-Item -ItemType Directory -Force -Path $build | Out-Null }

# Pick a generator
$hasNinja = (Get-Command ninja -ErrorAction SilentlyContinue) -ne $null
$generator = $null

# Probe VS 2022 using vswhere
$vswhere = Join-Path ${env:ProgramFiles(x86)} 'Microsoft Visual Studio\Installer\vswhere.exe'
if (!(Test-Path $vswhere)) { $vswhere = Join-Path $env:ProgramFiles 'Microsoft Visual Studio\Installer\vswhere.exe' }
if (Test-Path $vswhere) {
    $vsVer = & $vswhere -latest -property installationVersion 2>$null
    if ($vsVer -and $vsVer.StartsWith('17')) { $generator = 'Visual Studio 17 2022' }
}

if (-not $generator) {
    if ($hasNinja) { $generator = 'Ninja' } else { $generator = 'Visual Studio 17 2022' }
}

# Configure
if ($hasNinja) {
    cmake -S $native -B $build -G $generator -DCMAKE_BUILD_TYPE=$Configuration
} else {
    cmake -S $native -B $build -G $generator
}
if ($LASTEXITCODE -ne 0) { throw "cmake configure failed" }

# Build
cmake --build $build --config $Configuration --target unity_physx_flow
if ($LASTEXITCODE -ne 0) { throw "build failed" }

Write-Host "Built native plugin to $build" -ForegroundColor Green
