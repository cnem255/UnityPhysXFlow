# Build script for unity_physx_flow.dll
# This compiles the C++ plugin that integrates NVIDIA PhysX Flow with Unity

param(
    [string]$Config = "Release",
    [switch]$Clean,
    [switch]$Install
)

$ErrorActionPreference = "Stop"

$ProjectRoot = $PSScriptRoot
$NativeDir = Join-Path $ProjectRoot "native"
$BuildDir = Join-Path $NativeDir "_build"
$PhysXRoot = Join-Path $ProjectRoot "PhysX"

Write-Host "=== UnityPhysXFlow Native Build ===" -ForegroundColor Cyan
Write-Host "Project Root: $ProjectRoot"
Write-Host "Native Dir: $NativeDir"
Write-Host "Build Dir: $BuildDir"
Write-Host "PhysX Root: $PhysXRoot"
Write-Host "Configuration: $Config"
Write-Host ""

# Verify PhysX Flow headers exist
$FlowInclude = Join-Path $PhysXRoot "flow\include"
if (-not (Test-Path $FlowInclude)) {
    Write-Error "PhysX Flow headers not found at: $FlowInclude"
    Write-Error "Make sure you have PhysX SDK 5.6.1 with Flow in the PhysX/ folder"
    exit 1
}

Write-Host "✓ Found PhysX Flow headers at: $FlowInclude" -ForegroundColor Green

# Clean if requested
if ($Clean) {
    Write-Host "Cleaning build directory..." -ForegroundColor Yellow
    if (Test-Path $BuildDir) {
        Remove-Item -Recurse -Force $BuildDir
    }
}

# Create build directory
if (-not (Test-Path $BuildDir)) {
    New-Item -ItemType Directory -Path $BuildDir | Out-Null
}

# Configure with CMake
Write-Host "`nConfiguring with CMake..." -ForegroundColor Cyan
Push-Location $BuildDir
try {
    $cmakeArgs = @(
        "-G", "Visual Studio 17 2022",
        "-A", "x64",
        "-DPHYSX_ROOT=$PhysXRoot",
        ".."
    )
    
    & cmake @cmakeArgs
    if ($LASTEXITCODE -ne 0) {
        Write-Error "CMake configuration failed"
        exit 1
    }
    
    Write-Host "✓ CMake configuration successful" -ForegroundColor Green
    
    # Build with CMake
    Write-Host "`nBuilding $Config..." -ForegroundColor Cyan
    & cmake --build . --config $Config
    if ($LASTEXITCODE -ne 0) {
        Write-Error "Build failed"
        exit 1
    }
    
    Write-Host "✓ Build successful" -ForegroundColor Green
    
    # Find output DLL
    $OutputDll = Join-Path $BuildDir "$Config\unity_physx_flow.dll"
    if (-not (Test-Path $OutputDll)) {
        Write-Error "Output DLL not found at: $OutputDll"
        exit 1
    }
    
    Write-Host "`n✓ Built: $OutputDll" -ForegroundColor Green
    
    # Install to Unity package if requested
    if ($Install) {
        $PluginDir = Join-Path $ProjectRoot "Packages\com.nvidia.physxflow\Plugins\x86_64"
        
        Write-Host "`nInstalling to Unity package..." -ForegroundColor Cyan
        Write-Host "Destination: $PluginDir"
        
        if (-not (Test-Path $PluginDir)) {
            New-Item -ItemType Directory -Path $PluginDir | Out-Null
        }
        
        Copy-Item $OutputDll -Destination $PluginDir -Force
        Write-Host "✓ Installed unity_physx_flow.dll" -ForegroundColor Green
        
        # Also verify nvflow.dll and nvflowext.dll are present
        $nvflowDll = Join-Path $PluginDir "nvflow.dll"
        $nvflowextDll = Join-Path $PluginDir "nvflowext.dll"
        
        if (-not (Test-Path $nvflowDll)) {
            Write-Warning "nvflow.dll not found in plugin directory!"
            Write-Warning "Copy it from PhysX\flow\bin\x64\"
        } else {
            Write-Host "✓ nvflow.dll present" -ForegroundColor Green
        }
        
        if (-not (Test-Path $nvflowextDll)) {
            Write-Warning "nvflowext.dll not found in plugin directory!"
            Write-Warning "Copy it from PhysX\flow\bin\x64\"
        } else {
            Write-Host "✓ nvflowext.dll present" -ForegroundColor Green
        }
    }
    
} finally {
    Pop-Location
}

Write-Host "`n=== Build Complete ===" -ForegroundColor Green
Write-Host "To install to Unity package, run with -Install flag"
Write-Host "Example: .\build-native.ps1 -Install"
