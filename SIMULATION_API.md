# UnityPhysXFlow Simulation API

This document describes the simulation control API for UnityPhysXFlow, which allows you to create and control PhysX Flow emitters and grids from Unity.

## Overview

The UnityPhysXFlow bridge exposes:
- **Emitters**: Spawn fluid particles at specific positions with configurable radius and density.
- **Grids**: Manage simulation grids with custom resolution and cell size.
- **Rendering**: Export grid data (density/velocity) as Unity Texture3D for volumetric rendering.

## C# API

### Core Functions

```csharp
// Initialize the bridge (call once at startup)
int UnityPhysXFlow.Init(Action<int, string> onEvent);

// Step simulation forward
void UnityPhysXFlow.Step(float deltaTime);

// Shutdown and cleanup
void UnityPhysXFlow.Shutdown();
```

### Emitter API

```csharp
// Create a new emitter at position with radius and density
int UnityPhysXFlow.CreateEmitter(Vector3 position, float radius, float density);

// Update emitter parameters
void UnityPhysXFlow.SetEmitterParams(int emitterHandle, Vector3 position, float radius, float density);

// Destroy an emitter
void UnityPhysXFlow.DestroyEmitter(int emitterHandle);
```

### Grid API

```csharp
// Create a simulation grid
int UnityPhysXFlow.CreateGrid(int sizeX, int sizeY, int sizeZ, float cellSize);

// Step a specific grid (optional, otherwise use global Step)
void UnityPhysXFlow.StepGrid(int gridHandle, float deltaTime);

// Export grid density as Texture3D for rendering
Texture3D UnityPhysXFlow.ExportGridDensityAsTexture3D(int gridHandle);

// Export grid velocity as Texture3D
Texture3D UnityPhysXFlow.ExportGridVelocityAsTexture3D(int gridHandle);

// Destroy a grid
void UnityPhysXFlow.DestroyGrid(int gridHandle);
```

## Unity Components

### FlowEmitter Component

Add this component to any GameObject to turn it into a fluid emitter.

**Inspector Properties:**
- `radius`: Size of the emitter sphere (0.1-10)
- `density`: Density of emitted fluid (0.1-10)
- `autoCreate`: Auto-create emitter on Start

**Usage:**
```csharp
// Add component in code
FlowEmitter emitter = gameObject.AddComponent<FlowEmitter>();
emitter.radius = 1.5f;
emitter.density = 2.0f;
emitter.CreateEmitter();

// Update at runtime
emitter.transform.position = newPosition; // Synced automatically in Update
```

### FlowGrid Component

Add this component to manage a simulation grid and render fluids volumetrically.

**Inspector Properties:**
- `sizeX/Y/Z`: Grid resolution (e.g., 64x64x64)
- `cellSize`: Cell size in world units (0.01-1.0)
- `volumetricMaterial`: Material for rendering (use VolumetricFluid shader)
- `updateInterval`: Update textures every N frames (0 = every frame)
- `autoCreate`: Auto-create grid on Start

**Usage:**
```csharp
FlowGrid grid = gameObject.AddComponent<FlowGrid>();
grid.sizeX = grid.sizeY = grid.sizeZ = 64;
grid.cellSize = 0.1f;
grid.volumetricMaterial = myVolumetricMaterial;
grid.CreateGrid();
```

## Volumetric Rendering

### VolumetricFluid Shader

The included `VolumetricFluid.shader` provides basic volumetric rendering for Flow grids.

**Shader Properties:**
- `_DensityTex`: Density 3D texture (auto-set by FlowGrid)
- `_VelocityTex`: Velocity 3D texture (auto-set by FlowGrid)
- `_DensityScale`: Density multiplier for visibility
- `_Color`: Fluid color tint
- `_StepSize`: Ray marching step size (smaller = higher quality, slower)
- `_MaxSteps`: Maximum ray marching steps

**Setup:**
1. Create a material using the `PhysXFlow/VolumetricFluid` shader.
2. Assign this material to the `volumetricMaterial` field of your FlowGrid component.
3. The FlowGrid will automatically update `_DensityTex` and `_VelocityTex` each frame.

**Example Material Settings:**
- `_DensityScale`: 1.0
- `_Color`: (0.2, 0.5, 1.0, 1.0) - light blue
- `_StepSize`: 0.01
- `_MaxSteps`: 128

## Example Scene Setup

1. **Create a new GameObject** named "FlowSystem"
2. **Add UnityPhysXFlowExample** component to initialize the bridge
3. **Create an emitter GameObject:**
   - Add FlowEmitter component
   - Set radius = 1.0, density = 1.0
   - Position above your scene
4. **Create a grid GameObject:**
   - Add FlowGrid component
   - Set size = 64x64x64, cellSize = 0.1
   - Create a material using VolumetricFluid shader
   - Assign material to volumetricMaterial field
5. **Press Play** - you should see fluid particles being simulated and rendered!

## Advanced Usage

### Manual API Control

For advanced control without components:

```csharp
// Initialize
UnityPhysXFlow.Init((evt, payload) => Debug.Log($"Event {evt}: {payload}"));

// Create grid
int gridHandle = UnityPhysXFlow.CreateGrid(64, 64, 64, 0.1f);

// Create emitter
int emitterHandle = UnityPhysXFlow.CreateEmitter(Vector3.zero, 1f, 1f);

// Game loop
void Update()
{
    // Step simulation
    UnityPhysXFlow.StepGrid(gridHandle, Time.deltaTime);
    
    // Export textures for rendering
    Texture3D density = UnityPhysXFlow.ExportGridDensityAsTexture3D(gridHandle);
    myMaterial.SetTexture("_DensityTex", density);
}

// Cleanup
void OnDestroy()
{
    UnityPhysXFlow.DestroyEmitter(emitterHandle);
    UnityPhysXFlow.DestroyGrid(gridHandle);
    UnityPhysXFlow.Shutdown();
}
```

## Performance Tips

1. **Grid Resolution**: Start with 32x32x32 or 64x64x64 for real-time performance.
2. **Update Interval**: Set `FlowGrid.updateInterval` to 2-5 to reduce texture upload overhead.
3. **Ray Marching**: Adjust `_StepSize` and `_MaxSteps` for quality/performance balance.
4. **Multiple Grids**: You can have multiple grids with different resolutions for LOD.

## TODO / Future Features

- [ ] Integrate actual Flow simulation (currently using placeholder data)
- [ ] NanoVDB export for more efficient sparse grid representation
- [ ] HDRP/URP volumetric fog integration
- [ ] Advanced emitter types (directed jets, explosions)
- [ ] Collision and boundary conditions
- [ ] Multi-GPU support
