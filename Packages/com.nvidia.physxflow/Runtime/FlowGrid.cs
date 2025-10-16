using UnityEngine;

namespace UnityPhysXFlow
{
    /// <summary>
    /// PhysX Flow Grid Component - manages a simulation grid and exports density/velocity for rendering.
    /// </summary>
    [AddComponentMenu("PhysX Flow/Flow Grid")]
    public class FlowGrid : MonoBehaviour
    {
        [Header("Grid Parameters")]
        [Tooltip("Grid resolution in X")]
        public int sizeX = 64;

        [Tooltip("Grid resolution in Y")]
        public int sizeY = 64;

        [Tooltip("Grid resolution in Z")]
        public int sizeZ = 64;

        [Tooltip("Cell size in world units")]
        [Range(0.01f, 1f)]
        public float cellSize = 0.1f;

        [Tooltip("Auto-create grid on Start")]
        public bool autoCreate = true;

        [Header("Rendering")]
        [Tooltip("Material for volumetric rendering (should use Flow shader)")]
        public Material volumetricMaterial;

        [Tooltip("Update textures every N frames (0 = every frame)")]
        [Range(0, 10)]
        public int updateInterval = 1;

        [Header("Debug")]
        [Tooltip("Use placeholder test data if simulation isn't working")]
        public bool usePlaceholderData = false;

        private int _gridHandle = -1;
        private Texture3D _densityTexture;
        private Texture3D _velocityTexture;
        private int _frameCounter = 0;
        private GameObject _visualCube;
        private MeshRenderer _meshRenderer;

        private void Start()
        {
            if (autoCreate)
            {
                // Ensure Flow is initialized before creating grid
                UnityPhysXFlow.EnsureInitialized();
                CreateGrid();
            }
        }

        private void Update()
        {
            if (_gridHandle < 0) return;

            // Step simulation
            UnityPhysXFlow.StepGrid(_gridHandle, Time.deltaTime);

            // Update textures at specified interval
            _frameCounter++;
            if (updateInterval == 0 || _frameCounter >= updateInterval)
            {
                _frameCounter = 0;
                UpdateTextures();
            }
        }

        public void CreateGrid()
        {
            if (_gridHandle >= 0) return; // Already created

            // Check if Flow is initialized
            if (!UnityPhysXFlow.IsInitialized)
            {
                Debug.LogError("[FlowGrid] Cannot create grid: Flow not initialized. Call UnityPhysXFlow.Init() first.");
                return;
            }

            _gridHandle = UnityPhysXFlow.CreateGrid(sizeX, sizeY, sizeZ, cellSize);
            if (_gridHandle < 0)
            {
                Debug.LogError($"[FlowGrid] Failed to create grid {sizeX}x{sizeY}x{sizeZ}");
            }
            else
            {
                Debug.Log($"[FlowGrid] Created grid {_gridHandle}: {sizeX}x{sizeY}x{sizeZ}, cellSize={cellSize}");
                CreateVisualCube();
            }
        }

        public void DestroyGrid()
        {
            if (_gridHandle < 0) return;

            UnityPhysXFlow.DestroyGrid(_gridHandle);
            Debug.Log($"[FlowGrid] Destroyed grid {_gridHandle}");
            _gridHandle = -1;

            if (_densityTexture != null) Destroy(_densityTexture);
            if (_velocityTexture != null) Destroy(_velocityTexture);
            
            DestroyVisualCube();
        }

        private void CreateVisualCube()
        {
            // Create a cube mesh to render the volumetric data
            _visualCube = GameObject.CreatePrimitive(PrimitiveType.Cube);
            _visualCube.name = "FlowGrid_Visual";
            _visualCube.transform.SetParent(transform, false);
            _visualCube.transform.localPosition = Vector3.zero;
            
            // Scale to match grid world size
            Vector3 gridWorldSize = new Vector3(sizeX * cellSize, sizeY * cellSize, sizeZ * cellSize);
            _visualCube.transform.localScale = gridWorldSize;
            
            // Remove collider (we don't need physics on the visual)
            var collider = _visualCube.GetComponent<Collider>();
            if (collider != null) Destroy(collider);
            
            // Get renderer and assign material
            _meshRenderer = _visualCube.GetComponent<MeshRenderer>();
            if (volumetricMaterial != null)
            {
                _meshRenderer.material = volumetricMaterial;
            }
            else
            {
                Debug.LogWarning("[FlowGrid] No volumetric material assigned! Assign a material using the VolumetricFluid or VolumetricFluidNVIDIA shader.");
            }
        }

        private void DestroyVisualCube()
        {
            if (_visualCube != null)
            {
                Destroy(_visualCube);
                _visualCube = null;
                _meshRenderer = null;
            }
        }

        private void UpdateTextures()
        {
            if (_gridHandle < 0) return;

            if (usePlaceholderData)
            {
                UpdatePlaceholderTextures();
                return;
            }

            // Export density
            Texture3D densityTex = UnityPhysXFlow.ExportGridDensityAsTexture3D(_gridHandle);
            if (densityTex != null)
            {
                if (_densityTexture != null) Destroy(_densityTexture);
                _densityTexture = densityTex;

                // Bind to material
                if (volumetricMaterial != null)
                {
                    volumetricMaterial.SetTexture("_DensityTex", _densityTexture);
                }
            }
            else
            {
                Debug.LogWarning("[FlowGrid] ExportGridDensityAsTexture3D returned null. Enable 'Use Placeholder Data' to test rendering.");
            }

            // Export velocity
            Texture3D velocityTex = UnityPhysXFlow.ExportGridVelocityAsTexture3D(_gridHandle);
            if (velocityTex != null)
            {
                if (_velocityTexture != null) Destroy(_velocityTexture);
                _velocityTexture = velocityTex;

                // Bind to material
                if (volumetricMaterial != null)
                {
                    volumetricMaterial.SetTexture("_VelocityTex", _velocityTexture);
                }
            }
        }

        private void UpdatePlaceholderTextures()
        {
            // Create test density texture with a sphere in the center
            if (_densityTexture == null)
            {
                _densityTexture = new Texture3D(sizeX, sizeY, sizeZ, TextureFormat.RFloat, false);
                _densityTexture.wrapMode = TextureWrapMode.Clamp;
                _densityTexture.filterMode = FilterMode.Bilinear;
                
                Color[] colors = new Color[sizeX * sizeY * sizeZ];
                float centerX = sizeX * 0.5f;
                float centerY = sizeY * 0.5f;
                float centerZ = sizeZ * 0.5f;
                float radius = Mathf.Min(sizeX, sizeY, sizeZ) * 0.3f;
                
                for (int z = 0; z < sizeZ; z++)
                {
                    for (int y = 0; y < sizeY; y++)
                    {
                        for (int x = 0; x < sizeX; x++)
                        {
                            int idx = x + y * sizeX + z * sizeX * sizeY;
                            
                            // Distance from center
                            float dx = x - centerX;
                            float dy = y - centerY;
                            float dz = z - centerZ;
                            float dist = Mathf.Sqrt(dx * dx + dy * dy + dz * dz);
                            
                            // Smooth falloff
                            float density = Mathf.Max(0, 1.0f - (dist / radius));
                            density = density * density; // Squared falloff for smoother look
                            
                            colors[idx] = new Color(density, 0, 0, 0);
                        }
                    }
                }
                
                _densityTexture.SetPixels(colors);
                _densityTexture.Apply();
                
                if (volumetricMaterial != null)
                {
                    volumetricMaterial.SetTexture("_DensityTex", _densityTexture);
                }
                
                Debug.Log("[FlowGrid] Created placeholder density texture for testing");
            }
            
            // Create empty velocity texture
            if (_velocityTexture == null)
            {
                _velocityTexture = new Texture3D(sizeX, sizeY, sizeZ, TextureFormat.RGBAFloat, false);
                _velocityTexture.wrapMode = TextureWrapMode.Clamp;
                
                Color[] colors = new Color[sizeX * sizeY * sizeZ];
                for (int i = 0; i < colors.Length; i++)
                {
                    colors[i] = new Color(0, 0, 0, 0);
                }
                
                _velocityTexture.SetPixels(colors);
                _velocityTexture.Apply();
                
                if (volumetricMaterial != null)
                {
                    volumetricMaterial.SetTexture("_VelocityTex", _velocityTexture);
                }
            }
        }

        private void OnDestroy()
        {
            DestroyGrid();
        }

        private void OnDrawGizmos()
        {
            Vector3 gridWorldSize = new Vector3(sizeX * cellSize, sizeY * cellSize, sizeZ * cellSize);
            Gizmos.color = Color.green;
            Gizmos.DrawWireCube(transform.position, gridWorldSize);
        }

        private void OnDrawGizmosSelected()
        {
            Vector3 gridWorldSize = new Vector3(sizeX * cellSize, sizeY * cellSize, sizeZ * cellSize);
            Gizmos.color = new Color(0, 1, 0, 0.2f);
            Gizmos.DrawCube(transform.position, gridWorldSize);
        }
    }
}
