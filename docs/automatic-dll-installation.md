# Automatic DLL Installation - Technical Details

## Overview

UnityPhysXFlow v1.0.0+ includes an automatic DLL installation system that eliminates manual setup. This document explains how it works for contributors and advanced users.

---

## Architecture

### Components

1. **FlowDllInstaller.cs** - Automatic installation on package import
2. **FlowBuildPostProcessor.cs** - Build-time DLL copying
3. **Flow Runtime DLLs** - Included in package via Git LFS

### Flow Diagram

```
Package Import
     ↓
[InitializeOnLoad]
     ↓
Check: DLLs installed?
     ↓
  No → Install DLLs to Unity Editor directory
     ↓
  Yes → Check version
     ↓
Version mismatch? → Reinstall
     ↓
Done ✅

Build Process
     ↓
IPreprocessBuildWithReport
     ↓
Validate DLLs present
     ↓
IPostprocessBuildWithReport
     ↓
Copy DLLs to build output
     ↓
Done ✅
```

---

## FlowDllInstaller.cs

### Trigger: Package Import

Uses `[InitializeOnLoad]` to run automatically when Unity loads the package.

### Installation Logic

```csharp
1. Check EditorPrefs for installation status
2. If not installed OR version changed:
   a. Locate package directory
   b. Find source DLLs in Plugins/x86_64/
   c. Get Unity Editor directory
   d. Check write permissions
   e. Copy DLLs with overwrite
   f. Update EditorPrefs
3. Log success/failure
```

### EditorPrefs Keys

- `NvidiaPhysXFlow.DllsInstalled` - Boolean flag
- `NvidiaPhysXFlow.InstalledVersion` - Installed version string

### Menu Items

#### Install Flow Runtime DLLs
- Path: `Tools > PhysX Flow > Install Flow Runtime DLLs`
- Priority: 100
- Function: Manual (re)installation

#### Verify DLL Installation
- Path: `Tools > PhysX Flow > Verify DLL Installation`
- Priority: 101
- Function: Check DLL presence and report status

#### Uninstall Flow Runtime DLLs
- Path: `Tools > PhysX Flow > Uninstall Flow Runtime DLLs`
- Priority: 102
- Function: Remove DLLs and clear EditorPrefs

#### Open Package Documentation
- Path: `Tools > PhysX Flow > Open Package Documentation`
- Priority: 200
- Function: Open INSTALLATION.md or GitHub repo

---

## FlowBuildPostProcessor.cs

### Trigger: Build Completion

Implements `IPostprocessBuildWithReport` with `callbackOrder = 0`.

### Build Process

```csharp
1. Check if build target is Windows (x64 or x86)
2. If not Windows:
   - Log warning
   - Return (no action)
3. Get build output directory
4. Locate package via PackageInfo
5. Find source DLLs in Plugins/x86_64/
6. Copy DLLs to build directory
7. Log success/failure
```

### Validation

Implements `IPreprocessBuildWithReport` to validate DLLs exist before building.

---

## DLL Locations

### Development (Unity Editor)

**Windows**:
```
C:\Program Files\Unity\Hub\Editor\{version}\Editor\
├── Unity.exe
├── nvflow.dll          ← Installed here
└── nvflowext.dll       ← Installed here
```

**Portable Unity**:
```
C:\Unity\{version}\
├── Unity.exe
├── nvflow.dll
└── nvflowext.dll
```

### Builds (Standalone)

```
YourGame_Data\..\
├── YourGame.exe
├── YourGame_Data\
├── nvflow.dll          ← Copied here
└── nvflowext.dll       ← Copied here
```

### Package (Source)

```
Packages/com.nvidia.physxflow/Plugins/x86_64/
├── unity_physx_flow.dll    (Unity bridge)
├── nvflow.dll              (Flow runtime)
└── nvflowext.dll           (Flow extensions)
```

---

## Git LFS Configuration

DLLs are stored with Git LFS to avoid bloating the repository.

### .gitattributes

```properties
*.dll filter=lfs diff=lfs merge=lfs -text
*.so filter=lfs diff=lfs merge=lfs -text
*.dylib filter=lfs diff=lfs merge=lfs -text
```

### Verification

```bash
git lfs ls-files
# Should show:
# nvflow.dll
# nvflowext.dll
# unity_physx_flow.dll
```

---

## Permission Handling

### Windows Protected Directories

Unity Editor is often installed in `C:\Program Files\`, which requires admin rights.

### Solutions Implemented

1. **Detect Insufficient Permissions**
   - Try writing test file
   - Catch `UnauthorizedAccessException`

2. **Inform User**
   - Show dialog with clear instructions
   - Offer alternatives:
     - Run Unity as Administrator
     - Manual installation
     - Use portable Unity

3. **Graceful Fallback**
   - Manual installation menu item
   - Manual instructions in dialog
   - Documentation link

---

## Error Handling

### Common Errors

#### 1. Package Not Found
**Cause**: PackageInfo.FindForAssetPath() returns null  
**Solution**: Fallback to manual path search

#### 2. Source DLLs Missing
**Cause**: DLLs not in Plugins/x86_64/  
**Solution**: Show manual installation dialog

#### 3. Access Denied
**Cause**: Insufficient permissions  
**Solution**: Prompt to run as admin or use manual method

#### 4. DLL Already Loaded
**Cause**: Unity has DLL locked  
**Solution**: Retry on next Unity restart

---

## Version Management

### Version Checking

```csharp
const string CURRENT_VERSION = "1.0.0";

bool ShouldInstallDlls()
{
    string installedVersion = EditorPrefs.GetString(VERSION_KEY, "0.0.0");
    return installedVersion != CURRENT_VERSION;
}
```

### Update Process

1. User updates package via UPM
2. Unity reloads domain
3. `[InitializeOnLoad]` runs
4. Version mismatch detected
5. DLLs automatically reinstalled
6. Version updated in EditorPrefs

---

## Testing

### Manual Testing

#### Test Installation
1. Delete DLLs from Unity Editor directory
2. Delete EditorPrefs keys (Edit → Clear All PlayerPrefs)
3. Restart Unity
4. Check Console for installation message
5. Verify DLLs exist via menu item

#### Test Build
1. Build Windows x64 standalone
2. Check Console for "Flow DLLs copied" message
3. Check build directory for nvflow.dll and nvflowext.dll
4. Run built .exe - should not show DLL missing error

#### Test Manual Controls
1. Use "Uninstall" menu item
2. Verify DLLs removed
3. Use "Install" menu item
4. Verify DLLs restored
5. Use "Verify" menu item
6. Check status report

---

## Future Improvements

### Planned Enhancements

1. **Multi-Platform Support**
   - Linux: Copy .so files
   - macOS: Copy .dylib files
   - Platform-specific paths

2. **Custom Installation Path**
   - Allow user to specify DLL directory
   - Store in EditorPrefs
   - Use for both editor and builds

3. **DLL Checksums**
   - Verify DLL integrity
   - Detect corruption
   - Auto-repair if needed

4. **Silent Mode**
   - Suppress dialogs for CI/CD
   - Log to file instead
   - Exit codes for automation

5. **Advanced Options**
   - Skip installation preference
   - Custom DLL source path
   - Build-time DLL exclusion

---

## Troubleshooting

### "Installation failed" in Console

**Check**:
1. Unity Editor path is writable
2. Source DLLs exist in package
3. Git LFS pulled files (not pointers)
4. Antivirus not blocking

**Solution**:
- Run Unity as Administrator
- Use manual installation
- Check Git LFS status: `git lfs pull`

### DLLs Missing After Build

**Check**:
1. Build target is Windows
2. Console shows "Flow DLLs copied" message
3. Build succeeded without errors

**Solution**:
- Manually copy from Packages/com.nvidia.physxflow/Plugins/x86_64/
- Check for build script conflicts
- Verify FlowBuildPostProcessor.cs is compiled

### "DLL not found" at Runtime

**Check**:
1. DLLs in same directory as .exe
2. DLLs are x64 (not x86)
3. Dependencies satisfied (Visual C++ Redistributable)

**Solution**:
- Install VC++ Redist 2015-2022
- Verify DLLs with `dumpbin /dependents nvflow.dll`
- Check Event Viewer for detailed error

---

## Contributing

### Adding New DLLs

1. Copy DLL to `Plugins/x86_64/`
2. Update `FlowDllInstaller.cs`:
   - Add new DLL to source/dest paths
   - Update copy logic
   - Update verification
3. Update `FlowBuildPostProcessor.cs`
4. Update CHANGELOG.md
5. Test on clean Unity install

### Modifying Installation Logic

1. Edit `FlowDllInstaller.cs`
2. Update `CURRENT_VERSION` to force reinstall for testers
3. Test with clean EditorPrefs
4. Document changes in this file
5. Update user-facing documentation

---

## See Also

- [INSTALLATION.md](INSTALLATION.md) - User installation guide
- [CHANGELOG.md](CHANGELOG.md) - Version history
- [README.md](README.md) - Package overview

---

**Last Updated**: 2025-10-16  
**Version**: 1.0.0
