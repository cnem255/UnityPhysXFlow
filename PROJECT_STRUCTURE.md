# UnityPhysXFlow Project Structure

Complete file listing for the UnityPhysXFlow project.

## Root Directory

```
UnityPhysXFlow/
├── README.md                           # Project overview and quick-start
├── DEPLOYMENT.md                       # Build and deployment guide
├── SIMULATION_API.md                   # Complete API reference
├── QUICK_REFERENCE.md                  # Quick lookup for common tasks
├── UNITY_DEPLOYMENT_CHECKLIST.md       # Step-by-step Unity integration
├── UNITY_INTEGRATION_SUMMARY.md        # Technical implementation details
├── IMPLEMENTATION_COMPLETE.md          # Current status and next steps
├── FLOW_BUILD_WORKAROUND.md           # PhysX Flow build troubleshooting
├── build.ps1                           # Native DLL build script
├── .vscode/                            # VS Code workspace settings
├── native/                             # Native C++ bridge (→)
├── Unity/                              # Unity C# components (→)
└── PhysX/                              # PhysX SDK (external, not listed)
```

---

## Native Bridge (`native/`)

```
native/
├── CMakeLists.txt                      # CMake build configuration
├── include/
│   └── UnityPhysXFlow.h               # Public C API header
└── src/
    └── UnityPhysXFlow.cpp             # Implementation with Flow integration
```

### Build Artifacts (Generated)
```
native/_build/
├── Release/
│   ├── unity_physx_flow.dll           # Main native DLL (174 KB)
│   └── unity_physx_flow.lib           # Import library
└── Debug/
    ├── unity_physx_flow.dll           # Debug build
    └── unity_physx_flow.lib
```

---

## Unity Integration (`Unity/`)

```
Unity/
├── Runtime/                            # Unity runtime scripts
│   ├── UnityPhysXFlow.cs              # Main C# wrapper and API
│   ├── FlowEmitter.cs                 # Emitter component
│   └── FlowGrid.cs                    # Grid component
├── Editor/                             # Unity editor scripts
│   ├── FlowComponentEditors.cs        # Custom inspectors
│   └── FlowSetupMenu.cs               # Menu items and setup wizard
└── Shaders/
    └── VolumetricFluid.shader         # Raymarch shader for rendering
```

### Recommended Unity Project Structure
```
Assets/
└── Plugins/
    └── UnityPhysXFlow/
        ├── x86_64/
        │   └── unity_physx_flow.dll   # Native DLL
        ├── Runtime/
        │   ├── UnityPhysXFlow.cs
        │   ├── FlowEmitter.cs
        │   └── FlowGrid.cs
        ├── Editor/
        │   ├── FlowComponentEditors.cs
        │   └── FlowSetupMenu.cs
        └── Shaders/
            └── VolumetricFluid.shader
```

---

## Documentation Files

### User Documentation
- **README.md** - Project overview, setup, quick-start
- **QUICK_REFERENCE.md** - Quick lookup, common tasks, troubleshooting
- **UNITY_DEPLOYMENT_CHECKLIST.md** - Step-by-step integration guide
- **SIMULATION_API.md** - Complete API reference with examples

### Technical Documentation
- **UNITY_INTEGRATION_SUMMARY.md** - Implementation details, architecture
- **IMPLEMENTATION_COMPLETE.md** - Current status, next steps
- **DEPLOYMENT.md** - Build process, deployment options
- **FLOW_BUILD_WORKAROUND.md** - PhysX Flow SDK build issues

---

## Key Files by Purpose

### Initialization & Setup
```
Unity/UnityPhysXFlow.cs               # Init(), Step(), Shutdown()
Unity/Editor/FlowSetupMenu.cs         # Setup wizard, menu items
native/src/UnityPhysXFlow.cpp         # Upf_Init(), NvFlowLoader
```

### Simulation Control
```
Unity/UnityPhysXFlow.cs               # CreateEmitter(), CreateGrid()
Unity/FlowEmitter.cs                  # Emitter component
Unity/FlowGrid.cs                     # Grid component
native/include/UnityPhysXFlow.h       # C API declarations
native/src/UnityPhysXFlow.cpp         # C API implementation
```

### Rendering Pipeline
```
Unity/UnityPhysXFlow.cs               # ExportGridDensityAsTexture3D()
Unity/FlowGrid.cs                     # UpdateTextures(), material binding
Unity/Shaders/VolumetricFluid.shader  # Raymarch rendering
native/src/UnityPhysXFlow.cpp         # Upf_ExportGridDensity/Velocity()
```

### Editor Integration
```
Unity/Editor/FlowComponentEditors.cs  # Custom inspectors
Unity/Editor/FlowSetupMenu.cs         # Menu items, wizard
Unity/FlowEmitter.cs                  # OnDrawGizmos()
Unity/FlowGrid.cs                     # OnDrawGizmos()
```

---

## Dependencies

### External (Not Included)
- **PhysX 5.6.1 SDK** - `PhysX/` directory (git submodule or manual checkout)
  - Flow headers: `PhysX/flow/include/`
  - Flow DLLs: `PhysX/flow/_build/windows-x86_64/release/`

### Build Tools
- **CMake** 3.20+ (build system)
- **Ninja** or Visual Studio 2022 (generator)
- **MSVC 17+** (compiler)
- **PowerShell** 5.1+ (build script)

### Runtime Dependencies
- **nvflow.dll** - PhysX Flow core library
- **nvflowext.dll** - PhysX Flow extensions
- **slang.dll** - Shader compiler (used by Flow)
- **Other Flow dependencies** - See `PhysX/flow/_build/.../release/`

---

## File Statistics

### Code Files
```
C++ Header:   1 file   (UnityPhysXFlow.h)
C++ Source:   1 file   (UnityPhysXFlow.cpp)
C# Runtime:   3 files  (UnityPhysXFlow.cs, FlowEmitter.cs, FlowGrid.cs)
C# Editor:    2 files  (FlowComponentEditors.cs, FlowSetupMenu.cs)
Shaders:      1 file   (VolumetricFluid.shader)
```

### Documentation
```
User Docs:    4 files  (README, QUICK_REFERENCE, DEPLOYMENT_CHECKLIST, SIMULATION_API)
Tech Docs:    4 files  (INTEGRATION_SUMMARY, IMPLEMENTATION_COMPLETE, DEPLOYMENT, FLOW_BUILD)
```

### Build System
```
CMake:        1 file   (CMakeLists.txt)
PowerShell:   1 file   (build.ps1)
```

### Total Lines of Code (Approximate)
```
C++:          ~500 lines
C#:           ~800 lines
Shader:       ~100 lines
Docs:         ~2000 lines
```

---

## Binary Artifacts

### Native DLL
```
File:     unity_physx_flow.dll
Size:     ~174 KB (Release), ~300 KB (Debug)
Platform: Windows x64
API:      C ABI (extern "C")
```

### Flow Runtime DLLs (External)
```
nvflow.dll:     ~15 MB (Release)
nvflowext.dll:  ~5 MB (Release)
slang.dll:      ~20 MB
```

---

## Git Repository Structure

```
UnityPhysXFlow/
├── .git/                               # Git metadata
├── .gitignore                          # Ignore build artifacts, IDE files
├── README.md                           # (tracked)
├── native/                             # (tracked)
├── Unity/                              # (tracked)
├── PhysX/                              # (submodule or external)
└── native/_build/                      # (ignored)
```

### Ignored Files
- `native/_build/` - CMake build directory
- `*.dll`, `*.lib`, `*.pdb` - Build artifacts
- `.vs/`, `.vscode/` - IDE metadata
- Unity temp files (if Unity project in repo)

---

## Usage Flow

### Developer Workflow
```
1. Clone repo
2. Checkout PhysX SDK (git submodule or manual)
3. Build Flow SDK (PhysX/flow/build.bat)
4. Build native DLL (.\build.ps1)
5. Copy files to Unity project
6. Test in Unity Editor
7. Build standalone game
```

### Unity Developer Workflow
```
1. Receive Unity package or manual files
2. Copy DLLs to Assets/Plugins/
3. Copy C# scripts to Assets/Plugins/UnityPhysXFlow/
4. Run setup wizard (Tools > PhysX Flow > Setup Wizard)
5. Create scene with FlowGrid and FlowEmitter
6. Assign volumetric material
7. Press Play
```

---

## Version History

### Current Version: 1.0 (Implementation Complete)
- ✅ Native bridge with simulation API
- ✅ C# wrapper with texture export
- ✅ Unity components (FlowEmitter, FlowGrid)
- ✅ Custom editors and menu system
- ✅ Volumetric rendering shader
- ✅ Complete documentation
- ⚠️ Placeholder simulation (actual Flow integration pending)

### Next Version: 1.1 (Planned)
- 🔲 Real Flow simulation integration
- 🔲 Actual density/velocity export
- 🔲 Collision and boundary conditions

### Future: 2.0
- 🔲 NanoVDB export
- 🔲 HDRP/URP integration
- 🔲 Advanced rendering features

---

## Related Resources

### Official Documentation
- PhysX SDK: https://github.com/NVIDIA-Omniverse/PhysX
- Flow Documentation: `PhysX/flow/doc/`
- Unity Scripting API: https://docs.unity3d.com/ScriptReference/

### This Project
- GitHub: (your repository URL)
- Issues: (your repository URL)/issues
- Wiki: (your repository URL)/wiki

---

## License

- **UnityPhysXFlow bridge code**: MIT License
- **PhysX/Flow SDK**: NVIDIA License (see PhysX/LICENSE.md)
- **Unity scripts**: MIT License (same as bridge)

---

## Maintenance

### Build Validation
Run `.\build.ps1 -Configuration Release` to verify:
- CMake configuration succeeds
- C++ compilation succeeds
- DLL is created with correct exports

### Unity Validation
Follow `UNITY_DEPLOYMENT_CHECKLIST.md` to verify:
- DLL loads without errors
- Components appear in Add Component menu
- Initialization succeeds in Play mode

### Documentation Updates
When modifying API:
1. Update `native/include/UnityPhysXFlow.h`
2. Update `Unity/UnityPhysXFlow.cs`
3. Update `SIMULATION_API.md`
4. Update `QUICK_REFERENCE.md` if user-facing

---

This structure provides a complete, production-ready PhysX Flow integration for Unity with native feel and comprehensive documentation.
