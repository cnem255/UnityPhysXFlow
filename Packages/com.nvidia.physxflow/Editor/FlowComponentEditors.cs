using UnityEngine;
using UnityEditor;

namespace UnityPhysXFlow.Editor
{
    [CustomEditor(typeof(FlowEmitter))]
    public class FlowEmitterEditor : UnityEditor.Editor
    {
        public override void OnInspectorGUI()
        {
            FlowEmitter emitter = (FlowEmitter)target;

            EditorGUILayout.Space();
            EditorGUILayout.LabelField("PhysX Flow Emitter", EditorStyles.boldLabel);
            EditorGUILayout.HelpBox(
                "Emits fluid particles into the simulation. Position is synced from Transform.", 
                MessageType.Info
            );

            EditorGUILayout.Space();
            DrawDefaultInspector();

            EditorGUILayout.Space();
            
            GUI.enabled = Application.isPlaying;
            EditorGUILayout.BeginHorizontal();
            if (GUILayout.Button("Create Emitter"))
            {
                emitter.CreateEmitter();
            }
            if (GUILayout.Button("Destroy Emitter"))
            {
                emitter.DestroyEmitter();
            }
            EditorGUILayout.EndHorizontal();
            GUI.enabled = true;

            if (!Application.isPlaying)
            {
                EditorGUILayout.HelpBox("Enter Play Mode to create/destroy emitters.", MessageType.Warning);
            }
        }
    }

    [CustomEditor(typeof(FlowGrid))]
    public class FlowGridEditor : UnityEditor.Editor
    {
        public override void OnInspectorGUI()
        {
            FlowGrid grid = (FlowGrid)target;

            EditorGUILayout.Space();
            EditorGUILayout.LabelField("PhysX Flow Grid", EditorStyles.boldLabel);
            EditorGUILayout.HelpBox(
                "Manages a simulation grid. Exports density/velocity as Texture3D for volumetric rendering.", 
                MessageType.Info
            );

            EditorGUILayout.Space();
            DrawDefaultInspector();

            EditorGUILayout.Space();

            // Show grid stats
            int totalCells = grid.sizeX * grid.sizeY * grid.sizeZ;
            float memoryMB = (totalCells * sizeof(float) * 4) / (1024f * 1024f); // density + velocity (vec3)
            EditorGUILayout.LabelField("Grid Stats", EditorStyles.boldLabel);
            EditorGUILayout.LabelField($"Total Cells: {totalCells:N0}");
            EditorGUILayout.LabelField($"Estimated Memory: {memoryMB:F2} MB");
            Vector3 worldSize = new Vector3(grid.sizeX * grid.cellSize, grid.sizeY * grid.cellSize, grid.sizeZ * grid.cellSize);
            EditorGUILayout.LabelField($"World Size: {worldSize}");

            EditorGUILayout.Space();
            
            GUI.enabled = Application.isPlaying;
            EditorGUILayout.BeginHorizontal();
            if (GUILayout.Button("Create Grid"))
            {
                grid.CreateGrid();
            }
            if (GUILayout.Button("Destroy Grid"))
            {
                grid.DestroyGrid();
            }
            EditorGUILayout.EndHorizontal();
            GUI.enabled = true;

            if (!Application.isPlaying)
            {
                EditorGUILayout.HelpBox("Enter Play Mode to create/destroy grids.", MessageType.Warning);
            }

            // Validate material
            if (grid.volumetricMaterial == null)
            {
                EditorGUILayout.HelpBox(
                    "No volumetric material assigned. Create a material using PhysXFlow/VolumetricFluid shader.", 
                    MessageType.Warning
                );
            }
        }
    }

    [CustomEditor(typeof(UnityPhysXFlowExample))]
    public class UnityPhysXFlowExampleEditor : UnityEditor.Editor
    {
        public override void OnInspectorGUI()
        {
            UnityPhysXFlowExample example = (UnityPhysXFlowExample)target;

            EditorGUILayout.Space();
            EditorGUILayout.LabelField("UnityPhysXFlow Example", EditorStyles.boldLabel);
            EditorGUILayout.HelpBox(
                "Initializes the PhysX Flow bridge. Add this to one GameObject in your scene.", 
                MessageType.Info
            );

            EditorGUILayout.Space();
            DrawDefaultInspector();

            EditorGUILayout.Space();

            if (string.IsNullOrEmpty(example.flowDllPath))
            {
                EditorGUILayout.HelpBox(
                    "Flow DLL path not set. Ensure nvflow.dll and nvflowext.dll are in your Unity project or specify the path.", 
                    MessageType.Warning
                );
            }

            if (example.createDemoScene && example.volumetricMaterial == null)
            {
                EditorGUILayout.HelpBox(
                    "Demo scene is enabled but no volumetric material assigned. Fluids won't be visible.", 
                    MessageType.Warning
                );
            }
        }
    }
}
