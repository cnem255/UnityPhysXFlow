# Publishing to GitHub - Quick Guide

## One-Time Setup

### 1. Create GitHub Repository

1. Go to https://github.com/new
2. Repository name: `UnityPhysXFlow`
3. Description: "NVIDIA PhysX Flow 2.2 integration for Unity with volumetric rendering"
4. Public or Private (recommend Public for Unity Package Manager)
5. **Do not** initialize with README (you already have one)
6. Click "Create repository"

### 2. Connect Local Repository

```powershell
# Add GitHub as remote
git remote add origin https://github.com/YOUR-USERNAME/UnityPhysXFlow.git

# Verify remote
git remote -v

# Push main branch and all tags
git push -u origin main --tags
```

### 3. Configure Repository Settings

#### Enable Git LFS (if using)
```powershell
git lfs install
git lfs track "*.dll"
git add .gitattributes
git commit -m "Configure Git LFS"
git push
```

#### Enable GitHub Actions
1. Go to repository → Settings → Actions → General
2. Enable "Allow all actions and reusable workflows"
3. Save

#### Set Repository Topics (Optional)
1. Go to repository main page
2. Click ⚙️ next to "About"
3. Add topics: `unity`, `physics`, `fluid-simulation`, `physx`, `nvidia`, `unity-package`

#### Enable Discussions (Optional)
Settings → General → Features → Check "Discussions"

---

## Create First Release

### Automatic (Recommended)

The build-release workflow triggers on Git tags:

```powershell
# Option 1: Manual tag
git tag v1.0.0
git push origin v1.0.0

# Option 2: Use version script
.\update-version.ps1 -BumpType patch
# Follow prompts, then:
git push origin main
git push origin v1.0.0
```

GitHub Actions will automatically:
1. Build native DLL
2. Build Flow SDK
3. Package Unity package as .zip
4. Create GitHub release with artifacts

### Manual Release (if needed)

1. Go to repository → Releases
2. Click "Create a new release"
3. Choose tag: `v1.0.0` (or create new)
4. Release title: `UnityPhysXFlow v1.0.0`
5. Description: Copy from CHANGELOG.md
6. Attach files:
   - `UnityPhysXFlow-1.0.0.zip` (package)
   - `unity_physx_flow.dll`
   - `nvflow.dll`, `nvflowext.dll`
7. Click "Publish release"

---

## Share with Unity Users

### Installation Instructions

Users can now install via Unity Package Manager:

1. Open Unity Package Manager (`Window > Package Manager`)
2. Click `+` → `Add package from git URL`
3. Enter:
   ```
   https://github.com/YOUR-USERNAME/UnityPhysXFlow.git?path=/Packages/com.nvidia.physxflow
   ```

### Update README.md

Replace `YOUR-USERNAME` with your actual GitHub username:

```powershell
# Find and replace in package README
$files = @(
    "Packages\com.nvidia.physxflow\README.md",
    "Packages\com.nvidia.physxflow\INSTALLATION.md",
    "README.md"
)

foreach ($file in $files) {
    (Get-Content $file) -replace 'YOUR-USERNAME', 'ActualGitHubUsername' | Set-Content $file
}

git add .
git commit -m "docs: Update GitHub username in URLs"
git push
```

---

## Ongoing Maintenance

### Releasing Updates

#### Auto-version workflow (on every push to main):
```powershell
# Make changes
git add .

# Commit with version hint
git commit -m "feat: Added new feature [minor]"
# or "fix: Bug fix" (auto-detects as patch)
# or "breaking: API change [major]"

# Push
git push origin main

# Auto-version workflow will:
# - Bump version
# - Update CHANGELOG
# - Create tag
# - Trigger build-release workflow
```

#### Manual version bump:
```powershell
# Update version
.\update-version.ps1 -BumpType minor

# Push commit and tag
git push origin main
git push origin v1.1.0
```

### Monitoring

- **Actions tab**: Monitor workflow runs
- **Insights → Traffic**: See repository views
- **Releases**: Track downloads
- **Issues**: User bug reports
- **Discussions**: Community questions

---

## Testing Your Published Package

### Create Test Unity Project

1. Open Unity Hub
2. Create new project (Unity 2021.3+)
3. Open Package Manager
4. Add package from Git URL:
   ```
   https://github.com/YOUR-USERNAME/UnityPhysXFlow.git?path=/Packages/com.nvidia.physxflow
   ```

### Verify Installation

1. Check Package Manager shows correct version
2. Verify components appear: `Add Component > PhysX Flow`
3. Check menu items: `Tools > PhysX Flow`
4. Run Setup Wizard
5. Test basic scene creation

---

## Troubleshooting

### "Repository not found" Error

**Issue**: Unity can't access repository

**Solutions**:
- Make repository public, or
- Authenticate with GitHub:
  ```
  git config --global credential.helper manager
  ```
- Use personal access token in URL

### Workflows Not Running

**Issue**: GitHub Actions not triggering

**Solutions**:
- Check Actions are enabled in repository settings
- Verify workflow files are in `.github/workflows/`
- Check workflow syntax with GitHub Actions validator
- Look for error messages in Actions tab

### Package Not Updating in Unity

**Issue**: Unity doesn't show new version

**Solutions**:
- Close and reopen Package Manager
- Click refresh button in Package Manager
- Remove and re-add package
- Check that Git tag was pushed: `git tag -l`

---

## Best Practices

### Versioning
- **Patch** (1.0.X): Bug fixes, documentation updates
- **Minor** (1.X.0): New features, backward compatible
- **Major** (X.0.0): Breaking changes, API redesign

### Commit Messages
- Use conventional commits: `feat:`, `fix:`, `docs:`, `chore:`
- Be descriptive: "fix: Memory leak in grid export" vs "Fixed bug"
- Reference issues: "fix #123: Crash on shutdown"

### Releases
- Write clear release notes
- Highlight breaking changes
- Include migration guides for major versions
- Tag pre-releases: `v1.0.0-beta.1`

### Documentation
- Keep CHANGELOG.md updated
- Update documentation with API changes
- Add examples for new features
- Respond to issues and discussions

---

## Success Checklist

Before announcing your package:

- [ ] Repository pushed to GitHub
- [ ] First release (v1.0.0) created
- [ ] GitHub Actions workflows tested
- [ ] Package installs successfully in clean Unity project
- [ ] Components work as expected
- [ ] Documentation URLs updated (no "YOUR-USERNAME")
- [ ] License and copyright information correct
- [ ] README has clear installation instructions
- [ ] At least one release with Flow DLLs attached

---

## Announcement Templates

### GitHub Release Description
```markdown
## 🌊 UnityPhysXFlow v1.0.0

First release of NVIDIA PhysX Flow integration for Unity!

### ✨ Features
- Real-time GPU fluid simulation via PhysX Flow 2.2
- Unity components (FlowEmitter, FlowGrid)
- Volumetric rendering shader
- Easy installation via Unity Package Manager
- Complete API for simulation control

### 📦 Installation
Add via Unity Package Manager:
https://github.com/YOUR-USERNAME/UnityPhysXFlow.git?path=/Packages/com.nvidia.physxflow

### 📚 Documentation
See [README](https://github.com/YOUR-USERNAME/UnityPhysXFlow) for full setup guide.

### ⚠️ Requirements
- Unity 2021.3+
- Windows x64
- Download nvflow.dll and nvflowext.dll from this release
```

### Forum Post / Reddit
```markdown
Title: [Released] UnityPhysXFlow - NVIDIA PhysX Flow Fluid Simulation for Unity

I've created an integration of NVIDIA PhysX Flow 2.2 for Unity, bringing GPU-accelerated fluid simulation with easy-to-use components.

**Features:**
- Native PhysX Flow integration (no stubbed modes)
- Unity-native components and inspectors
- Volumetric rendering shader included
- Install via Unity Package Manager in one line

**Installation:**
Window > Package Manager > + > Add package from git URL
Enter: https://github.com/YOUR-USERNAME/UnityPhysXFlow.git?path=/Packages/com.nvidia.physxflow

**Links:**
- GitHub: https://github.com/YOUR-USERNAME/UnityPhysXFlow
- Documentation: [link]
- Demo: [optional video/gif]

Feedback and contributions welcome!
```

---

## Next Steps After Publishing

1. **Test with fresh Unity project** to verify installation
2. **Create demo video/GIF** for README
3. **Announce** on Unity forums, Reddit (r/Unity3D), Twitter
4. **Monitor** issues and discussions
5. **Plan** next version features
6. **Consider** submitting to OpenUPM registry

---

## Commands Summary

```powershell
# Initial publish
git remote add origin https://github.com/YOUR-USERNAME/UnityPhysXFlow.git
git push -u origin main --tags

# Create first release
git tag v1.0.0
git push origin v1.0.0

# Future updates (automatic)
git add .
git commit -m "feat: New feature [minor]"
git push origin main
# Auto-version will handle the rest

# Or manual updates
.\update-version.ps1 -BumpType minor
git push origin main
git push origin v1.1.0
```

---

**You're all set! 🚀**

After pushing to GitHub, your Unity package is ready for the world!
