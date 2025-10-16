using System;
using System.Runtime.InteropServices;
using System.Threading;
using UnityEngine;

namespace UnityPhysXFlow
{
    public static class UnityPhysXFlow
    {
#if UNITY_STANDALONE_WIN || UNITY_EDITOR_WIN
        private const string DllName = "unity_physx_flow";
#else
        private const string DllName = "unity_physx_flow";
#endif

        [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
        public delegate void EventCallback(int eventType, string jsonPayload, IntPtr userData);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int Upf_Init();

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void Upf_RegisterCallback(EventCallback cb, IntPtr userData);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void Upf_Step(float dt);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void Upf_EmitTestEvent(string message);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void Upf_Shutdown();

        private static bool _isInitialized = false;

#if UNITY_STANDALONE_WIN || UNITY_EDITOR_WIN
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.Unicode)]
        private static extern void Upf_SetDllDirectoryW(string path);
#endif

        // --- Simulation API ---
        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int Upf_CreateEmitter(float x, float y, float z, float radius, float density);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void Upf_DestroyEmitter(int emitterHandle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void Upf_SetEmitterParams(int emitterHandle, float x, float y, float z, float radius, float density);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern int Upf_CreateGrid(int sizeX, int sizeY, int sizeZ, float cellSize);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void Upf_DestroyGrid(int gridHandle);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern void Upf_StepGrid(int gridHandle, float dt);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr Upf_ExportGridDensity(int gridHandle, out int outSizeX, out int outSizeY, out int outSizeZ, out int outFormat);

        [DllImport(DllName, CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr Upf_ExportGridVelocity(int gridHandle, out int outSizeX, out int outSizeY, out int outSizeZ, out int outFormat);

        private static EventCallback _cb;
        private static GCHandle _gcThis;

        /// <summary>
        /// Check if Flow system is initialized
        /// </summary>
        public static bool IsInitialized => _isInitialized;

        /// <summary>
        /// Auto-initialize on application start
        /// </summary>
        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.BeforeSceneLoad)]
        private static void AutoInitialize()
        {
            if (!_isInitialized)
            {
                Debug.Log("[UnityPhysXFlow] Auto-initializing...");
                EnsureInitialized();
            }
        }

        public static void SetDllDirectory(string path)
        {
#if UNITY_STANDALONE_WIN || UNITY_EDITOR_WIN
            Upf_SetDllDirectoryW(path);
#endif
        }

        public static int Init(Action<int, string> onEvent)
        {
            if (_isInitialized)
            {
                Debug.LogWarning("[UnityPhysXFlow] Already initialized");
                return 0;
            }

            _cb = (evt, payload, ud) =>
            {
                // Unity main thread dispatch if needed
                UnityMainThreadDispatcher.Enqueue(() => onEvent?.Invoke(evt, payload));
            };
            _gcThis = GCHandle.Alloc(_cb);
            Upf_RegisterCallback(_cb, IntPtr.Zero);
            
            int result = Upf_Init();
            if (result == 0)
            {
                _isInitialized = true;
                Debug.Log("[UnityPhysXFlow] Initialized successfully");
            }
            else
            {
                Debug.LogError($"[UnityPhysXFlow] Initialization failed with code {result}");
            }
            
            return result;
        }

        /// <summary>
        /// Initialize with default event handler if not already initialized
        /// </summary>
        public static void EnsureInitialized()
        {
            if (!_isInitialized)
            {
                Init((evt, payload) => Debug.Log($"[UnityPhysXFlow] Event {evt}: {payload}"));
            }
        }

        public static void Step(float dt) => Upf_Step(dt);
        public static void EmitTest(string msg) => Upf_EmitTestEvent(msg);
        public static void Shutdown()
        {
            Upf_Shutdown();
            if (_gcThis.IsAllocated) _gcThis.Free();
            _cb = null;
            _isInitialized = false;
            Debug.Log("[UnityPhysXFlow] Shutdown complete");
        }

        // --- Simulation API ---
        public static int CreateEmitter(Vector3 position, float radius, float density)
        {
            return Upf_CreateEmitter(position.x, position.y, position.z, radius, density);
        }

        public static void DestroyEmitter(int emitterHandle)
        {
            Upf_DestroyEmitter(emitterHandle);
        }

        public static void SetEmitterParams(int emitterHandle, Vector3 position, float radius, float density)
        {
            Upf_SetEmitterParams(emitterHandle, position.x, position.y, position.z, radius, density);
        }

        public static int CreateGrid(int sizeX, int sizeY, int sizeZ, float cellSize)
        {
            return Upf_CreateGrid(sizeX, sizeY, sizeZ, cellSize);
        }

        public static void DestroyGrid(int gridHandle)
        {
            Upf_DestroyGrid(gridHandle);
        }

        public static void StepGrid(int gridHandle, float dt)
        {
            Upf_StepGrid(gridHandle, dt);
        }

        public static Texture3D ExportGridDensityAsTexture3D(int gridHandle)
        {
            IntPtr dataPtr = Upf_ExportGridDensity(gridHandle, out int sizeX, out int sizeY, out int sizeZ, out int format);
            if (dataPtr == IntPtr.Zero) return null;

            // Format 0 = float32
            Texture3D tex = new Texture3D(sizeX, sizeY, sizeZ, TextureFormat.RFloat, false);
            int dataSize = sizeX * sizeY * sizeZ * sizeof(float);
            byte[] data = new byte[dataSize];
            Marshal.Copy(dataPtr, data, 0, dataSize);

            // Convert byte[] to float[] and load into texture
            float[] floatData = new float[sizeX * sizeY * sizeZ];
            Buffer.BlockCopy(data, 0, floatData, 0, dataSize);
            
            // Unity Texture3D expects color data; we'll pack float into RFloat format
            Color[] colors = new Color[floatData.Length];
            for (int i = 0; i < floatData.Length; i++)
            {
                colors[i] = new Color(floatData[i], 0, 0, 0);
            }
            tex.SetPixels(colors);
            tex.Apply();
            return tex;
        }

        public static Texture3D ExportGridVelocityAsTexture3D(int gridHandle)
        {
            IntPtr dataPtr = Upf_ExportGridVelocity(gridHandle, out int sizeX, out int sizeY, out int sizeZ, out int format);
            if (dataPtr == IntPtr.Zero) return null;

            // Format 1 = float32x3 (vec3)
            Texture3D tex = new Texture3D(sizeX, sizeY, sizeZ, TextureFormat.RGBAFloat, false);
            int dataSize = sizeX * sizeY * sizeZ * 3 * sizeof(float);
            byte[] data = new byte[dataSize];
            Marshal.Copy(dataPtr, data, 0, dataSize);

            float[] floatData = new float[sizeX * sizeY * sizeZ * 3];
            Buffer.BlockCopy(data, 0, floatData, 0, dataSize);

            Color[] colors = new Color[sizeX * sizeY * sizeZ];
            for (int i = 0; i < colors.Length; i++)
            {
                int idx = i * 3;
                colors[i] = new Color(floatData[idx], floatData[idx + 1], floatData[idx + 2], 0);
            }
            tex.SetPixels(colors);
            tex.Apply();
            return tex;
        }
    }

    // Minimal main-thread dispatcher
    public class UnityMainThreadDispatcher : MonoBehaviour
    {
        private static readonly System.Collections.Concurrent.ConcurrentQueue<Action> _queue = new();
        private static UnityMainThreadDispatcher _instance;

        [RuntimeInitializeOnLoadMethod(RuntimeInitializeLoadType.AfterSceneLoad)]
        private static void Bootstrap()
        {
            if (_instance == null)
            {
                var go = new GameObject("FlowMainThreadDispatcher");
                DontDestroyOnLoad(go);
                _instance = go.AddComponent<UnityMainThreadDispatcher>();
            }
        }

        public static void Enqueue(Action a)
        {
            if (a != null) _queue.Enqueue(a);
        }

        private void Update()
        {
            while (_queue.TryDequeue(out var a))
            {
                try { a(); } catch (Exception e) { Debug.LogException(e); }
            }
        }
    }

    // Example usage component
    public class UnityPhysXFlowExample : MonoBehaviour
    {
        [Header("Setup")]
        [Tooltip("Optional: path to PhysX Flow DLLs")]
        public string flowDllPath = "";

        [Header("Demo Configuration")]
        [Tooltip("Create a demo emitter and grid")]
        public bool createDemoScene = true;

        [Tooltip("Material for volumetric rendering")]
        public Material volumetricMaterial;

        private int _demoEmitterHandle = -1;
        private int _demoGridHandle = -1;

        private void Start()
        {
            // Set DLL directory if specified
            if (!string.IsNullOrEmpty(flowDllPath))
            {
                UnityPhysXFlow.SetDllDirectory(flowDllPath);
            }

            // Initialize UnityPhysXFlow
            int rc = UnityPhysXFlow.Init((evt, payload) => 
            {
                // Optional: filter out noisy frame events
                if (evt != 0) // 0 = frame flush events
                {
                    Debug.Log($"[UnityPhysXFlow] evt={evt} payload={payload}");
                }
            });

            if (rc != 0)
            {
                Debug.LogError($"[UnityPhysXFlow] Init failed with code {rc}");
                return;
            }

            Debug.Log($"[UnityPhysXFlow] Initialized successfully");

            // Create demo scene if requested
            if (createDemoScene)
            {
                CreateDemoScene();
            }
        }

        private void CreateDemoScene()
        {
            // Create a grid
            _demoGridHandle = UnityPhysXFlow.CreateGrid(64, 64, 64, 0.1f);
            if (_demoGridHandle < 0)
            {
                Debug.LogError("[UnityPhysXFlow] Failed to create demo grid");
                return;
            }

            Debug.Log($"[UnityPhysXFlow] Created demo grid {_demoGridHandle}");

            // Create an emitter at origin
            Vector3 emitterPos = transform.position + Vector3.up * 2f;
            _demoEmitterHandle = UnityPhysXFlow.CreateEmitter(emitterPos, 1f, 1f);
            if (_demoEmitterHandle < 0)
            {
                Debug.LogError("[UnityPhysXFlow] Failed to create demo emitter");
                return;
            }

            Debug.Log($"[UnityPhysXFlow] Created demo emitter {_demoEmitterHandle} at {emitterPos}");

            // Create a visual cube for the grid
            GameObject gridVis = GameObject.CreatePrimitive(PrimitiveType.Cube);
            gridVis.name = "FlowGrid_Visual";
            gridVis.transform.position = transform.position;
            gridVis.transform.localScale = new Vector3(6.4f, 6.4f, 6.4f); // 64 * 0.1

            if (volumetricMaterial != null)
            {
                gridVis.GetComponent<Renderer>().material = volumetricMaterial;
            }
        }

        private void Update()
        {
            UnityPhysXFlow.Step(Time.deltaTime);

            // Step demo grid if active
            if (_demoGridHandle >= 0)
            {
                UnityPhysXFlow.StepGrid(_demoGridHandle, Time.deltaTime);
            }
        }

        private void OnDestroy()
        {
            // Clean up demo resources
            if (_demoEmitterHandle >= 0)
            {
                UnityPhysXFlow.DestroyEmitter(_demoEmitterHandle);
            }

            if (_demoGridHandle >= 0)
            {
                UnityPhysXFlow.DestroyGrid(_demoGridHandle);
            }

            UnityPhysXFlow.Shutdown();
            Debug.Log("[UnityPhysXFlow] Shutdown complete");
        }
    }
}
