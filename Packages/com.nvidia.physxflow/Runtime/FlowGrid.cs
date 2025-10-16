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

        private int _gridHandle = -1;
        private Texture3D _densityTexture;
        private Texture3D _velocityTexture;
        private int _frameCounter = 0;

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
        }

        private void UpdateTextures()
        {
            if (_gridHandle < 0) return;

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
