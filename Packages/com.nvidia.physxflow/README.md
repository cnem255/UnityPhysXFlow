# NVIDIA PhysX Flow for Unity

Real-time fluid simulation powered by NVIDIA PhysX Flow 2.2, integrated directly into Unity.

## Features

- **Fluid Simulation**: GPU-accelerated PhysX Flow simulation
- **Unity Components**: Native-feeling emitters and grids
- **Volumetric Rendering**: Built-in raymarched shader
- **Easy Setup**: One-click wizard and menu items
- **Performance**: Optimized for real-time gameplay

## Quick Start

### Installation

#### Option 1: Unity Package Manager (Recommended)
1. Open Unity Package Manager (`Window > Package Manager`)
2. Click `+` → `Add package from git URL`
3. Enter: `https://github.com/cnem255/UnityPhysXFlow.git?path=/Packages/com.nvidia.physxflow`
4. Click `Add`
5. **Flow DLLs install automatically!** ✨

#### Option 2: Manual Installation
1. Clone or download this repository
2. Copy `Packages/com.nvidia.physxflow` to your Unity project's `Packages` folder

### ✨ Automatic DLL Installation

**No manual setup required!** The package automatically:

- ✅ **Installs Flow runtime DLLs** to Unity Editor on first import
- ✅ **Copies DLLs to builds** automatically when you build your game
- ✅ **Updates DLLs** when package version changes

**Verification**: Check via `Tools > PhysX Flow > Verify DLL Installation`

**Manual Control** (if needed):
- `Tools > PhysX Flow > Install Flow Runtime DLLs` - Reinstall DLLs
- `Tools > PhysX Flow > Uninstall Flow Runtime DLLs` - Remove DLLs

The DLLs (`nvflow.dll` and `nvflowext.dll`) are:
- ✅ Included in the package (no separate download needed)
- ✅ Automatically copied to Unity Editor directory (for development)
- ✅ Automatically copied to build output (for distribution)

### First Scene

1. **Menu**: `Tools > PhysX Flow > Setup Wizard`
2. Set DLL path (if not next to Unity.exe)
3. Check "Create Demo Scene"
4. Click "Create Flow Setup"
5. Press Play!

## Components

### FlowEmitter
Spawns fluid particles at a position.

```csharp
FlowEmitter emitter = gameObject.AddComponent<FlowEmitter>();
emitter.radius = 1.5f;
emitter.density = 2.0f;
```

### FlowGrid
Manages simulation grid and rendering.

```csharp
FlowGrid grid = gameObject.AddComponent<FlowGrid>();
grid.sizeX = grid.sizeY = grid.sizeZ = 64;
grid.cellSize = 0.1f;
grid.volumetricMaterial = myMaterial;
```

## API Example

```csharp
using UnityPhysXFlow;

// Initialize
UnityPhysXFlow.Init((evt, payload) => Debug.Log(payload));

// Create emitter
int emitter = UnityPhysXFlow.CreateEmitter(Vector3.zero, radius: 1f, density: 1f);

// Create grid
int grid = UnityPhysXFlow.CreateGrid(64, 64, 64, cellSize: 0.1f);

// Game loop
void Update()
{
    UnityPhysXFlow.Step(Time.deltaTime);
}

// Cleanup
UnityPhysXFlow.DestroyEmitter(emitter);
UnityPhysXFlow.DestroyGrid(grid);
UnityPhysXFlow.Shutdown();
```

## Documentation

- **Quick Reference**: `Documentation~/QUICK_REFERENCE.md`
- **API Reference**: `Documentation~/SIMULATION_API.md`
- **Deployment Guide**: `Documentation~/UNITY_DEPLOYMENT_CHECKLIST.md`

Access in Unity: `Tools > PhysX Flow > Documentation`

## Requirements

- Unity 2021.3 or later
- Windows x64
- NVIDIA GPU (recommended for best performance)
- PhysX Flow runtime DLLs (see Installation)

## Performance Tips

- Start with 64³ grids for good real-time performance
- Increase `FlowGrid.updateInterval` to reduce texture uploads
- Adjust shader `_StepSize` and `_MaxSteps` for quality/performance balance

## Known Limitations

- **Current Version**: Uses placeholder simulation data
- **Next Update**: Full PhysX Flow simulation integration
- **Platform**: Windows only (Linux/Mac support planned)

## Support

- **Issues**: [GitHub Issues](https://github.com/your-username/UnityPhysXFlow/issues)
- **Discussions**: [GitHub Discussions](https://github.com/your-username/UnityPhysXFlow/discussions)
- **Documentation**: [Full Docs](https://github.com/your-username/UnityPhysXFlow)

## License

- **UnityPhysXFlow**: MIT License
- **PhysX/Flow SDK**: NVIDIA License

See LICENSE.md for details.

## Version

**1.0.0** - Initial release with simulation API and volumetric rendering

See CHANGELOG.md for version history.
