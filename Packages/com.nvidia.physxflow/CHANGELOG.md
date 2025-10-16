# Changelog

All notable changes to NVIDIA PhysX Flow for Unity will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.0.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [1.0.0] - 2025-10-16

### Added
- Initial release of NVIDIA PhysX Flow for Unity package
- Native C++ bridge (`unity_physx_flow.dll`) with Flow SDK integration
- C# wrapper with P/Invoke bindings
- `FlowEmitter` component for spawning fluid particles
- `FlowGrid` component for simulation domain management
- Volumetric rendering shader (`PhysXFlow/VolumetricFluid`)
- Custom Unity inspectors for all components
- Setup wizard (`Tools > PhysX Flow > Setup Wizard`)
- Menu items for quick scene creation
- Automatic texture export (density and velocity)
- Gizmo visualization for emitters and grids
- Complete API for emitter and grid control
- Thread-safe native state management
- Main-thread event dispatch system
- Profiler event forwarding from Flow to Unity

### API
- `UnityPhysXFlow.Init()` - Initialize Flow bridge
- `UnityPhysXFlow.CreateEmitter()` - Create fluid emitter
- `UnityPhysXFlow.SetEmitterParams()` - Update emitter parameters
- `UnityPhysXFlow.DestroyEmitter()` - Remove emitter
- `UnityPhysXFlow.CreateGrid()` - Create simulation grid
- `UnityPhysXFlow.StepGrid()` - Advance simulation
- `UnityPhysXFlow.ExportGridDensityAsTexture3D()` - Export density as Texture3D
- `UnityPhysXFlow.ExportGridVelocityAsTexture3D()` - Export velocity as Texture3D
- `UnityPhysXFlow.DestroyGrid()` - Remove grid
- `UnityPhysXFlow.Shutdown()` - Cleanup resources

### Documentation
- Comprehensive API reference
- Quick reference guide
- Deployment checklist
- Unity integration summary
- Shader documentation

### Known Issues
- Simulation uses placeholder data (actual Flow integration in progress)
- Windows x64 only (cross-platform support planned)
- Requires manual DLL deployment

## [Unreleased]

### Planned for 1.1.0
- Full PhysX Flow simulation integration (replace placeholder data)
- Real-time density/velocity export from Flow grids
- Collision detection with Unity geometry
- Boundary condition support
- Performance optimizations

### Planned for 2.0.0
- NanoVDB sparse volume export
- HDRP volumetric fog integration
- URP renderer feature for volumetrics
- Multi-platform support (Linux, macOS)
- Advanced emitter types (jets, explosions)
- Compute shader-based raymarching

### Planned for Future Releases
- Multi-GPU support
- VFX Graph integration
- Scriptable Render Pipeline (SRP) Batcher compatibility
- Mobile platform support (if feasible)
- Advanced rendering features (shadows, lighting, multi-scattering)

## Version History

- **1.0.0** (2025-10-16): Initial release with API scaffolding and Unity components

---

## Update Instructions

### Updating via Unity Package Manager
If installed via Git URL:
1. Open Package Manager
2. Select "PhysX Flow for Unity"
3. Click "Update" when available

### Updating Manually
1. Backup your project
2. Delete `Packages/com.nvidia.physxflow`
3. Copy new version to `Packages/`
4. Restart Unity

### Breaking Changes
We follow semantic versioning:
- **Major version** (x.0.0): Breaking API changes
- **Minor version** (1.x.0): New features, backward compatible
- **Patch version** (1.0.x): Bug fixes, backward compatible

---

## Contributing

We welcome contributions! See CONTRIBUTING.md for guidelines.

## Support

- **Issues**: [GitHub Issues](https://github.com/your-username/UnityPhysXFlow/issues)
- **Discussions**: [GitHub Discussions](https://github.com/your-username/UnityPhysXFlow/discussions)
