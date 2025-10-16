using System.IO;
using UnityEditor;
using UnityEditor.Build;
using UnityEditor.Build.Reporting;
using UnityEngine;

namespace NvidiaPhysXFlow.Editor
{
    /// <summary>
    /// Automatically copies Flow runtime DLLs to the build output directory.
    /// Ensures that built games have the required DLLs next to the executable.
    /// </summary>
    public class FlowBuildPostProcessor : IPostprocessBuildWithReport
    {
        public int callbackOrder => 0;

        public void OnPostprocessBuild(BuildReport report)
        {
            // Only process Windows builds
            if (report.summary.platform != BuildTarget.StandaloneWindows64 &&
                report.summary.platform != BuildTarget.StandaloneWindows)
            {
                Debug.LogWarning("[PhysX Flow] Flow DLLs are only available for Windows. Your build may not work correctly.");
                return;
            }

            try
            {
                CopyFlowDllsToBuild(report.summary.outputPath);
            }
            catch (System.Exception ex)
            {
                Debug.LogError($"[PhysX Flow] Failed to copy Flow DLLs to build: {ex.Message}");
            }
        }

        private void CopyFlowDllsToBuild(string buildPath)
        {
            // Get the build directory (where the .exe is)
            string buildDir = Path.GetDirectoryName(buildPath);

            // Find package path
            var packageInfo = UnityEditor.PackageManager.PackageInfo.FindForAssetPath("Packages/com.nvidia.physxflow");
            if (packageInfo == null)
            {
                Debug.LogError("[PhysX Flow] Could not locate package for build post-processing.");
                return;
            }

            string sourcePath = Path.Combine(packageInfo.resolvedPath, "Plugins", "x86_64");
            string nvflowSource = Path.Combine(sourcePath, "nvflow.dll");
            string nvflowextSource = Path.Combine(sourcePath, "nvflowext.dll");

            // Check if source DLLs exist
            if (!File.Exists(nvflowSource) || !File.Exists(nvflowextSource))
            {
                Debug.LogError($"[PhysX Flow] Flow runtime DLLs not found in package: {sourcePath}");
                return;
            }

            // Copy DLLs to build directory
            string nvflowDest = Path.Combine(buildDir, "nvflow.dll");
            string nvflowextDest = Path.Combine(buildDir, "nvflowext.dll");

            File.Copy(nvflowSource, nvflowDest, overwrite: true);
            File.Copy(nvflowextSource, nvflowextDest, overwrite: true);

            Debug.Log($"[PhysX Flow] ✅ Flow runtime DLLs copied to build: {buildDir}");
            Debug.Log("[PhysX Flow] ✅ nvflow.dll");
            Debug.Log("[PhysX Flow] ✅ nvflowext.dll");
        }
    }

    /// <summary>
    /// Validates that Flow DLLs are present before building.
    /// </summary>
    public class FlowBuildValidator : IPreprocessBuildWithReport
    {
        public int callbackOrder => 0;

        public void OnPreprocessBuild(BuildReport report)
        {
            // Only check Windows builds
            if (report.summary.platform != BuildTarget.StandaloneWindows64 &&
                report.summary.platform != BuildTarget.StandaloneWindows)
            {
                return;
            }

            // Check if Flow DLLs are in the package
            var packageInfo = UnityEditor.PackageManager.PackageInfo.FindForAssetPath("Packages/com.nvidia.physxflow");
            if (packageInfo == null)
            {
                return;
            }

            string sourcePath = Path.Combine(packageInfo.resolvedPath, "Plugins", "x86_64");
            string nvflowSource = Path.Combine(sourcePath, "nvflow.dll");
            string nvflowextSource = Path.Combine(sourcePath, "nvflowext.dll");

            bool hasFlowDlls = File.Exists(nvflowSource) && File.Exists(nvflowextSource);

            if (!hasFlowDlls)
            {
                Debug.LogWarning("[PhysX Flow] Flow runtime DLLs not found in package. Your build may not work correctly.");
                Debug.LogWarning($"[PhysX Flow] Expected location: {sourcePath}");
            }
        }
    }
}
