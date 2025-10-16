using System;
using System.IO;
using UnityEditor;
using UnityEngine;

namespace NvidiaPhysXFlow.Editor
{
    /// <summary>
    /// Automatically installs Flow runtime DLLs when the package is imported.
    /// Copies nvflow.dll and nvflowext.dll to the Unity Editor directory for seamless development.
    /// </summary>
    [InitializeOnLoad]
    public class FlowDllInstaller
    {
        private const string PREF_KEY = "NvidiaPhysXFlow.DllsInstalled";
        private const string VERSION_KEY = "NvidiaPhysXFlow.InstalledVersion";
        private const string CURRENT_VERSION = "1.0.0";

        static FlowDllInstaller()
        {
            // Check if DLLs need to be installed or updated
            if (ShouldInstallDlls())
            {
                EditorApplication.delayCall += () =>
                {
                    // Delay to ensure package is fully imported
                    EditorApplication.delayCall += InstallDllsAutomatically;
                };
            }
        }

        private static bool ShouldInstallDlls()
        {
            // Check if never installed
            if (!EditorPrefs.HasKey(PREF_KEY))
                return true;

            // Check if version changed
            string installedVersion = EditorPrefs.GetString(VERSION_KEY, "0.0.0");
            if (installedVersion != CURRENT_VERSION)
                return true;

            // Check if DLLs exist in Unity Editor directory
            string editorPath = Path.GetDirectoryName(EditorApplication.applicationPath);
            string nvflowPath = Path.Combine(editorPath, "nvflow.dll");
            string nvflowextPath = Path.Combine(editorPath, "nvflowext.dll");

            if (!File.Exists(nvflowPath) || !File.Exists(nvflowextPath))
                return true;

            return false;
        }

        private static void InstallDllsAutomatically()
        {
            try
            {
                var result = InstallFlowDlls(silent: false);
                if (result)
                {
                    EditorPrefs.SetBool(PREF_KEY, true);
                    EditorPrefs.SetString(VERSION_KEY, CURRENT_VERSION);
                    Debug.Log("[PhysX Flow] ✅ Flow runtime DLLs installed successfully! You can now use PhysX Flow in your project.");
                }
            }
            catch (Exception ex)
            {
                Debug.LogError($"[PhysX Flow] Failed to auto-install DLLs: {ex.Message}");
                ShowManualInstallDialog();
            }
        }

        [MenuItem("Tools/PhysX Flow/Install Flow Runtime DLLs", priority = 100)]
        public static void InstallDllsManually()
        {
            if (InstallFlowDlls(silent: false))
            {
                EditorPrefs.SetBool(PREF_KEY, true);
                EditorPrefs.SetString(VERSION_KEY, CURRENT_VERSION);
                EditorUtility.DisplayDialog(
                    "PhysX Flow - Installation Complete",
                    "Flow runtime DLLs have been successfully installed!\n\n" +
                    "Location: Unity Editor directory\n" +
                    "Files: nvflow.dll, nvflowext.dll\n\n" +
                    "You can now use PhysX Flow in your project.",
                    "OK"
                );
            }
        }

        [MenuItem("Tools/PhysX Flow/Verify DLL Installation", priority = 101)]
        public static void VerifyInstallation()
        {
            string editorPath = Path.GetDirectoryName(EditorApplication.applicationPath);
            string nvflowPath = Path.Combine(editorPath, "nvflow.dll");
            string nvflowextPath = Path.Combine(editorPath, "nvflowext.dll");

            bool nvflowExists = File.Exists(nvflowPath);
            bool nvflowextExists = File.Exists(nvflowextPath);

            string message = "Flow Runtime DLL Status:\n\n";
            message += $"nvflow.dll: {(nvflowExists ? "✅ Installed" : "❌ Missing")}\n";
            message += $"nvflowext.dll: {(nvflowextExists ? "✅ Installed" : "❌ Missing")}\n\n";
            message += $"Location: {editorPath}\n\n";

            if (nvflowExists && nvflowextExists)
            {
                message += "All required DLLs are installed correctly!";
                EditorUtility.DisplayDialog("PhysX Flow - Verification Success", message, "OK");
            }
            else
            {
                message += "Some DLLs are missing. Click 'Install' to install them now.";
                if (EditorUtility.DisplayDialog("PhysX Flow - Installation Required", message, "Install", "Cancel"))
                {
                    InstallDllsManually();
                }
            }
        }

        [MenuItem("Tools/PhysX Flow/Uninstall Flow Runtime DLLs", priority = 102)]
        public static void UninstallDlls()
        {
            if (!EditorUtility.DisplayDialog(
                "PhysX Flow - Uninstall DLLs",
                "This will remove nvflow.dll and nvflowext.dll from your Unity Editor directory.\n\n" +
                "You will need to reinstall them to use PhysX Flow.\n\n" +
                "Continue?",
                "Uninstall",
                "Cancel"))
            {
                return;
            }

            try
            {
                string editorPath = Path.GetDirectoryName(EditorApplication.applicationPath);
                string nvflowPath = Path.Combine(editorPath, "nvflow.dll");
                string nvflowextPath = Path.Combine(editorPath, "nvflowext.dll");

                int removedCount = 0;
                if (File.Exists(nvflowPath))
                {
                    File.Delete(nvflowPath);
                    removedCount++;
                }
                if (File.Exists(nvflowextPath))
                {
                    File.Delete(nvflowextPath);
                    removedCount++;
                }

                EditorPrefs.DeleteKey(PREF_KEY);
                EditorPrefs.DeleteKey(VERSION_KEY);

                EditorUtility.DisplayDialog(
                    "PhysX Flow - Uninstall Complete",
                    $"Removed {removedCount} DLL(s) from Unity Editor directory.",
                    "OK"
                );
            }
            catch (Exception ex)
            {
                EditorUtility.DisplayDialog(
                    "PhysX Flow - Uninstall Failed",
                    $"Failed to remove DLLs:\n\n{ex.Message}",
                    "OK"
                );
            }
        }

        private static bool InstallFlowDlls(bool silent)
        {
            try
            {
                // Find the package directory
                string packagePath = GetPackagePath();
                if (string.IsNullOrEmpty(packagePath))
                {
                    if (!silent)
                        Debug.LogError("[PhysX Flow] Could not locate package directory.");
                    return false;
                }

                // Source DLL paths (in package)
                string sourcePath = Path.Combine(packagePath, "Plugins", "x86_64");
                string nvflowSource = Path.Combine(sourcePath, "nvflow.dll");
                string nvflowextSource = Path.Combine(sourcePath, "nvflowext.dll");

                // Check if source DLLs exist
                if (!File.Exists(nvflowSource) || !File.Exists(nvflowextSource))
                {
                    if (!silent)
                    {
                        Debug.LogError($"[PhysX Flow] Source DLLs not found in package:\n{sourcePath}");
                        ShowManualInstallDialog();
                    }
                    return false;
                }

                // Destination: Unity Editor directory
                string editorPath = Path.GetDirectoryName(EditorApplication.applicationPath);
                string nvflowDest = Path.Combine(editorPath, "nvflow.dll");
                string nvflowextDest = Path.Combine(editorPath, "nvflowext.dll");

                // Check write permissions
                if (!HasWritePermission(editorPath))
                {
                    if (!silent)
                    {
                        EditorUtility.DisplayDialog(
                            "PhysX Flow - Permission Required",
                            "Unity Editor directory requires administrator permissions.\n\n" +
                            "Please run Unity as Administrator and try again, or install DLLs manually.\n\n" +
                            $"Target: {editorPath}",
                            "OK"
                        );
                    }
                    return false;
                }

                // Copy DLLs
                if (!silent)
                    Debug.Log($"[PhysX Flow] Installing Flow runtime DLLs to: {editorPath}");

                File.Copy(nvflowSource, nvflowDest, overwrite: true);
                File.Copy(nvflowextSource, nvflowextDest, overwrite: true);

                if (!silent)
                {
                    Debug.Log("[PhysX Flow] ✅ nvflow.dll installed");
                    Debug.Log("[PhysX Flow] ✅ nvflowext.dll installed");
                }

                return true;
            }
            catch (UnauthorizedAccessException)
            {
                if (!silent)
                {
                    EditorUtility.DisplayDialog(
                        "PhysX Flow - Access Denied",
                        "Could not install DLLs due to insufficient permissions.\n\n" +
                        "Please run Unity as Administrator and try again.",
                        "OK"
                    );
                }
                return false;
            }
            catch (Exception ex)
            {
                if (!silent)
                {
                    Debug.LogError($"[PhysX Flow] Installation failed: {ex.Message}");
                    ShowManualInstallDialog();
                }
                return false;
            }
        }

        private static string GetPackagePath()
        {
            // Try to find package path via PackageInfo
            var packageInfo = UnityEditor.PackageManager.PackageInfo.FindForAssetPath("Packages/com.nvidia.physxflow");
            if (packageInfo != null)
            {
                return packageInfo.resolvedPath;
            }

            // Fallback: search for the package
            string packagesPath = Path.GetFullPath("Packages");
            string packagePath = Path.Combine(packagesPath, "com.nvidia.physxflow");
            if (Directory.Exists(packagePath))
            {
                return packagePath;
            }

            return null;
        }

        private static bool HasWritePermission(string path)
        {
            try
            {
                string testFile = Path.Combine(path, $"_test_write_{Guid.NewGuid()}.tmp");
                File.WriteAllText(testFile, "test");
                File.Delete(testFile);
                return true;
            }
            catch
            {
                return false;
            }
        }

        private static void ShowManualInstallDialog()
        {
            EditorApplication.delayCall += () =>
            {
                bool result = EditorUtility.DisplayDialog(
                    "PhysX Flow - Manual Installation Required",
                    "Automatic installation failed. Would you like to see manual installation instructions?",
                    "Show Instructions",
                    "Cancel"
                );

                if (result)
                {
                    ShowManualInstructions();
                }
            };
        }

        private static void ShowManualInstructions()
        {
            string instructions = "Manual Installation Instructions:\n\n" +
                "1. Locate the Flow DLLs in your package:\n" +
                "   Packages/com.nvidia.physxflow/Plugins/x86_64/\n\n" +
                "2. Copy these files:\n" +
                "   - nvflow.dll\n" +
                "   - nvflowext.dll\n\n" +
                "3. Paste them into your Unity Editor directory:\n" +
                $"   {Path.GetDirectoryName(EditorApplication.applicationPath)}\n\n" +
                "4. Restart Unity Editor\n\n" +
                "For builds, copy the DLLs next to your game executable.";

            EditorUtility.DisplayDialog(
                "PhysX Flow - Manual Installation",
                instructions,
                "OK"
            );

            Debug.Log($"[PhysX Flow] Manual Installation Instructions:\n{instructions}");
        }

        [MenuItem("Tools/PhysX Flow/Open Package Documentation", priority = 200)]
        public static void OpenDocumentation()
        {
            string packagePath = GetPackagePath();
            if (!string.IsNullOrEmpty(packagePath))
            {
                string readmePath = Path.Combine(packagePath, "INSTALLATION.md");
                if (File.Exists(readmePath))
                {
                    UnityEditorInternal.InternalEditorUtility.OpenFileAtLineExternal(readmePath, 0);
                    return;
                }
            }

            Application.OpenURL("https://github.com/cnem255/UnityPhysXFlow");
        }
    }
}
