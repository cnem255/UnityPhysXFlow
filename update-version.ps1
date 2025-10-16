# Unity Package Version Update Script
# Usage: .\update-version.ps1 -BumpType [major|minor|patch]

param(
    [Parameter(Mandatory=$true)]
    [ValidateSet("major", "minor", "patch")]
    [string]$BumpType
)

$ErrorActionPreference = "Stop"

$packageJsonPath = "Packages\com.nvidia.physxflow\package.json"
$changelogPath = "Packages\com.nvidia.physxflow\CHANGELOG.md"

Write-Host "📦 Unity Package Version Updater" -ForegroundColor Cyan
Write-Host "================================" -ForegroundColor Cyan
Write-Host ""

# Read current version
if (-not (Test-Path $packageJsonPath)) {
    Write-Host "❌ Error: package.json not found at $packageJsonPath" -ForegroundColor Red
    exit 1
}

$packageJson = Get-Content $packageJsonPath | ConvertFrom-Json
$currentVersion = $packageJson.version

Write-Host "📌 Current version: $currentVersion" -ForegroundColor Yellow

# Parse version
$versionParts = $currentVersion -split '\.'
$major = [int]$versionParts[0]
$minor = [int]$versionParts[1]
$patch = [int]$versionParts[2]

# Bump version
switch ($BumpType) {
    "major" {
        $major++
        $minor = 0
        $patch = 0
    }
    "minor" {
        $minor++
        $patch = 0
    }
    "patch" {
        $patch++
    }
}

$newVersion = "$major.$minor.$patch"
Write-Host "🚀 New version: $newVersion" -ForegroundColor Green
Write-Host ""

# Ask for confirmation
$confirm = Read-Host "Update version from $currentVersion to $newVersion? (y/n)"
if ($confirm -ne "y") {
    Write-Host "❌ Cancelled" -ForegroundColor Yellow
    exit 0
}

# Update package.json
Write-Host "📝 Updating package.json..." -ForegroundColor Cyan
$packageJson.version = $newVersion
$packageJson | ConvertTo-Json -Depth 10 | Set-Content $packageJsonPath
Write-Host "✅ package.json updated" -ForegroundColor Green

# Update CHANGELOG
Write-Host "📝 Updating CHANGELOG.md..." -ForegroundColor Cyan
$date = Get-Date -Format "yyyy-MM-dd"
$changelogHeader = @"
# Changelog

## [$newVersion] - $date

### Changed
- Version bump to $newVersion

"@

$existingChangelog = Get-Content $changelogPath -Raw
$updatedChangelog = $changelogHeader + ($existingChangelog -replace '^# Changelog\s*', '')
Set-Content $changelogPath -Value $updatedChangelog
Write-Host "✅ CHANGELOG.md updated" -ForegroundColor Green

# Git operations
Write-Host ""
Write-Host "📦 Git operations..." -ForegroundColor Cyan

$gitStatus = git status --porcelain
if ($gitStatus) {
    Write-Host "Creating commit and tag..." -ForegroundColor Yellow
    
    git add $packageJsonPath
    git add $changelogPath
    git commit -m "chore: bump version to $newVersion"
    git tag "v$newVersion"
    
    Write-Host "✅ Committed and tagged as v$newVersion" -ForegroundColor Green
    Write-Host ""
    Write-Host "📤 To publish, run:" -ForegroundColor Cyan
    Write-Host "   git push origin main" -ForegroundColor White
    Write-Host "   git push origin v$newVersion" -ForegroundColor White
} else {
    Write-Host "⚠️  No changes to commit" -ForegroundColor Yellow
}

Write-Host ""
Write-Host "✅ Version update complete!" -ForegroundColor Green
Write-Host ""
Write-Host "Next steps:" -ForegroundColor Cyan
Write-Host "1. Push the commit and tag to trigger CI/CD" -ForegroundColor White
Write-Host "2. GitHub Actions will automatically build and create a release" -ForegroundColor White
Write-Host "3. Unity Package Manager users will be notified of the update" -ForegroundColor White
