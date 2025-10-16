# UnityPhysXFlow# UnityPhysXFlow



A native Windows bridge for NVIDIA Omniverse PhysX Flow 2.2 (PhysX 5.6.1) to integrate real-time fluid simulation into Unity.A native Windows bridge for NVIDIA Omniverse PhysX Flow 2.2 (PhysX 5.6.1) to integrate real-time fluid simulation into Unity.



[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

[![Unity Version](https://img.shields.io/badge/Unity-2021.3+-blue.svg)](https://unity.com/)[![Unity Version](https://img.shields.io/badge/Unity-2021.3+-blue.svg)](https://unity.com/)

[![Platform](https://img.shields.io/badge/Platform-Windows%20x64-lightgrey.svg)]()[![Platform](https://img.shields.io/badge/Platform-Windows%20x64-lightgrey.svg)]()



## ✨ Features## Features



- 🌊 **Real-time Fluid Simulation** - GPU-accelerated PhysX Flow integration- 🌊 **Real-time Fluid Simulation**: GPU-accelerated PhysX Flow integration

- 🎮 **Unity Native Feel** - Components, inspectors, and menu items that blend seamlessly- 🎮 **Unity Native Feel**: Components, inspectors, and menu items that blend seamlessly

- 🎨 **Volumetric Rendering** - Built-in raymarched shader for beautiful fluid visuals- 🎨 **Volumetric Rendering**: Built-in raymarched shader for beautiful fluid visuals

- 📦 **Easy Installation** - Install via Unity Package Manager with one line- 📦 **Easy Installation**: Install via Unity Package Manager with one line

- 🔄 **Automatic Updates** - Get notified of new versions through UPM- 🔄 **Automatic Updates**: Get notified of new versions through UPM

- 🛠️ **Complete API** - Full control over emitters, grids, and simulation parameters- 🛠️ **Complete API**: Full control over emitters, grids, and simulation parameters



------



## 🚀 Quick Start## Quick Install



### For Unity Users (Recommended)### For Unity Users (Recommended)



Install via Unity Package Manager:Install via Unity Package Manager:



1. Open Unity Package Manager (`Window > Package Manager`)1. Open Unity Package Manager (`Window > Package Manager`)

2. Click `+` → `Add package from git URL`2. Click `+` → `Add package from git URL`

3. Enter: `https://github.com/cnem255/UnityPhysXFlow.git?path=/Packages/com.nvidia.physxflow`3. Enter: `https://github.com/YOUR-USERNAME/UnityPhysXFlow.git?path=/Packages/com.nvidia.physxflow`

4. Follow the [Installation Guide](Packages/com.nvidia.physxflow/INSTALLATION.md) to set up Flow runtime DLLs4. Install Flow runtime DLLs (see [Installation Guide](Packages/com.nvidia.physxflow/INSTALLATION.md))



### For DevelopersSee [complete installation guide](Packages/com.nvidia.physxflow/INSTALLATION.md) for details.



Clone and build from source:### For Developers



```powershellClone and build from source:

git clone --recursive https://github.com/cnem255/UnityPhysXFlow.git

cd UnityPhysXFlow```powershell

./build.ps1 -Configuration Releasegit clone --recursive https://github.com/YOUR-USERNAME/UnityPhysXFlow.git

```cd UnityPhysXFlow

./build.ps1 -Configuration Release

**Prerequisites:**```

- Windows with Visual Studio 2022 (MSVC 17+)

- CMake 3.20+ and Ninja in PATH**Prerequisites**:

- PhysX SDK (included as submodule)- Windows with Visual Studio 2022 (MSVC 17+)

- CMake 3.20+ and Ninja in PATH

---- PhysX SDK included as submodule



## 📖 Documentation## Build the Native Plugin

From the repo root:

### Getting Started```pwsh

- 📥 **[Installation Guide](Packages/com.nvidia.physxflow/INSTALLATION.md)** - Complete setup instructions for Unity./build.ps1 -Configuration Release

- 🎯 **[Quick Reference](docs/quick-reference.md)** - Fast API lookup and common patterns```

- 🏗️ **[Project Structure](docs/project-structure.md)** - Understanding the codebase organizationArtifacts go to `native/_build/Release/unity_physx_flow.dll` (or `_build/bin` for Ninja builds). Build artifacts also include the `.lib` import library.



### API & Integration## Unity Setup

- 📚 **[API Reference](docs/api-reference.md)** - Complete simulation API documentation1. **Copy the DLL** into your Unity project:

- 🎮 **[Unity Integration](docs/unity-integration.md)** - Components, inspectors, and Unity-specific features   - Place `unity_physx_flow.dll` under `Assets/Plugins/x86_64/` (Editor and standalone)

2. **Add the C# scripts**:

### Development & Deployment   - Copy `Unity/UnityPhysXFlow.cs` to `Assets/Plugins/UnityPhysXFlow/Runtime/`

- 🔧 **[Package Setup](docs/package-setup.md)** - How the Unity package is structured   - Copy `Unity/FlowEmitter.cs` and `Unity/FlowGrid.cs` to `Assets/Plugins/UnityPhysXFlow/Runtime/`

- 🚢 **[Deployment Guide](docs/deployment.md)** - Building and distributing the plugin   - Copy `Unity/Editor/FlowComponentEditors.cs` to `Assets/Plugins/UnityPhysXFlow/Editor/`

- 🐛 **[Flow Build Workaround](docs/flow-build-workaround.md)** - Troubleshooting Flow SDK builds3. **Add the shader**:

   - Copy `Unity/Shaders/VolumetricFluid.shader` to `Assets/Plugins/UnityPhysXFlow/Shaders/`

### Publishing & Maintenance4. **Deploy Flow runtime DLLs**:

- 📦 **[GitHub Publishing](docs/github-publishing.md)** - How to publish releases and manage versions   - Copy `nvflow.dll` and `nvflowext.dll` from `PhysX/flow/_build/windows-x86_64/release/` next to the Unity Editor executable (or your built game `.exe`)

- ✅ **[Unity Deployment Checklist](docs/unity-deployment-checklist.md)** - Pre-release validation steps   - *Alternative*: Call `UnityPhysXFlow.SetDllDirectory(path)` at startup to point at the Flow DLL folder

- 📝 **[Project Summary](docs/project-summary.md)** - Overview of implementation and architecture5. **Add the example component** `UnityPhysXFlowExample` to a scene and hit Play; the console should log per-frame flush messages with profiler JSON

- 📋 **[Implementation Notes](docs/implementation-notes.md)** - Technical decisions and patterns used

## Quick Start: Create a Fluid Scene

---

### Method 1: Using the Example Component (Automatic)

## 🎮 Usage Example1. Create an empty GameObject named "FlowSystem"

2. Add the `UnityPhysXFlowExample` component

```csharp3. Enable "Create Demo Scene"

using UnityPhysXFlow;4. Create a material using the `PhysXFlow/VolumetricFluid` shader

5. Assign the material to the `volumetricMaterial` field

public class FluidExample : MonoBehaviour6. Press Play!

{

    void Start()### Method 2: Manual Setup (More Control)

    {1. **Initialize UnityPhysXFlow**:

        // Initialize PhysX Flow   - Create a GameObject with `UnityPhysXFlowExample` component (or call `UnityPhysXFlow.Init()` in your own script)

        UnityPhysXFlow.Init((eventType, payload) => {

            Debug.Log($"Flow event: {payload}");2. **Create a Fluid Grid**:

        });   - Create a GameObject named "FluidGrid"

           - Add the `FlowGrid` component

        // Create a fluid emitter   - Set grid resolution (e.g., 64x64x64) and cell size (e.g., 0.1)

        int emitter = UnityPhysXFlow.CreateEmitter(   - Create a material using `PhysXFlow/VolumetricFluid` shader

            position: Vector3.zero,   - Assign the material to the `volumetricMaterial` field

            radius: 1.0f,

            density: 1.0f3. **Add Emitters**:

        );   - Create a GameObject named "FluidEmitter"

           - Add the `FlowEmitter` component

        // Create a simulation grid   - Position it above your grid

        int grid = UnityPhysXFlow.CreateGrid(   - Adjust radius and density as needed

            sizeX: 64, sizeY: 64, sizeZ: 64,

            cellSize: 0.1f4. **Press Play** and watch the fluid simulation!

        );

    }## API Overview

    

    void Update()### Core API

    {```csharp

        // Step the simulationusing UnityPhysXFlow;

        UnityPhysXFlow.Step(Time.deltaTime);

        // Initialize Flow and register event callback

        // Export density for renderingint rc = UnityPhysXFlow.Init((eventType, payload) => {

        Texture3D density = UnityPhysXFlow.ExportGridDensityAsTexture3D(gridHandle);    Debug.Log($"Flow event {eventType}: {payload}");

        material.SetTexture("_DensityTex", density);});

    }

}// Per-frame step

```UnityPhysXFlow.Step(Time.deltaTime);



See the [API Reference](docs/api-reference.md) for complete documentation.// Shutdown

UnityPhysXFlow.Shutdown();

---```



## 📊 Current Status### Simulation API

```csharp

- ✅ Native plugin with complete simulation API// Create emitter

- ✅ C# wrapper with Unity Texture3D conversionint emitterHandle = UnityPhysXFlow.CreateEmitter(Vector3.zero, radius: 1f, density: 1f);

- ✅ Unity components (`FlowEmitter`, `FlowGrid`) with custom inspectors

- ✅ Volumetric rendering shader for Unity// Update emitter

- ✅ Unity Package Manager structure with automatic updatesUnityPhysXFlow.SetEmitterParams(emitterHandle, newPosition, newRadius, newDensity);

- ✅ Build pipeline validated (Debug and Release)

- ⏳ Integrate actual Flow simulation (currently using placeholder data)// Create grid

- ⏳ NanoVDB export for sparse gridsint gridHandle = UnityPhysXFlow.CreateGrid(64, 64, 64, cellSize: 0.1f);

- ⏳ HDRP/URP volumetric fog integration

// Export for rendering

---Texture3D densityTex = UnityPhysXFlow.ExportGridDensityAsTexture3D(gridHandle);

myMaterial.SetTexture("_DensityTex", densityTex);

## 🤝 Contributing

// Cleanup

Contributions are welcome! Please feel free to submit a Pull Request.UnityPhysXFlow.DestroyEmitter(emitterHandle);

UnityPhysXFlow.DestroyGrid(gridHandle);

---```



## 📄 LicenseFor detailed API documentation, see [SIMULATION_API.md](SIMULATION_API.md).



This bridge code is licensed under the MIT License. See [LICENSE](Packages/com.nvidia.physxflow/LICENSE.md) for details.## Rendering Fluids in Unity



NVIDIA PhysX/Flow SDKs retain their respective licenses.### Built-in Volumetric Shader

The included `VolumetricFluid.shader` provides raymarched volumetric rendering:

---

**Shader Properties:**

## 🔗 Links- `_DensityTex`: Density 3D texture (auto-set by FlowGrid)

- `_VelocityTex`: Velocity 3D texture (auto-set by FlowGrid)

- [Unity Package](Packages/com.nvidia.physxflow/) - The distributable Unity package- `_DensityScale`: Visibility multiplier

- [NVIDIA PhysX](https://github.com/NVIDIA-Omniverse/PhysX) - Official PhysX SDK- `_Color`: Fluid color tint

- [NVIDIA Flow Documentation](https://docs.omniverse.nvidia.com/flow/latest/index.html) - Official Flow docs- `_StepSize`: Ray marching step size (quality vs performance)

- `_MaxSteps`: Maximum ray marching iterations

---

**Recommended Settings:**

**Made with ❤️ for the Unity community**- Resolution: Start with 64³ grids for real-time performance

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
