# Unity Package Manager Installation Guide

## Overview

UnityPhysXFlow can be installed via Unity Package Manager (UPM) using Git, providing automatic updates and easy integration.

---

## Method 1: Install via Git URL (Recommended)

This method allows automatic updates through Unity Package Manager.

### Steps

1. **Open Unity Package Manager**
   - In Unity Editor: `Window > Package Manager`

2. **Add Package from Git URL**
   - Click the `+` button (top-left corner)
   - Select `Add package from git URL...`

3. **Enter Package URL**
   ```
   https://github.com/YOUR-USERNAME/UnityPhysXFlow.git?path=/Packages/com.nvidia.physxflow
   ```
   Replace `YOUR-USERNAME` with your GitHub username

4. **Click Add**
   - Unity will download and install the package
   - Wait for compilation to complete

5. **Install Flow Runtime DLLs** (See below)

### Updating

When a new version is released:
1. Open Package Manager
2. Select "PhysX Flow for Unity"
3. Click "Update" button when available

---

## Method 2: Install Specific Version

To install a specific version (e.g., v1.0.0):

```
https://github.com/YOUR-USERNAME/UnityPhysXFlow.git?path=/Packages/com.nvidia.physxflow#v1.0.0
```

The `#v1.0.0` specifies the git tag for that release.

---

## Method 3: Manual Installation

If you don't have Git or prefer manual installation:

### Steps

1. **Download Package**
   - Go to [Releases](https://github.com/YOUR-USERNAME/UnityPhysXFlow/releases)
   - Download `UnityPhysXFlow-X.X.X.zip`

2. **Extract to Packages Folder**
   ```
   YourUnityProject/
   └── Packages/
       └── com.nvidia.physxflow/    (extracted here)
   ```

3. **Restart Unity**
   - Unity will detect and import the package

### Updating Manually

1. Delete `Packages/com.nvidia.physxflow` folder
2. Download and extract new version
3. Restart Unity

---

## Installing Flow Runtime DLLs

**Important**: The package requires PhysX Flow runtime DLLs to function.

### Option A: Download from Releases (Easiest)

1. Go to [Releases](https://github.com/YOUR-USERNAME/UnityPhysXFlow/releases)
2. Download `nvflow.dll` and `nvflowext.dll`
3. Place them in one of these locations:

   **For Unity Editor (Development)**:
   ```
   C:\Program Files\Unity\Hub\Editor\2021.3.XX\Editor\
   ```
   (Next to `Unity.exe`)

   **For Builds (Standalone Games)**:
   ```
   YourUnityProject/Assets/Plugins/x86_64/
   ```

### Option B: Build from Source

If you need to build the DLLs yourself:

1. Clone the full repository (includes PhysX SDK)
2. Run `PhysX/flow/build.bat`
3. Find DLLs at: `PhysX/flow/_build/windows-x86_64/release/`
4. Copy to Unity as described above

### Option C: Set DLL Path at Runtime

In your Unity scene:
1. Add `UnityPhysXFlowExample` component
2. Set `flowDllPath` to the folder containing the DLLs
3. Example: `C:\Projects\PhysXFlow\bin\`

---

## Package Contents

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
│   └── *.asmdef
├── Shaders/
│   └── VolumetricFluid.shader # Rendering shader
├── Plugins/x86_64/
│   └── unity_physx_flow.dll   # Native bridge DLL
└── Documentation~/             # Offline docs
```

---

## Verifying Installation

### Check Package Manager

1. Open Package Manager
2. Look for "PhysX Flow for Unity" in the list
3. Version should be displayed (e.g., 1.0.0)

### Check Components

1. Create a new GameObject
2. Click `Add Component`
3. You should see:
   - `PhysX Flow > Flow Emitter`
   - `PhysX Flow > Flow Grid`

### Check Menu Items

Look for:
- `GameObject > PhysX Flow > ...`
- `Tools > PhysX Flow > ...`

### Test Initialization

1. Create an empty GameObject
2. Add `UnityPhysXFlowExample` component
3. Press Play
4. Check Console for: `[UnityPhysXFlow] Initialized successfully`

---

## Troubleshooting

### "Package not found" Error

**Problem**: Unity can't find the Git repository

**Solutions**:
- Verify Git is installed: `git --version` in terminal
- Check GitHub URL is correct
- Ensure repository is public (or you're authenticated)
- Try HTTPS instead of SSH URL

### "Failed to resolve packages" Error

**Problem**: Unity can't resolve the package

**Solutions**:
- Check Unity version (requires 2021.3+)
- Verify `package.json` exists in the path
- Try removing `Library/` folder and reimporting

### "Init failed with code -1" Error

**Problem**: Flow runtime DLLs not found

**Solutions**:
- Verify `nvflow.dll` and `nvflowext.dll` are in the correct location
- Check file permissions (not blocked by Windows)
- Try setting `flowDllPath` explicitly
- Check Console for detailed error messages

### Components Not Appearing

**Problem**: FlowEmitter/FlowGrid not in Add Component menu

**Solutions**:
- Wait for Unity to finish compiling scripts
- Check for compilation errors in Console
- Verify assembly definitions are present
- Try `Assets > Reimport All`

---

## Updating the Package

### Automatic Updates (Git URL Method)

Unity Package Manager will check for updates periodically:

1. **Check for Updates**
   - Open Package Manager
   - Select "PhysX Flow for Unity"
   - If update available, "Update" button will appear

2. **Update**
   - Click "Update" button
   - Unity will pull latest version from Git
   - Wait for compilation

3. **Review Changes**
   - Check CHANGELOG in package (Documentation~/)
   - Review breaking changes if major version

### Manual Update Notification

We recommend subscribing to:
- [GitHub Releases](https://github.com/YOUR-USERNAME/UnityPhysXFlow/releases)
- [GitHub Watch](https://github.com/YOUR-USERNAME/UnityPhysXFlow) (for notifications)

---

## Uninstalling

### Via Package Manager

1. Open Package Manager
2. Select "PhysX Flow for Unity"
3. Click "Remove" button
4. Confirm removal

### Manual Uninstall

1. Delete `Packages/com.nvidia.physxflow/` folder
2. Delete Flow DLLs from:
   - Unity Editor folder (if placed there)
   - `Assets/Plugins/x86_64/` (if placed there)
3. Restart Unity

---

## Advanced: Local Package Development

For contributing or local development:

### Using Local Path

1. Clone repository locally
2. In Unity, add package from disk:
   ```
   Window > Package Manager > + > Add package from disk...
   ```
3. Select `package.json` in `Packages/com.nvidia.physxflow/`

### Making Changes

1. Edit scripts in `Packages/com.nvidia.physxflow/`
2. Changes reflect immediately in Unity
3. Commit and push changes
4. Update version with `update-version.ps1`

---

## Support

- **Issues**: [GitHub Issues](https://github.com/YOUR-USERNAME/UnityPhysXFlow/issues)
- **Discussions**: [GitHub Discussions](https://github.com/YOUR-USERNAME/UnityPhysXFlow/discussions)
- **Documentation**: [Full Docs](https://github.com/YOUR-USERNAME/UnityPhysXFlow)

---

## Next Steps

After installation:
1. Run Setup Wizard: `Tools > PhysX Flow > Setup Wizard`
2. Follow [Quick Start Guide](README.md)
3. Check [API Documentation](Documentation~/SIMULATION_API.md)
4. Explore [Example Scenes](Samples~/BasicFluidScene)

Happy fluid simulating! 🌊
