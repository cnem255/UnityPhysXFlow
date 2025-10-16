# Unity Deployment Checklist

Use this checklist to integrate UnityPhysXFlow into your Unity project.

## Prerequisites

- [ ] Built `unity_physx_flow.dll` (Release configuration)
- [ ] Built PhysX Flow SDK (`nvflow.dll` and `nvflowext.dll` available)
- [ ] Unity 2021.3 or later (tested on 2021.3 LTS)
- [ ] Unity Editor on Windows x64

---

## Step 1: Copy Native DLL

- [ ] Locate `native/_build/Release/unity_physx_flow.dll`
- [ ] Create folder in Unity: `Assets/Plugins/UnityPhysXFlow/x86_64/`
- [ ] Copy `unity_physx_flow.dll` to this folder
- [ ] **Verify** in Unity: Check that DLL appears in Project window

### Platform Settings (Optional)
In Unity, select `unity_physx_flow.dll` in Project window:
- [ ] Set **Platform**: Windows
- [ ] Set **CPU**: x86_64
- [ ] Set **Load on startup**: ✓ (checked)

---

## Step 2: Copy C# Scripts

### Runtime Scripts
Create folder: `Assets/Plugins/UnityPhysXFlow/Runtime/`

Copy these files from `Unity/`:
- [ ] `UnityPhysXFlow.cs`
- [ ] `FlowEmitter.cs`
- [ ] `FlowGrid.cs`

### Editor Scripts
Create folder: `Assets/Plugins/UnityPhysXFlow/Editor/`

Copy these files from `Unity/Editor/`:
- [ ] `FlowComponentEditors.cs`
- [ ] `FlowSetupMenu.cs`

---

## Step 3: Copy Shader

Create folder: `Assets/Plugins/UnityPhysXFlow/Shaders/`

- [ ] Copy `Unity/Shaders/VolumetricFluid.shader`
- [ ] **Verify** in Unity: Shader should appear under `PhysXFlow/VolumetricFluid`

---

## Step 4: Deploy Flow Runtime DLLs

Choose **ONE** of these options:

### Option A: Next to Unity Editor (Easiest for development)
- [ ] Locate PhysX Flow DLLs at: `PhysX/flow/_build/windows-x86_64/release/`
- [ ] Copy `nvflow.dll` to Unity Editor folder (e.g., `C:\Program Files\Unity\Hub\Editor\2021.3.x\Editor\`)
- [ ] Copy `nvflowext.dll` to Unity Editor folder
- [ ] Copy any other DLLs in that folder (e.g., `slang.dll`)

### Option B: Specify Path at Runtime (Flexible)
- [ ] Keep Flow DLLs in `PhysX/flow/_build/windows-x86_64/release/`
- [ ] In Unity scene, select FlowSystem GameObject
- [ ] Set `flowDllPath` to the full path of the release folder
- [ ] Example: `C:\Projects\UnityPhysXFlow\PhysX\flow\_build\windows-x86_64\release`

### Option C: Copy to Unity Project (For builds)
- [ ] Create folder: `Assets/Plugins/UnityPhysXFlow/x86_64/`
- [ ] Copy `nvflow.dll`, `nvflowext.dll`, and dependencies to this folder
- [ ] In Unity, select each DLL and configure Platform Settings (Windows x64)

---

## Step 5: Create Initial Scene

### Using Setup Wizard (Recommended)
- [ ] Open Unity
- [ ] Menu: `Tools > PhysX Flow > Setup Wizard`
- [ ] Set DLL path (if using Option B above)
- [ ] Check "Create Demo Scene"
- [ ] Click "Create Flow Setup"
- [ ] Material is auto-created, assign it to FlowGrid

### Manual Setup
- [ ] Menu: `GameObject > PhysX Flow > Create Complete Scene`
- [ ] Menu: `Assets > Create > PhysX Flow > Volumetric Material`
- [ ] Select FlowGrid in Hierarchy
- [ ] Drag material to `volumetricMaterial` field

---

## Step 6: Test Integration

- [ ] Press Play in Unity
- [ ] **Verify Console** shows: `[UnityPhysXFlow] Initialized successfully`
- [ ] **Verify Console** shows: `[FlowGrid] Created grid X`
- [ ] **Verify Console** shows: `[FlowEmitter] Created emitter X`
- [ ] No errors in Console

### Troubleshooting First Run
If you see errors, check:
- [ ] Console shows "Init failed with code -1" → Flow DLLs not found (revisit Step 4)
- [ ] Console shows "Failed to create grid" → DLL loaded but simulation issue
- [ ] No errors but no visuals → Material not assigned or shader missing

---

## Step 7: Verify Rendering

- [ ] Grid wireframe gizmo visible in Scene view (green box)
- [ ] Emitter gizmo visible in Scene view (cyan sphere)
- [ ] Fluid **should** be visible in Game view (currently placeholder data, so may be faint)

### If Fluid Not Visible
- [ ] Check FlowGrid has `volumetricMaterial` assigned
- [ ] Check material uses `PhysXFlow/VolumetricFluid` shader
- [ ] Increase material's `_DensityScale` to 10 for testing
- [ ] Try larger grid (128x128x128)

---

## Step 8: Build for Standalone (Optional)

### Build Settings
- [ ] Menu: `File > Build Settings`
- [ ] Platform: Windows
- [ ] Architecture: x86_64
- [ ] Click "Build" or "Build And Run"

### Post-Build
- [ ] Navigate to build folder (e.g., `Builds/MyGame/`)
- [ ] Copy Flow DLLs to same folder as .exe:
  - [ ] `nvflow.dll`
  - [ ] `nvflowext.dll`
  - [ ] Any dependencies (check `PhysX/flow/_build/.../release/`)
- [ ] Run `.exe` and verify fluids work

---

## Step 9: Customize and Extend

Now that basic integration works, customize:

### Scene Setup
- [ ] Adjust grid resolution for your target hardware
- [ ] Position emitters for your gameplay
- [ ] Experiment with shader settings (`_Color`, `_StepSize`, etc.)

### Performance Tuning
- [ ] Set FlowGrid `updateInterval` to 2-5 for better FPS
- [ ] Reduce grid resolution if FPS is low
- [ ] Adjust shader `_MaxSteps` for quality/performance balance

### Advanced
- [ ] Create custom materials with VolumetricFluid shader
- [ ] Script dynamic emitter control (see QUICK_REFERENCE.md)
- [ ] Multiple grids for LOD or multi-zone simulation

---

## Verification Summary

✅ **All checks passed?** Your UnityPhysXFlow integration is complete!

If you encounter issues, refer to:
- **QUICK_REFERENCE.md**: Common troubleshooting
- **SIMULATION_API.md**: Full API reference
- **README.md**: Project overview

---

## Known Limitations (Current Version)

⚠️ **Placeholder Simulation**: The current implementation uses placeholder density/velocity data. Full PhysX Flow simulation (actual fluid dynamics) is the next development phase.

What works now:
- ✅ DLL loading and initialization
- ✅ API calls to create/destroy emitters and grids
- ✅ Texture export pipeline
- ✅ Volumetric rendering shader
- ✅ Unity components and editor integration

What's next:
- 🔲 Replace placeholder data with real Flow simulation
- 🔲 NanoVDB export for sparse grids
- 🔲 Collision with Unity geometry
- 🔲 Advanced rendering (HDRP/URP integration)

---

## Success Criteria

Your integration is successful when:
1. ✅ Unity Editor starts without DLL errors
2. ✅ Play mode shows initialization logs
3. ✅ Components appear in Add Component menu
4. ✅ Gizmos render in Scene view
5. ✅ No errors in Console during Play mode
6. ✅ (Later) Fluid visuals appear in Game view with actual simulation

---

## Support

- **Documentation**: See `Tools > PhysX Flow > Documentation` in Unity
- **API Reference**: SIMULATION_API.md
- **Quick Reference**: QUICK_REFERENCE.md
- **Issues**: Check console logs and compare against troubleshooting guides

Happy fluid simulating! 🌊
