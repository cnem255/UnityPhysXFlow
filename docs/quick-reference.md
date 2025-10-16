# UnityPhysXFlow Quick Reference

## Setup (One-Time)

### Option 1: Using the Wizard (Easiest)
1. **Menu**: `Tools > PhysX Flow > Setup Wizard`
2. Set DLL path (optional)
3. Check "Create Demo Scene"
4. Click "Create Flow Setup"
5. Press Play!

### Option 2: Menu Items
1. `GameObject > PhysX Flow > Create Complete Scene`
2. Assign volumetric material to FlowGrid
3. Press Play!

### Option 3: Manual
1. Create GameObject → Add `UnityPhysXFlowExample`
2. Create GameObject → Add `FlowGrid` → Assign material
3. Create GameObject → Add `FlowEmitter` → Position above grid
4. Press Play!

---

## Components

### FlowEmitter
**Purpose**: Spawns fluid particles  
**Key Properties**:
- `radius`: Size of emission area (0.1-10)
- `density`: Particle density (0.1-10)
- `autoCreate`: Auto-initialize on Start

**Transform**: Position synced every frame

### FlowGrid
**Purpose**: Simulation domain + rendering  
**Key Properties**:
- `sizeX/Y/Z`: Grid resolution (32-128 recommended)
- `cellSize`: World-space cell size (0.01-1.0)
- `volumetricMaterial`: Material with VolumetricFluid shader
- `updateInterval`: Texture refresh rate (1-5 frames)

**Tips**:
- Start with 64³ grid for good performance
- Higher resolution = better detail, slower
- Increase updateInterval for performance

---

## Shader: PhysXFlow/VolumetricFluid

### Key Properties
| Property | Type | Recommended | Purpose |
|----------|------|-------------|---------|
| `_DensityTex` | 3D Texture | Auto-set | Fluid density |
| `_VelocityTex` | 3D Texture | Auto-set | Fluid motion |
| `_DensityScale` | Float | 1.0 | Visibility multiplier |
| `_Color` | Color | (0.2, 0.5, 1.0) | Fluid tint |
| `_StepSize` | Float | 0.01 | Ray quality (lower=better) |
| `_MaxSteps` | Int | 128 | Max iterations (higher=slower) |

### Performance Tuning
- **Fast**: `_StepSize=0.02`, `_MaxSteps=64`
- **Balanced**: `_StepSize=0.01`, `_MaxSteps=128` (default)
- **Quality**: `_StepSize=0.005`, `_MaxSteps=256`

---

## API Quick Reference

```csharp
using UnityPhysXFlow;

// Initialization (once)
UnityPhysXFlow.Init((evt, payload) => Debug.Log(payload));

// Create emitter
int emitter = UnityPhysXFlow.CreateEmitter(
    position: Vector3.zero,
    radius: 1f,
    density: 1f
);

// Update emitter
UnityPhysXFlow.SetEmitterParams(emitter, newPos, newRadius, newDensity);

// Create grid
int grid = UnityPhysXFlow.CreateGrid(64, 64, 64, cellSize: 0.1f);

// Export textures (in Update)
Texture3D density = UnityPhysXFlow.ExportGridDensityAsTexture3D(grid);
material.SetTexture("_DensityTex", density);

// Cleanup
UnityPhysXFlow.DestroyEmitter(emitter);
UnityPhysXFlow.DestroyGrid(grid);
UnityPhysXFlow.Shutdown();
```

---

## Troubleshooting

### "Init failed with code -1"
- **Cause**: nvflow.dll/nvflowext.dll not found
- **Fix**: Set `flowDllPath` in UnityPhysXFlowExample, or copy DLLs next to Unity.exe

### No fluid visible
- **Check**:
  - Is volumetric material assigned to FlowGrid?
  - Is emitter positioned above/inside grid?
  - Is `_DensityScale` > 0?
  - Is grid large enough? (Try 128³)

### Low performance
- **Reduce**: Grid resolution (64³ → 32³)
- **Increase**: `updateInterval` (0 → 3)
- **Adjust**: `_StepSize` (0.01 → 0.02)
- **Reduce**: `_MaxSteps` (128 → 64)

### Fluid looks blocky
- **Increase**: Grid resolution (64³ → 128³)
- **Decrease**: `_StepSize` (0.01 → 0.005)
- **Increase**: `_MaxSteps` (128 → 256)

### DLL not loading in build
- Copy `nvflow.dll` and `nvflowext.dll` to build folder (next to .exe)
- Or use `UnityPhysXFlow.SetDllDirectory()` to specify path at runtime

---

## Example Scenes

### Basic Emitter
```
FlowSystem (UnityPhysXFlowExample)
  └─ FlowGrid (64³, cellSize=0.1)
  └─ FlowEmitter (pos=[0,3,0], radius=1)
```

### Multiple Emitters
```
FlowSystem
  └─ FlowGrid (128³, cellSize=0.05)
  └─ Emitter1 (pos=[-2,3,0])
  └─ Emitter2 (pos=[2,3,0])
  └─ Emitter3 (pos=[0,5,0])
```

### Dynamic Emitter
```csharp
public class MovingEmitter : MonoBehaviour
{
    private FlowEmitter emitter;
    
    void Start()
    {
        emitter = GetComponent<FlowEmitter>();
    }
    
    void Update()
    {
        // Orbit around origin
        float angle = Time.time;
        transform.position = new Vector3(
            Mathf.Cos(angle) * 3f,
            3f,
            Mathf.Sin(angle) * 3f
        );
        
        // Pulse density
        emitter.density = 1f + Mathf.Sin(Time.time * 2f) * 0.5f;
    }
}
```

---

## Menu Reference

### GameObject Menu
- `PhysX Flow > Create Flow System`
- `PhysX Flow > Create Flow Emitter`
- `PhysX Flow > Create Flow Grid`
- `PhysX Flow > Create Complete Scene`

### Assets Menu
- `Create > PhysX Flow > Volumetric Material`

### Tools Menu
- `PhysX Flow > Setup Wizard`
- `PhysX Flow > Documentation`

---

## Performance Guidelines

| Grid Size | Recommended For | FPS (approx) |
|-----------|-----------------|--------------|
| 32³ | Low-end, mobile | 60+ |
| 64³ | Balanced | 30-60 |
| 128³ | High-end | 15-30 |
| 256³ | Offline/cinematic | <15 |

*Based on GTX 1060, single emitter, default shader settings*

---

## Full Documentation

- **README.md**: Project overview and setup
- **SIMULATION_API.md**: Complete API reference
- **UNITY_INTEGRATION_SUMMARY.md**: Technical implementation details
- **DEPLOYMENT.md**: Build and deployment guide

Access via: `Tools > PhysX Flow > Documentation`
