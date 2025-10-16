using UnityEngine;
using UnityEngine.Rendering;

namespace UnityPhysXFlow
{
    /// <summary>
    /// GPU-accelerated volumetric renderer using compute shaders
    /// Provides massive performance improvement over CPU mesh rendering
    /// </summary>
    [RequireComponent(typeof(FlowGrid))]
    public class FlowGPURenderer : MonoBehaviour
    {
        [Header("Compute Shader")]
        [SerializeField] private ComputeShader rayMarchShader;
        
        [Header("Rendering")]
        [SerializeField] private RenderTexture outputTexture;
        [SerializeField, Range(512, 2048)] private int renderWidth = 1024;
        [SerializeField, Range(512, 2048)] private int renderHeight = 1024;
        
        [Header("Visual Settings")]
        [SerializeField] private Color volumeColor = new Color(0.8f, 0.9f, 1.0f, 1.0f);
        [SerializeField, Range(0.1f, 10f)] private float densityScale = 2.0f;
        [SerializeField, Range(0.01f, 1f)] private float stepSize = 0.1f;
        [SerializeField, Range(16, 256)] private int maxSteps = 128;
        
        [Header("Lighting")]
        [SerializeField, Range(0f, 2f)] private float lightIntensity = 1.0f;
        [SerializeField] private Vector3 lightDirection = new Vector3(-1, -1, -1);
        
        [Header("Debug")]
        [SerializeField] private bool showOutput = true;
        
        private FlowGrid flowGrid;
        private Texture3D densityTexture;
        private Texture3D velocityTexture;
        private int kernelHandle;
        private Material displayMaterial;
        
        private void Start()
        {
            flowGrid = GetComponent<FlowGrid>();
            
            // Load compute shader if not assigned
            if (rayMarchShader == null)
            {
                rayMarchShader = Resources.Load<ComputeShader>("VolumetricRayMarch");
            }
            
            if (rayMarchShader != null)
            {
                kernelHandle = rayMarchShader.FindKernel("CSMain");
                CreateTextures();
                CreateOutputTexture();
                
                // Create material for displaying output
                displayMaterial = new Material(Shader.Find("Unlit/Texture"));
            }
            else
            {
                Debug.LogError("FlowGPURenderer: Compute shader not found!");
                enabled = false;
            }
        }
        
        private void CreateTextures()
        {
            Vector3Int dims = flowGrid.GridDimensions;
            int sizeX = dims.x;
            int sizeY = dims.y;
            int sizeZ = dims.z;
            
            // Create 3D textures for GPU
            densityTexture = new Texture3D(sizeX, sizeY, sizeZ, TextureFormat.RFloat, false);
            densityTexture.wrapMode = TextureWrapMode.Clamp;
            densityTexture.filterMode = FilterMode.Trilinear;
            
            velocityTexture = new Texture3D(sizeX, sizeY, sizeZ, TextureFormat.RGBAFloat, false);
            velocityTexture.wrapMode = TextureWrapMode.Clamp;
            velocityTexture.filterMode = FilterMode.Trilinear;
            
            Debug.Log($"FlowGPURenderer: Created 3D textures ({sizeX}x{sizeY}x{sizeZ})");
        }
        
        private void CreateOutputTexture()
        {
            if (outputTexture != null)
            {
                outputTexture.Release();
            }
            
            outputTexture = new RenderTexture(renderWidth, renderHeight, 0, RenderTextureFormat.ARGBFloat);
            outputTexture.enableRandomWrite = true;
            outputTexture.Create();
        }
        
        private void Update()
        {
            if (rayMarchShader == null || densityTexture == null) return;
            
            // Upload density and velocity data to GPU
            UploadDataToGPU();
            
            // Execute compute shader
            RenderVolume();
        }
        
        private void UploadDataToGPU()
        {
            // Get data from FlowGrid
            var densityData = flowGrid.GetDensityData();
            var velocityData = flowGrid.GetVelocityData();
            
            if (densityData != null && densityData.Length > 0)
            {
                // Upload density (single channel float)
                densityTexture.SetPixelData(densityData, 0);
                densityTexture.Apply();
            }
            
            if (velocityData != null && velocityData.Length > 0)
            {
                // Convert velocity data (3 floats per cell) to Color format (4 floats)
                Vector3Int dims = flowGrid.GridDimensions;
                int cellCount = dims.x * dims.y * dims.z;
                Color[] velocityColors = new Color[cellCount];
                
                for (int i = 0; i < cellCount; i++)
                {
                    int idx = i * 3;
                    velocityColors[i] = new Color(
                        velocityData[idx + 0],
                        velocityData[idx + 1],
                        velocityData[idx + 2],
                        0f
                    );
                }
                
                velocityTexture.SetPixels(velocityColors);
                velocityTexture.Apply();
            }
        }
        
        private void RenderVolume()
        {
            Camera cam = Camera.main;
            if (cam == null) return;
            
            // Set compute shader parameters
            rayMarchShader.SetTexture(kernelHandle, "_DensityTexture", densityTexture);
            rayMarchShader.SetTexture(kernelHandle, "_VelocityTexture", velocityTexture);
            rayMarchShader.SetTexture(kernelHandle, "_Result", outputTexture);
            
            // Camera matrices
            Matrix4x4 viewProj = cam.projectionMatrix * cam.worldToCameraMatrix;
            Matrix4x4 invViewProj = viewProj.inverse;
            rayMarchShader.SetMatrix("_InvViewProj", invViewProj);
            rayMarchShader.SetVector("_CameraPos", cam.transform.position);
            rayMarchShader.SetVector("_CameraForward", cam.transform.forward);
            
            // Volume parameters
            Vector3 volumeSize = flowGrid.GridWorldSize;
            
            rayMarchShader.SetVector("_VolumePosition", transform.position);
            rayMarchShader.SetVector("_VolumeSize", volumeSize);
            rayMarchShader.SetFloat("_CellSize", flowGrid.cellSize);
            
            // Rendering parameters
            rayMarchShader.SetFloat("_DensityScale", densityScale);
            rayMarchShader.SetVector("_Color", volumeColor);
            rayMarchShader.SetFloat("_StepSize", stepSize);
            rayMarchShader.SetInt("_MaxSteps", maxSteps);
            
            // Lighting
            rayMarchShader.SetFloat("_LightIntensity", lightIntensity);
            rayMarchShader.SetVector("_LightDir", lightDirection.normalized);
            
            // Dispatch compute shader
            int threadGroupsX = Mathf.CeilToInt(renderWidth / 8.0f);
            int threadGroupsY = Mathf.CeilToInt(renderHeight / 8.0f);
            rayMarchShader.Dispatch(kernelHandle, threadGroupsX, threadGroupsY, 1);
        }
        
        private void OnRenderImage(RenderTexture source, RenderTexture destination)
        {
            if (showOutput && outputTexture != null && displayMaterial != null)
            {
                Graphics.Blit(outputTexture, destination, displayMaterial);
            }
            else
            {
                Graphics.Blit(source, destination);
            }
        }
        
        private void OnDestroy()
        {
            if (densityTexture != null)
            {
                Destroy(densityTexture);
            }
            
            if (velocityTexture != null)
            {
                Destroy(velocityTexture);
            }
            
            if (outputTexture != null)
            {
                outputTexture.Release();
                Destroy(outputTexture);
            }
            
            if (displayMaterial != null)
            {
                Destroy(displayMaterial);
            }
        }
        
        private void OnValidate()
        {
            // Recreate output texture if resolution changed
            if (outputTexture != null && 
                (outputTexture.width != renderWidth || outputTexture.height != renderHeight))
            {
                CreateOutputTexture();
            }
        }
    }
}
