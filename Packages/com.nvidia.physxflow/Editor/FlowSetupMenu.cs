using UnityEngine;
using UnityEditor;
using System.IO;

namespace UnityPhysXFlow.Editor
{
    /// <summary>
    /// Menu items for quick UnityPhysXFlow setup
    /// </summary>
    public static class FlowSetupMenu
    {
        [MenuItem("GameObject/PhysX Flow/Create Flow System", false, 10)]
        public static void CreateFlowSystem()
        {
            GameObject flowSystem = new GameObject("FlowSystem");
            flowSystem.AddComponent<UnityPhysXFlowExample>();
            Selection.activeGameObject = flowSystem;

            Debug.Log("[UnityPhysXFlow] Created FlowSystem. Configure DLL path and volumetric material, then press Play.");
        }

        [MenuItem("GameObject/PhysX Flow/Create Flow Emitter", false, 11)]
        public static void CreateFlowEmitter()
        {
            GameObject emitter = new GameObject("FlowEmitter");
            emitter.AddComponent<FlowEmitter>();
            emitter.transform.position = Vector3.up * 2f;
            Selection.activeGameObject = emitter;

            Debug.Log("[UnityPhysXFlow] Created FlowEmitter. Adjust position, radius, and density in Inspector.");
        }

        [MenuItem("GameObject/PhysX Flow/Create Flow Grid", false, 12)]
        public static void CreateFlowGrid()
        {
            GameObject grid = new GameObject("FlowGrid");
            grid.AddComponent<FlowGrid>();
            Selection.activeGameObject = grid;

            Debug.Log("[UnityPhysXFlow] Created FlowGrid. Assign a volumetric material in Inspector.");
        }

        [MenuItem("GameObject/PhysX Flow/Create Complete Scene", false, 13)]
        public static void CreateCompleteScene()
        {
            // Create system
            GameObject flowSystem = new GameObject("FlowSystem");
            var example = flowSystem.AddComponent<UnityPhysXFlowExample>();
            example.createDemoScene = false; // We'll manually create the scene

            // Create grid
            GameObject grid = new GameObject("FlowGrid");
            var flowGrid = grid.AddComponent<FlowGrid>();
            flowGrid.sizeX = flowGrid.sizeY = flowGrid.sizeZ = 64;
            flowGrid.cellSize = 0.1f;
            flowGrid.updateInterval = 2;

            // Create emitter
            GameObject emitter = new GameObject("FlowEmitter");
            var flowEmitter = emitter.AddComponent<FlowEmitter>();
            emitter.transform.position = new Vector3(0, 3, 0);
            flowEmitter.radius = 1.0f;
            flowEmitter.density = 1.0f;

            Selection.activeGameObject = flowSystem;

            EditorUtility.DisplayDialog(
                "UnityPhysXFlow Setup Complete",
                "Created:\n" +
                "- FlowSystem (initialization)\n" +
                "- FlowGrid (simulation domain)\n" +
                "- FlowEmitter (particle source)\n\n" +
                "Next steps:\n" +
                "1. Create a Material using 'PhysXFlow/VolumetricFluid' shader\n" +
                "2. Assign it to FlowGrid's 'volumetricMaterial' field\n" +
                "3. Set FlowSystem's 'flowDllPath' if needed\n" +
                "4. Press Play!",
                "OK"
            );
        }

        [MenuItem("Assets/Create/PhysX Flow/Volumetric Material", false, 300)]
        public static void CreateVolumetricMaterial()
        {
            Shader shader = Shader.Find("PhysXFlow/VolumetricFluid");
            if (shader == null)
            {
                EditorUtility.DisplayDialog(
                    "Shader Not Found",
                    "Could not find 'PhysXFlow/VolumetricFluid' shader.\n\n" +
                    "Make sure VolumetricFluid.shader is in your project at:\n" +
                    "Assets/Plugins/UnityPhysXFlow/Shaders/",
                    "OK"
                );
                return;
            }

            Material material = new Material(shader);
            material.SetFloat("_DensityScale", 1.0f);
            material.SetColor("_Color", new Color(0.2f, 0.5f, 1.0f, 1.0f));
            material.SetFloat("_StepSize", 0.01f);
            material.SetInt("_MaxSteps", 128);

            string path = "Assets/FlowVolumetricMaterial.mat";
            path = AssetDatabase.GenerateUniqueAssetPath(path);
            AssetDatabase.CreateAsset(material, path);
            AssetDatabase.SaveAssets();

            EditorUtility.FocusProjectWindow();
            Selection.activeObject = material;

            Debug.Log($"[UnityPhysXFlow] Created volumetric material at {path}");
        }

        [MenuItem("Tools/PhysX Flow/Setup Wizard", false, 1000)]
        public static void ShowSetupWizard()
        {
            FlowSetupWizard.ShowWindow();
        }

        [MenuItem("Tools/PhysX Flow/Documentation", false, 1001)]
        public static void OpenDocumentation()
        {
            string repoPath = Path.Combine(Application.dataPath, "..", "..", "..");
            string readmePath = Path.Combine(repoPath, "README.md");
            string simApiPath = Path.Combine(repoPath, "SIMULATION_API.md");

            if (File.Exists(readmePath))
            {
                Application.OpenURL("file:///" + Path.GetFullPath(readmePath).Replace("\\", "/"));
            }
            else if (File.Exists(simApiPath))
            {
                Application.OpenURL("file:///" + Path.GetFullPath(simApiPath).Replace("\\", "/"));
            }
            else
            {
                Application.OpenURL("https://github.com/your-repo/UnityPhysXFlow");
            }
        }
    }

    /// <summary>
    /// Setup wizard for first-time configuration
    /// </summary>
    public class FlowSetupWizard : EditorWindow
    {
        private string dllPath = "";
        private bool createDemoScene = true;

        public static void ShowWindow()
        {
            var window = GetWindow<FlowSetupWizard>("PhysX Flow Setup");
            window.minSize = new Vector2(400, 300);
            window.Show();
        }

        private void OnGUI()
        {
            GUILayout.Label("UnityPhysXFlow Setup Wizard", EditorStyles.boldLabel);
            EditorGUILayout.Space();

            EditorGUILayout.HelpBox(
                "This wizard will help you set up PhysX Flow in your Unity project.",
                MessageType.Info
            );

            EditorGUILayout.Space();

            // DLL path
            GUILayout.Label("Step 1: Flow DLL Location", EditorStyles.boldLabel);
            EditorGUILayout.HelpBox(
                "Specify the folder containing nvflow.dll and nvflowext.dll.\n" +
                "Leave empty if DLLs are next to Unity.exe.",
                MessageType.None
            );
            EditorGUILayout.BeginHorizontal();
            dllPath = EditorGUILayout.TextField("DLL Path:", dllPath);
            if (GUILayout.Button("Browse", GUILayout.Width(80)))
            {
                string path = EditorUtility.OpenFolderPanel("Select Flow DLL Folder", "", "");
                if (!string.IsNullOrEmpty(path))
                {
                    dllPath = path;
                }
            }
            EditorGUILayout.EndHorizontal();

            EditorGUILayout.Space();

            // Demo scene option
            GUILayout.Label("Step 2: Demo Scene", EditorStyles.boldLabel);
            createDemoScene = EditorGUILayout.Toggle("Create Demo Scene", createDemoScene);
            EditorGUILayout.HelpBox(
                "Creates a complete scene with FlowSystem, FlowGrid, and FlowEmitter.",
                MessageType.None
            );

            EditorGUILayout.Space();

            // Create button
            if (GUILayout.Button("Create Flow Setup", GUILayout.Height(40)))
            {
                CreateSetup();
            }

            EditorGUILayout.Space();

            // Documentation links
            GUILayout.Label("Resources", EditorStyles.boldLabel);
            if (GUILayout.Button("Open README"))
            {
                FlowSetupMenu.OpenDocumentation();
            }
            if (GUILayout.Button("Open API Documentation"))
            {
                string repoPath = Path.Combine(Application.dataPath, "..", "..", "..");
                string simApiPath = Path.Combine(repoPath, "SIMULATION_API.md");
                if (File.Exists(simApiPath))
                {
                    Application.OpenURL("file:///" + Path.GetFullPath(simApiPath).Replace("\\", "/"));
                }
            }
        }

        private void CreateSetup()
        {
            if (createDemoScene)
            {
                FlowSetupMenu.CreateCompleteScene();

                // Apply DLL path if specified
                if (!string.IsNullOrEmpty(dllPath))
                {
                    GameObject flowSystem = GameObject.Find("FlowSystem");
                    if (flowSystem != null)
                    {
                        var example = flowSystem.GetComponent<UnityPhysXFlowExample>();
                        if (example != null)
                        {
                            example.flowDllPath = dllPath;
                            EditorUtility.SetDirty(example);
                        }
                    }
                }

                // Create volumetric material
                FlowSetupMenu.CreateVolumetricMaterial();

                EditorUtility.DisplayDialog(
                    "Setup Complete",
                    "UnityPhysXFlow is ready!\n\n" +
                    "A volumetric material has been created.\n" +
                    "Assign it to the FlowGrid component and press Play.",
                    "OK"
                );

                Close();
            }
            else
            {
                FlowSetupMenu.CreateFlowSystem();
                Close();
            }
        }
    }
}
