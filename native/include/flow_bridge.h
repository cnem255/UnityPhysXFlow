#pragma once

#include <stdint.h>

#ifdef _WIN32
  #ifdef FLOW_UNITY_BRIDGE_EXPORTS
    #define FUB_API __declspec(dllexport)
  #else
    #define FUB_API __declspec(dllimport)
  #endif
#else
  #define FUB_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

// Callback signature for events from Flow side into Unity.
// The bridge will invoke this on a worker thread; Unity API calls
// must be dispatched to the main thread by C# code.
typedef void(*FubEventCallback)(int32_t event_type, const char* json_payload, void* user_data);

// Initialize the bridge. Returns 0 on success.
FUB_API int32_t Fub_Init();

// Register or replace the global event callback. Passing cb == nullptr disables callbacks.
FUB_API void Fub_RegisterCallback(FubEventCallback cb, void* user_data);

// Advance simulation (or pump events) by dt seconds. In stub mode, this just emits a test event when dt>0.
FUB_API void Fub_Step(float dt);

// Emit a test event immediately (useful to validate wiring without Flow SDK).
FUB_API void Fub_EmitTestEvent(const char* message);

// Shutdown and cleanup resources.
FUB_API void Fub_Shutdown();

#ifdef _WIN32
// Optional: set a folder to search for nvflow.dll and nvflowext.dll at runtime (call before Fub_Init).
FUB_API void Fub_SetDllDirectoryW(const wchar_t* path);
#endif

#ifdef __cplusplus
}
#endif
