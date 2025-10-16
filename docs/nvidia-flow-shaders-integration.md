# NVIDIA Flow Shaders Integration Guide

## Overview

Instead of writing custom raymarching shaders from scratch, we should leverage the **official NVIDIA Flow raymarching shaders** included in the PhysX Flow SDK. These are production-quality, optimized shaders used by NVIDIA's Flow demos.

## Available Flow Shaders

Located in `PhysX/flow/include/nvflowext/shaders/`:

### Core Raymarching Files

1. **`NvFlowRayMarchParams.h`**
   - Defines shader parameters structures
   - `NvFlowRayMarchLayerShaderParams` - Layer rendering parameters
   - `NvFlowRayMarchCloudLayerShaderParams` - Cloud-like fluid rendering
   - `NvFlowRayMarchIsosurfaceLayerShaderParams` - Isosurface rendering
   - `NvFlowRayMarchShaderParams` - Main shader parameters

2. **`NvFlowRayMarchCommon.hlsli`**
   - Common utilities for raymarching
   - Box intersection tests
   - Ray advancement algorithms
   - Hash functions for noise
   - Optimized for sparse voxel grids

3. **`NvFlowRayMarch.hlsli`** (706 lines!)
   - Full volumetric raymarching implementation
   - Supports cloud rendering mode
   - Block wireframe visualization
   - Density sampling with sparse grids
   - Color mapping support
   - Shadow support
   - Optimized step size calculation

4. **`NvFlowRayMarchIsosurface.hlsli`**
   - Isosurface extraction and rendering
   - Normal computation
   - Refraction support
   - Physically-based shading

5. **`NvFlowRayMarchUtils.h`**
   - Additional utility functions

## Why Use NVIDIA's Shaders?

### ✅ Advantages

1. **Production Quality**
   - Written by NVIDIA engineers
   - Used in official Flow demos
   - Heavily tested and optimized

2. **Sparse Grid Optimization**
   - Designed specifically for Flow's sparse voxel grids
   - Block-level culling
   - Efficient empty space skipping

3. **Advanced Features**
   - Cloud-like rendering
   - Isosurface rendering
   - Self-shadowing
   - Physical lighting
   - Color mapping
   - Multiple rendering modes

4. **Performance**
   - Optimized for NVIDIA GPUs
   - Proper loop unrolling hints
   - Efficient texture sampling
   - Minimal divergence

5. **Compatibility**
   - Works with NanoVDB export
   - Integrates with Flow's sparse structure
   - Supports multiple LOD levels

### ❌ Our Current Simple Shader Issues

- Simple fixed-step raymarching (inefficient)
- No sparse grid optimization
- No shadow support
- Basic lighting only
- Compiler loop unrolling issues
- Not designed for Flow's data structures

## Integration Strategy

### Phase 1: Direct Port (Current Approach)

**Status**: ❌ Not ideal - reinventing the wheel

Our current `VolumetricFluid.shader`:
- Simple raymarching from scratch
- Doesn't leverage Flow's sparse structure
- Missing advanced features
- Compiler warnings (loop unrolling)

### Phase 2: Wrap NVIDIA Shaders (Recommended)

**Status**: 🎯 Should implement this

Convert NVIDIA's HLSL shaders to Unity CG/HLSL:

```csharp
Shader "PhysXFlow/VolumetricFluid"
{
    CGPROGRAM
    // Include NVIDIA Flow shader headers
    #include "Packages/com.nvidia.physxflow/Shaders/NvFlow/NvFlowRayMarchCommon.cginc"
    
    // Adapt parameters to Unity's conventions
    // Call NVIDIA's raymarching functions
    ENDCG
}
```

**Steps**:
1. Copy relevant `.hlsli` files to package
2. Convert HLSL to Unity-compatible CG/HLSL
3. Adapt parameter structures
4. Create Unity-friendly wrapper functions
5. Test with Flow's exported grids

### Phase 3: Compute Shader Approach (Advanced)

**Status**: 🚀 Future optimization

Use Unity Compute Shaders with NVIDIA's code:
- Deferred rendering approach
- Full access to HLSL features
- Better performance
- More flexibility

## Implementation Plan

### Step 1: Copy NVIDIA Shader Files

```
Packages/com.nvidia.physxflow/Shaders/
├── NvFlow/
│   ├── NvFlowRayMarchParams.h
│   ├── NvFlowRayMarchCommon.hlsli
│   ├── NvFlowRayMarch.hlsli
│   ├── NvFlowRayMarchIsosurface.hlsli
│   └── NvFlowShader.hlsli (from nvflow/shaders/)
└── VolumetricFluid.shader (Unity wrapper)
```

### Step 2: Convert to Unity-Compatible Format

**Challenges**:
- Unity CG uses slightly different syntax than pure HLSL
- Need to bridge NVIDIA's parameter structures with Unity uniforms
- Texture binding differences
- Constant buffer alignment

**Solutions**:
- Create adapter layer
- Use Unity's `UNITY_SAMPLE_TEX3D` macros
- Map NVIDIA structs to Unity uniforms
- Test incrementally

### Step 3: Create Unity Wrapper Shader

```hlsl
Shader "PhysXFlow/VolumetricFluidNVIDIA"
{
    Properties
    {
        // Expose NVIDIA parameters as Unity properties
        _BlockSizeWorld ("Block Size World", Vector) = (1,1,1,0)
        _CellSize ("Cell Size", Vector) = (0.1,0.1,0.1,0)
        _StepSize ("Step Size", Float) = 0.01
        // ... more parameters
    }
    
    SubShader
    {
        Pass
        {
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #pragma target 5.0
            
            // Include NVIDIA Flow shaders
            #include "NvFlow/NvFlowRayMarchCommon.hlsli"
            #include "NvFlow/NvFlowRayMarch.hlsli"
            
            // Bridge Unity to NVIDIA
            struct v2f
            {
                float4 pos : SV_POSITION;
                float3 worldPos : TEXCOORD0;
                float3 rayDir : TEXCOORD1;
            };
            
            // Convert Unity textures to NVIDIA format
            Texture3D<float4> _DensityTexture;
            SamplerState sampler_DensityTexture;
            
            v2f vert(appdata_base v)
            {
                // Setup for NVIDIA raymarching
            }
            
            float4 frag(v2f i) : SV_Target
            {
                // Call NVIDIA's NvFlowRayMarchBlock function
                // Return raymarched result
            }
            
            ENDCG
        }
    }
}
```

### Step 4: C# API Integration

Update `FlowGrid.cs` to export Flow grids in NVIDIA-compatible format:

```csharp
public class FlowGrid : MonoBehaviour
{
    // Export grid data in format expected by NVIDIA shaders
    private NvFlowSparseLevelParams GetLevelParams()
    {
        // Bridge Unity data to NVIDIA parameter structure
    }
    
    // Set up material with NVIDIA shader parameters
    private void UpdateMaterialForNVIDIAShader()
    {
        if (volumetricMaterial != null)
        {
            // Set block size, cell size, etc.
            volumetricMaterial.SetVector("_BlockSizeWorld", ...);
            volumetricMaterial.SetVector("_CellSize", ...);
            // ... etc
        }
    }
}
```

## Benefits of This Approach

### Performance

- **10-100x faster** for sparse grids (blocks with no data are skipped)
- Optimized texture fetching
- Better cache coherency
- GPU-specific optimizations

### Quality

- **Professional lighting** (ambient, diffuse, specular)
- **Self-shadowing** (optional, performance cost)
- **Cloud mode** for soft, atmospheric fluids
- **Isosurface mode** for liquid-like surfaces
- **Color mapping** for temperature/velocity visualization

### Maintenance

- NVIDIA maintains and updates these shaders
- Bug fixes and optimizations come from NVIDIA
- Proven in production environments
- Less code for us to maintain

## Licensing

The Flow shaders are **BSD-3-Clause** licensed (same as PhysX SDK):
- ✅ Can use in commercial projects
- ✅ Can modify
- ✅ Must include copyright notice
- ✅ Must include license text

See shader headers for full license text.

## Next Steps

1. **Copy NVIDIA shader files** to package
2. **Create Unity adapter layer** for parameter structures
3. **Write Unity wrapper shader** using NVIDIA's functions
4. **Test with exported Flow grids**
5. **Document usage** for end users
6. **Replace simple shader** with NVIDIA-powered version

## Alternative: Compute Shader Pipeline

For maximum performance and flexibility:

1. **Raymarch in Compute Shader**
   - Use NVIDIA's HLSL code directly
   - No CG compatibility layer needed
   - Full HLSL feature set

2. **Output to RenderTexture**
   - Deferred rendering approach
   - Compositing with scene

3. **Benefits**:
   - Best performance
   - Access to all HLSL features
   - Easier to use NVIDIA code unchanged

## References

- NVIDIA Flow SDK: `PhysX/flow/`
- Flow Shaders: `PhysX/flow/include/nvflowext/shaders/`
- Flow Demos: `PhysX/flow/demo/` (shows shader usage)
- NanoVDB: For sparse volume export (future integration)

## Conclusion

**We should use NVIDIA's official shaders instead of writing our own.**

This gives us:
- ✅ Better performance
- ✅ Higher quality rendering
- ✅ Professional features
- ✅ Less maintenance
- ✅ Proven reliability

The integration work is worth it for the significant benefits.

---

**Status**: 📝 Documentation complete - implementation needed  
**Priority**: High - current shader has compiler warnings  
**Effort**: Medium - requires HLSL/CG porting and testing  
**Impact**: Major - significantly better rendering quality and performance
