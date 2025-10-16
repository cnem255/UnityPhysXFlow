# Unity Package Manager Installation Guide

## Overview

UnityPhysXFlow can be installed via Unity Package Manager (UPM) using Git. **Flow runtime DLLs are automatically installed** - no manual setup required!

---

## ✨ Quick Install (Recommended)

### Steps

1. **Open Unity Package Manager**
   - In Unity Editor: `Window > Package Manager`

2. **Add Package from Git URL**
   - Click the `+` button (top-left corner)
   - Select `Add package from git URL...`

3. **Enter Package URL**
   ```
   https://github.com/cnem255/UnityPhysXFlow.git?path=/Packages/com.nvidia.physxflow
   ```

4. **Click Add**
   - Unity will download and install the package
   - Wait for compilation to complete
   - **Flow DLLs install automatically!** ✅

That's it! The package is ready to use.

---

## 🎉 Automatic DLL Installation

**No manual DLL setup required!** The package automatically handles Flow runtime DLLs:

### What Happens Automatically

✅ **On Package Import**:
- Flow runtime DLLs (`nvflow.dll`, `nvflowext.dll`) are automatically copied to your Unity Editor directory
- You'll see a console message confirming successful installation
- Everything is ready for development immediately

✅ **On Build**:
- DLLs are automatically copied to your build output directory
- Your game will have everything it needs to run
- Works for both Development and Release builds

✅ **On Package Update**:
- DLLs are automatically updated to match the new version
- No manual intervention needed

### Verification

To verify DLLs are installed correctly:

1. Go to `Tools > PhysX Flow > Verify DLL Installation`
2. You'll see the status of each DLL
3. All should show ✅ Installed

---

## 🛠️ Manual DLL Control (Optional)

If you need to manually manage DLLs:

### Reinstall DLLs
`Tools > PhysX Flow > Install Flow Runtime DLLs`

Use this if:
- Automatic installation failed
- DLLs were accidentally deleted
- You want to refresh the installation

### Uninstall DLLs
`Tools > PhysX Flow > Uninstall Flow Runtime DLLs`

Use this to:
- Remove DLLs from Unity Editor directory
- Clean up before uninstalling the package

### Verify Installation
`Tools > PhysX Flow > Verify DLL Installation`

Use this to:
- Check if DLLs are present
- Diagnose installation issues
- Confirm version compatibility

---

## 📦 Package Contents

After installation, you'll have:

```
Packages/com.nvidia.physxflow/
├── Runtime/                    # C# runtime scripts
│   ├── UnityPhysXFlow.cs      # Main API wrapper
│   ├── FlowEmitter.cs         # Emitter component
│   ├── FlowGrid.cs            # Grid component
│   └── *.asmdef               # Assembly definition
├── Editor/                     # Editor scripts
│   ├── FlowComponentEditors.cs
│   ├── FlowSetupMenu.cs       # Menu items & wizard
│   ├── FlowDllInstaller.cs    # Automatic DLL installer ✨
│   ├── FlowBuildPostProcessor.cs # Build automation ✨
│   └── *.asmdef
├── Shaders/
│   └── VolumetricFluid.shader # Rendering shader
├── Plugins/x86_64/
│   ├── unity_physx_flow.dll   # Native bridge DLL
│   ├── nvflow.dll             # Flow runtime DLL ✨
│   └── nvflowext.dll          # Flow extensions DLL ✨
└── Documentation~/             # Offline docs
```

---

## 📝 Installation Methods Comparison

| Method | DLL Installation | Updates | Best For |
|--------|------------------|---------|----------|
| **Git URL** | ✅ Automatic | ✅ One-click | Most users |
| **Specific Version** | ✅ Automatic | Manual | Stable projects |
| **Manual/Local** | ✅ Automatic | Manual | Development |

---

## 🔄 Updating the Package

### Via Git URL (Recommended)

Unity Package Manager checks for updates periodically:

1. **Check for Updates**
   - Open Package Manager
   - Select "PhysX Flow for Unity"
   - If update available, "Update" button appears

2. **Update**
   - Click "Update" button
   - Unity pulls latest version from Git
   - DLLs update automatically ✅

3. **Review Changes**
   - Check CHANGELOG.md for changes
   - Review breaking changes if major version update

### Specific Version Installation

To install a specific version:

```
https://github.com/cnem255/UnityPhysXFlow.git?path=/Packages/com.nvidia.physxflow#v1.0.0
```

The `#v1.0.0` specifies the git tag for that release.

---

## 🔧 Troubleshooting

### "DLL installation failed" Message

**Problem**: Automatic installation couldn't copy DLLs

**Common Causes**:
- Unity Editor directory requires admin permissions
- Antivirus blocking file operations
- Disk permissions issue

**Solutions**:
1. Run Unity as Administrator and try again
2. Use manual installation: `Tools > PhysX Flow > Install Flow Runtime DLLs`
3. Check Windows Event Viewer for access denied errors
4. Temporarily disable antivirus and retry

### "Package not found" Error

**Problem**: Unity can't find the Git repository

**Solutions**:
- Verify Git is installed: `git --version` in terminal
- Check GitHub URL is correct
- Ensure repository is public
- Try HTTPS instead of SSH URL

### "Failed to resolve packages" Error

**Problem**: Unity can't resolve the package

**Solutions**:
- Check Unity version (requires 2021.3+)
- Verify `package.json` exists in the path
- Try removing `Library/` folder and reimporting

### DLLs Not Working in Build

**Problem**: Built game shows "DLL not found" error

**Solutions**:
- Check Console during build for "Flow DLLs copied" message
- Verify DLLs are in build output directory (next to .exe)
- If missing, manually copy from `Packages/com.nvidia.physxflow/Plugins/x86_64/`

### Components Not Appearing

**Problem**: FlowEmitter/FlowGrid not in Add Component menu

**Solutions**:
- Wait for Unity to finish compiling scripts
- Check for compilation errors in Console
- Verify assembly definitions are present
- Try `Assets > Reimport All`

### Admin Permissions Required

**Problem**: Installation requires admin rights

**Why**: Unity Editor directory (e.g., `C:\Program Files\Unity\...`) is protected

**Solutions**:

**Option 1: Run Unity as Admin** (Recommended)
- Right-click Unity Hub → Run as Administrator
- Launch your project
- Installation will succeed automatically

**Option 2: Alternative DLL Location**
- Install DLLs to `Assets/Plugins/x86_64/` instead
- Go to `Tools > PhysX Flow > Install Flow Runtime DLLs`
- When prompted, choose custom location

**Option 3: Portable Unity**
- Install Unity to a non-protected location (e.g., `C:\Unity\`)
- No admin rights needed

---

## 📚 Advanced: Build Configuration

### Custom Build Scripts

If you have custom build scripts, DLLs are automatically included via `IPostprocessBuildWithReport`.

To customize:

```csharp
using NvidiaPhysXFlow.Editor;

// Access the build post-processor
// (Runs automatically, no code needed)
```

### Excluding DLLs from Builds

If you want to manually manage DLLs in builds:

1. Delete or rename `FlowBuildPostProcessor.cs`
2. Manually copy DLLs to build output

---

## 🌍 Platform Notes

### Windows
✅ **Fully Supported**
- x64 only
- DLLs automatically managed
- Works in Editor and builds

### macOS / Linux
⏳ **Coming Soon**
- Flow SDK supports these platforms
- Unity package integration planned for future release

---

## 🗑️ Uninstalling

### Via Package Manager

1. Open Package Manager
2. Select "PhysX Flow for Unity"
3. Click "Remove" button
4. Confirm removal

**Note**: DLLs in Unity Editor directory are **not** automatically removed for safety.

### Complete Uninstall

To remove everything including DLLs:

1. Uninstall package via Package Manager
2. Run `Tools > PhysX Flow > Uninstall Flow Runtime DLLs` (before removing package)
3. Or manually delete from Unity Editor directory:
   - `nvflow.dll`
   - `nvflowext.dll`

---

## ✅ Verifying Installation

After installation, verify everything works:

### 1. Check Package Manager
- `Window > Package Manager`
- Look for "PhysX Flow for Unity"
- Version should be displayed (e.g., 1.0.0)

### 2. Verify DLLs
- `Tools > PhysX Flow > Verify DLL Installation`
- All DLLs should show ✅ Installed

### 3. Check Components
- Create a new GameObject
- Click `Add Component`
- Search for "Flow"
- You should see:
  - `PhysX Flow > Flow Emitter`
  - `PhysX Flow > Flow Grid`

### 4. Check Menu Items
- `GameObject > PhysX Flow > ...` (scene creation)
- `Tools > PhysX Flow > ...` (utilities)

### 5. Test Initialization
1. Create an empty GameObject
2. Add `UnityPhysXFlowExample` component
3. Press Play
4. Check Console for: `[UnityPhysXFlow] Initialized successfully`

If all checks pass, installation is successful! ✅

---

## 🎓 Next Steps

After installation:

1. **Run Setup Wizard**: `Tools > PhysX Flow > Setup Wizard`
2. **Read Quick Start**: See package README.md
3. **Explore API**: Check Documentation~/API_REFERENCE.md
4. **Try Examples**: Import samples from Package Manager

---

## 📞 Support

- **Issues**: [GitHub Issues](https://github.com/cnem255/UnityPhysXFlow/issues)
- **Discussions**: [GitHub Discussions](https://github.com/cnem255/UnityPhysXFlow/discussions)
- **Documentation**: [Full Docs](https://github.com/cnem255/UnityPhysXFlow)

---

**Happy fluid simulating! 🌊**
