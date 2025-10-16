# UnityPhysXFlow

A native Windows bridge for NVIDIA Omniverse PhysX Flow 2.2 (PhysX 5.6.1) to integrate real-time fluid simulation into Unity.

This repo provides:
- `unity_physx_flow.dll` – native C++ plugin that loads `nvflow.dll`/`nvflowext.dll` and exposes a C ABI
- Unity C# wrapper (`UnityPhysXFlow.cs`) with event callbacks, main-thread dispatch, and simulation API
- Flow runtime loading via `NvFlowLoader.h` (no static linking required)
- **Unity Components**: `FlowEmitter`, `FlowGrid` for easy scene setup
- **Volumetric Rendering**: Built-in shader for rendering Flow grids in Unity

## Prerequisites
- **Windows** with Visual Studio Build Tools or VS 2022 (MSVC 17+)
- **CMake** 3.20+ and **Ninja** (or VS generator) in PATH
- **PhysX 5.6.1 SDK** checked out at the repo root in `PhysX/` (should contain `physx`, `flow`, `blast`)
- **Build Flow** once to obtain runtime DLLs: run `PhysX/flow/build.bat` to generate `_build/windows-x86_64/{debug,release}` with `nvflow.dll` and `nvflowext.dll`

## Build the Native Plugin
From the repo root:
```pwsh
./build.ps1 -Configuration Release
```
Artifacts go to `native/_build/Release/unity_physx_flow.dll` (or `_build/bin` for Ninja builds). Build artifacts also include the `.lib` import library.

## Unity Setup
1. **Copy the DLL** into your Unity project:
   - Place `unity_physx_flow.dll` under `Assets/Plugins/x86_64/` (Editor and standalone)
2. **Add the C# scripts**:
   - Copy `Unity/UnityPhysXFlow.cs` to `Assets/Plugins/UnityPhysXFlow/Runtime/`
   - Copy `Unity/FlowEmitter.cs` and `Unity/FlowGrid.cs` to `Assets/Plugins/UnityPhysXFlow/Runtime/`
   - Copy `Unity/Editor/FlowComponentEditors.cs` to `Assets/Plugins/UnityPhysXFlow/Editor/`
3. **Add the shader**:
   - Copy `Unity/Shaders/VolumetricFluid.shader` to `Assets/Plugins/UnityPhysXFlow/Shaders/`
4. **Deploy Flow runtime DLLs**:
   - Copy `nvflow.dll` and `nvflowext.dll` from `PhysX/flow/_build/windows-x86_64/release/` next to the Unity Editor executable (or your built game `.exe`)
   - *Alternative*: Call `UnityPhysXFlow.SetDllDirectory(path)` at startup to point at the Flow DLL folder
5. **Add the example component** `UnityPhysXFlowExample` to a scene and hit Play; the console should log per-frame flush messages with profiler JSON

## Quick Start: Create a Fluid Scene

### Method 1: Using the Example Component (Automatic)
1. Create an empty GameObject named "FlowSystem"
2. Add the `UnityPhysXFlowExample` component
3. Enable "Create Demo Scene"
4. Create a material using the `PhysXFlow/VolumetricFluid` shader
5. Assign the material to the `volumetricMaterial` field
6. Press Play!

### Method 2: Manual Setup (More Control)
1. **Initialize UnityPhysXFlow**:
   - Create a GameObject with `UnityPhysXFlowExample` component (or call `UnityPhysXFlow.Init()` in your own script)

2. **Create a Fluid Grid**:
   - Create a GameObject named "FluidGrid"
   - Add the `FlowGrid` component
   - Set grid resolution (e.g., 64x64x64) and cell size (e.g., 0.1)
   - Create a material using `PhysXFlow/VolumetricFluid` shader
   - Assign the material to the `volumetricMaterial` field

3. **Add Emitters**:
   - Create a GameObject named "FluidEmitter"
   - Add the `FlowEmitter` component
   - Position it above your grid
   - Adjust radius and density as needed

4. **Press Play** and watch the fluid simulation!

## API Overview

### Core API
```csharp
using UnityPhysXFlow;

// Initialize Flow and register event callback
int rc = UnityPhysXFlow.Init((eventType, payload) => {
    Debug.Log($"Flow event {eventType}: {payload}");
});

// Per-frame step
UnityPhysXFlow.Step(Time.deltaTime);

// Shutdown
UnityPhysXFlow.Shutdown();
```

### Simulation API
```csharp
// Create emitter
int emitterHandle = UnityPhysXFlow.CreateEmitter(Vector3.zero, radius: 1f, density: 1f);

// Update emitter
UnityPhysXFlow.SetEmitterParams(emitterHandle, newPosition, newRadius, newDensity);

// Create grid
int gridHandle = UnityPhysXFlow.CreateGrid(64, 64, 64, cellSize: 0.1f);

// Export for rendering
Texture3D densityTex = UnityPhysXFlow.ExportGridDensityAsTexture3D(gridHandle);
myMaterial.SetTexture("_DensityTex", densityTex);

// Cleanup
UnityPhysXFlow.DestroyEmitter(emitterHandle);
UnityPhysXFlow.DestroyGrid(gridHandle);
```

For detailed API documentation, see [SIMULATION_API.md](SIMULATION_API.md).

## Rendering Fluids in Unity

### Built-in Volumetric Shader
The included `VolumetricFluid.shader` provides raymarched volumetric rendering:

**Shader Properties:**
- `_DensityTex`: Density 3D texture (auto-set by FlowGrid)
- `_VelocityTex`: Velocity 3D texture (auto-set by FlowGrid)
- `_DensityScale`: Visibility multiplier
- `_Color`: Fluid color tint
- `_StepSize`: Ray marching step size (quality vs performance)
- `_MaxSteps`: Maximum ray marching iterations

**Recommended Settings:**
- Resolution: Start with 64³ grids for real-time performance
- StepSize: 0.01 for good quality
- MaxSteps: 128
- UpdateInterval: 1-5 frames to reduce texture upload overhead

### Advanced Rendering Options

For production-quality rendering, consider:
- **NanoVDB Export**: Export sparse volumes for efficient large-scale simulations
- **HDRP Integration**: Use Unity's Volumetric Fog system with custom density injection
- **Compute Shader Optimization**: Offload raymarching to compute for better performance

## Components Reference

### FlowEmitter
Spawns fluid particles into the simulation.

**Inspector Properties:**
- `radius`: Size of emitter sphere (0.1-10)
- `density`: Density of emitted fluid (0.1-10)
- `autoCreate`: Auto-create on Start

Position is synced from Transform each frame.

### FlowGrid
Manages a simulation grid and exports data for rendering.

**Inspector Properties:**
- `sizeX/Y/Z`: Grid resolution
- `cellSize`: Cell size in world units
- `volumetricMaterial`: Material for rendering
- `updateInterval`: Update textures every N frames
- `autoCreate`: Auto-create on Start

The component automatically updates density/velocity textures and binds them to the material.

## Current Status
- ✅ Native plugin refactored to `unity_physx_flow` with Upf_* API
- ✅ C# wrapper with simulation API (emitters, grids, texture export)
- ✅ Unity components (`FlowEmitter`, `FlowGrid`) with custom editors
- ✅ Volumetric rendering shader for Unity
- ✅ Build pipeline validated (Debug and Release)
- 🔲 Integrate actual Flow simulation (currently using placeholder data)
- 🔲 NanoVDB export for sparse grids
- 🔲 HDRP/URP volumetric fog integration

## Notes
- The bridge initializes Flow using the **Vulkan backend** (like the Flow editor) so it doesn't require sharing Unity's graphics device
- Profiler events are forwarded as JSON (event type `2` for profiler batches, `0` for step info, `1` for errors)
- The example component runs in Edit Mode and Play Mode; the main-thread dispatcher auto-bootstraps
- **Current Limitation**: The simulation API is scaffolded but uses placeholder data. Full Flow integration (actual fluid dynamics) is next.

## Documentation
- [Simulation API Reference](SIMULATION_API.md)
- [Deployment Guide](DEPLOYMENT.md)
- [Flow Build Troubleshooting](FLOW_BUILD_WORKAROUND.md)

## License
This bridge code is MIT. NVIDIA PhysX/Flow SDKs retain their respective licenses.
