# Deployment Guide: UnityPhysXFlow

## ✅ Build Complete!

All components have been successfully built:

### Native Components
| Component | Location | Configuration |
|-----------|----------|---------------|
| **Unity Bridge DLL** | `native/_build/Release/unity_physx_flow.dll` | Release (optimized) |
| **Flow Core** | `PhysX/flow/_build/windows-x86_64/release/nvflow.dll` | Release |
| **Flow Extensions** | `PhysX/flow/_build/windows-x86_64/release/nvflowext.dll` | Release |
| **Flow RTX** (optional) | `PhysX/flow/_build/windows-x86_64/release/nvflow_rtx.dll` | Release |
| **Slang Compiler** | `PhysX/flow/_build/windows-x86_64/release/slang*.dll` | Release |

### Unity Components
| Component | Location | Description |
|-----------|----------|-------------|
| **C# Wrapper** | `Unity/UnityPhysXFlow.cs` | Managed API with Upf_* functions |

---

## 📦 Unity Integration Steps

### 1. Copy Native DLLs to Unity Project

Create these folders in your Unity project:
```
YourUnityProject/
├── Assets/
│   └── Plugins/
│       └── x86_64/           # Native DLLs for Windows x64
```

Copy the following files to `Assets/Plugins/x86_64/`:

**Required:**
- `native/_build/Release/unity_physx_flow.dll` (Your bridge)
- `PhysX/flow/_build/windows-x86_64/release/nvflow.dll` (Flow core)
- `PhysX/flow/_build/windows-x86_64/release/nvflowext.dll` (Flow extensions)
- `PhysX/flow/_build/windows-x86_64/release/slang.dll` (Slang shader compiler)
- `PhysX/flow/_build/windows-x86_64/release/slang-glslang.dll` (Slang dependency)

**Optional** (for ray-traced rendering):
- `PhysX/flow/_build/windows-x86_64/release/nvflow_rtx.dll`
- `PhysX/flow/_build/windows-x86_64/release/nvflowext_rtx.dll`

### 2. Add C# Wrapper to Unity

Copy `Unity/UnityPhysXFlow.cs` to:
```
YourUnityProject/Assets/Scripts/UnityPhysXFlow.cs
```

### 3. Quick Test

Create a new C# script in Unity:

```csharp
using UnityEngine;
using UnityPhysXFlow;

public class FlowTest : MonoBehaviour
{
    void Start()
    {
        // Initialize Flow
        int result = UnityPhysXFlow.Init((eventType, payload) =>
        {
            Debug.Log($"[Flow Event {eventType}] {payload}");
        });

        if (result == 0)
        {
            Debug.Log("✅ UnityPhysXFlow initialized successfully!");
        }
        else
        {
            Debug.LogError($"❌ UnityPhysXFlow initialization failed: {result}");
        }
    }

    void Update()
    {
        // Step simulation each frame
        UnityPhysXFlow.Step(Time.deltaTime);
    }

    void OnDestroy()
    {
        // Clean up on exit
        UnityPhysXFlow.Shutdown();
    }
}
```

Attach this script to any GameObject and press Play. You should see:
- Initialization success message
- Per-frame flush events in console
- Profiler JSON batches (event type 2)

---

## 🛠️ Troubleshooting

### DLL Not Found Errors

If Unity can't find `nvflow.dll` or `nvflowext.dll`:

**Option A:** Call `SetDllDirectory` before `Init()`:
```csharp
string flowDllPath = Path.Combine(Application.dataPath, "Plugins/x86_64");
UnityPhysXFlow.SetDllDirectory(flowDllPath);
```

**Option B:** Copy ALL DLLs next to the Unity Editor executable (`Unity.exe`):
- Useful for Editor testing
- Not needed for standalone builds

### Missing slang.dll

If you see errors about missing `slang.dll`:
- Copy `slang.dll` and `slang-glslang.dll` from Flow build folder
- These are shader compiler dependencies required by Flow

### Platform-Specific Issues

**Windows Only:**
- Current build is x64 Windows only
- Requires MSVC runtime (usually already installed)

**For Standalone Builds:**
- Copy all DLLs next to your built .exe
- Unity will automatically package `Assets/Plugins/x86_64/` content

---

## 🎯 Next Steps: Rendering Fluids

Now that the bridge is working, you can:

1. **Add Simulation API** (next phase):
   - Expose `NvFlowGrid` creation
   - Add emitter functions (box, sphere, mesh)
   - Control simulation params from C#

2. **Export Volume Data for Rendering**:
   - Use `NvFlowSparseNanoVdbExport` to get density/velocity
   - Create Unity `ComputeBuffer` from exported data
   - Write Unity shader to raymarch the volumes

3. **Integrate with Unity Render Pipeline**:
   - HDRP: Custom pass for volume rendering
   - URP: Renderer Feature for post-process raymarch
   - Built-in: Command buffer injection

---

## 📊 API Event Types

The callback receives these event types:

| Event Type | Description | Payload Format |
|------------|-------------|----------------|
| `0` | Simulation step info | `"flushedFrame=123, dt=0.016"` |
| `1` | Error message | Error text string |
| `2` | Profiler batch | JSON: `{"captureId":N,"entries":[...]}` |
| `99` | Test event | Custom test message |

---

## 🔧 Development Workflow

### Rebuilding the Bridge

When you modify the bridge code:

```pwsh
cd path/to/UnityPhysXFlow
.\build.ps1 -Configuration Release
```

Then copy the updated `unity_physx_flow.dll` to Unity's `Assets/Plugins/x86_64/`.

### Rebuilding Flow SDK

If you update Flow source (not usually needed):

```pwsh
cd PhysX/flow
.\build.bat
```

This regenerates all Flow DLLs.

---

## 📝 Notes

- **Performance**: Use Release builds for best performance
  - Debug builds are ~2x larger and much slower
  - Release DLLs are in `release/` folders

- **Vulkan Backend**: The bridge uses Vulkan for Flow
  - Self-contained, doesn't need Unity's graphics device
  - Works alongside Unity's D3D11/D3D12 renderer

- **Thread Safety**: Events fire from Flow's thread
  - C# wrapper dispatches to Unity main thread automatically
  - Safe to call Unity API in event callback

---

## ✅ Build Status Summary

```
✅ Flow SDK (Debug + Release)
   - nvflow.dll (1.97 MB Release)
   - nvflowext.dll (9.89 MB Release)
   - nvflow_rtx.dll + nvflowext_rtx.dll (optional)

✅ Unity Bridge
   - unity_physx_flow.dll (native)
   - UnityPhysXFlow.cs (managed wrapper)

✅ Build System
   - CMake + VS2022 (native bridge)
   - Premake5 + MSBuild (Flow SDK)
   - PowerShell automation (build.ps1)

🔲 Next Phase: Simulation API & Rendering
```

---

## 🆘 Support

Issues? Check:
1. **Build errors**: See `FLOW_BUILD_WORKAROUND.md` for Flow SDK fixes
2. **Runtime errors**: Enable Unity console to see event payloads
3. **Missing features**: Current API is minimal; rendering/simulation coming next

Happy fluid simulating! 🌊
