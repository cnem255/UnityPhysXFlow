#pragma once

#include <stdint.h>

#ifdef _WIN32
  #ifdef FLOW_UNITY_BRIDGE_EXPORTS
    #define UPF_API __declspec(dllexport)
  #else
    #define UPF_API __declspec(dllimport)
  #endif
#else
  #define UPF_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Callback signature for events from Flow side into Unity.
typedef void(*UpfEventCallback)(int32_t event_type, const char* json_payload, void* user_data);

// Initialize the bridge. Returns 0 on success.
UPF_API int32_t Upf_Init();

// Register or replace the global event callback.
UPF_API void Upf_RegisterCallback(UpfEventCallback cb, void* user_data);

// Advance/pump events.
UPF_API void Upf_Step(float dt);

// Emit a test event immediately.
UPF_API void Upf_EmitTestEvent(const char* message);

// Shutdown and cleanup resources.
UPF_API void Upf_Shutdown();

#ifdef _WIN32
// Optional: set a folder to search for nvflow.dll and nvflowext.dll at runtime (call before Upf_Init).
UPF_API void Upf_SetDllDirectoryW(const wchar_t* path);
#endif

#ifdef __cplusplus
}
#endif
