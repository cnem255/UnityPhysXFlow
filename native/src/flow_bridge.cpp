// Ensure export decorations for definitions in this TU during analysis
#ifndef FLOW_UNITY_BRIDGE_EXPORTS
#define FLOW_UNITY_BRIDGE_EXPORTS 1
#endif

// Local API
#include "../include/flow_bridge.h"

#include <mutex>
#include <string>
#include <cstdio>

// NvFlow SDK
#include "NvFlowContext.h"
#include "NvFlowExt.h"
#include "NvFlowLoader.h"

#ifdef _WIN32
#include <windows.h>
#endif

struct BridgeState {
    // Loader and primary device/context
    NvFlowLoader loader{};
    NvFlowDeviceManager* deviceManager = nullptr;
    NvFlowDevice* device = nullptr;
    NvFlowDeviceQueue* queue = nullptr;
    NvFlowContextInterface* ctxIface = nullptr;
    NvFlowContext* context = nullptr;

    // Callback into Unity
    FubEventCallback callback = nullptr;
    void* callbackUser = nullptr;

    std::mutex mtx;
    bool initialized = false;

    // DLL directory for runtime loading (optional)
#ifdef _WIN32
    std::wstring dllDirW;
#endif
};

static BridgeState g_state;

static void flowPrintError(const char* str, void* /*userdata*/)
{
    if (g_state.callback) {
        g_state.callback(1, str, g_state.callbackUser);
    }
}

static void NV_FLOW_ABI flowProfilerReport(void* /*userdata*/, NvFlowUint64 captureID, NvFlowUint numEntries, NvFlowProfilerEntry* entries)
{
    if (!g_state.callback) return;
    // Emit a compact JSON array of entries
    std::string json = "{\"captureId\":" + std::to_string((unsigned long long)captureID) + ",\"entries\":[";
    for (NvFlowUint i = 0; i < numEntries; i++) {
        const NvFlowProfilerEntry& e = entries[i];
        json += "{\"label\":\"";
        if (e.label) {
            // naive escape of quotes
            const char* p = e.label;
            while (*p) { if (*p == '"') json += '\\'; json += *p++; }
        }
        else { json += ""; }
        json += "\",\"cpuMs\":" + std::to_string((double)e.cpuDeltaTime) + ",\"gpuMs\":" + std::to_string((double)e.gpuDeltaTime) + "}";
        if (i + 1u < numEntries) json += ",";
    }
    json += "]}";
    g_state.callback(2, json.c_str(), g_state.callbackUser);
}

extern "C" {

FUB_API int32_t Fub_Init()
{
    std::lock_guard<std::mutex> lock(g_state.mtx);
    if (g_state.initialized) return 0;

#ifdef _WIN32
    // Optionally extend DLL search path if user set it earlier via Fub_SetDllDirectoryW
    // NvFlowLoader uses LoadLibrary("nvflow.dll") and LoadLibrary("nvflowext.dll")
#endif

    // Load Vulkan backend by default (as Flow editor does). Unity typically uses D3D12 on Windows,
    // but Flow is self-contained; we don't need Unity device interop for callbacks.
    NvFlowLoaderInitDeviceAPI(&g_state.loader, flowPrintError, nullptr, eNvFlowContextApi_vulkan);

    if (!g_state.loader.deviceInterface.getContext) {
        flowPrintError("NvFlow device interface not available", nullptr);
        return -1;
    }

    // Create device manager and device
    NvFlowThreadPoolInterface* threadPool = nullptr; // null -> internal
    g_state.deviceManager = g_state.loader.deviceInterface.createDeviceManager(/*enableValidationOnDebugBuild*/ 0, threadPool, 0);
    if (!g_state.deviceManager) {
        flowPrintError("Failed to create NvFlowDeviceManager", nullptr);
        return -2;
    }

    NvFlowDeviceDesc devDesc{};
    devDesc.deviceIndex = 0;
    devDesc.enableExternalUsage = NV_FLOW_FALSE;
    devDesc.logPrint = nullptr;

    g_state.device = g_state.loader.deviceInterface.createDevice(g_state.deviceManager, &devDesc);
    if (!g_state.device) {
        flowPrintError("Failed to create NvFlowDevice", nullptr);
        return -3;
    }

    g_state.queue = g_state.loader.deviceInterface.getDeviceQueue(g_state.device);
    if (!g_state.queue) {
        flowPrintError("Failed to get NvFlowDeviceQueue", nullptr);
        return -4;
    }

    g_state.ctxIface = g_state.loader.deviceInterface.getContextInterface(g_state.queue);
    g_state.context = g_state.loader.deviceInterface.getContext(g_state.queue);
    if (!g_state.ctxIface || !g_state.context) {
        flowPrintError("Failed to get NvFlow Context", nullptr);
        return -5;
    }

    // Enable profiler to demonstrate callbacks from within Flow execution
    g_state.loader.deviceInterface.enableProfiler(g_state.context, nullptr, &flowProfilerReport);

    g_state.initialized = true;
    return 0;
}

FUB_API void Fub_RegisterCallback(FubEventCallback cb, void* user_data)
{
    std::lock_guard<std::mutex> lock(g_state.mtx);
    g_state.callback = cb;
    g_state.callbackUser = user_data;
}

FUB_API void Fub_Step(float dt)
{
    if (!g_state.initialized) return;
    if (dt < 0.f) dt = 0.f;

    // For now, just flush the device queue to advance Flow frame counters.
    uint64_t flushedFrame = 0;
    g_state.loader.deviceInterface.flush(g_state.queue, &flushedFrame, nullptr, nullptr);

    if (g_state.callback) {
        char msg[128];
        snprintf(msg, sizeof(msg), "flushedFrame=%llu, dt=%f", (unsigned long long)flushedFrame, dt);
        g_state.callback(0, msg, g_state.callbackUser);
    }
}

FUB_API void Fub_EmitTestEvent(const char* message)
{
    if (g_state.callback) {
        g_state.callback(99, message ? message : "", g_state.callbackUser);
    }
}

FUB_API void Fub_Shutdown()
{
    std::lock_guard<std::mutex> lock(g_state.mtx);
    if (!g_state.initialized) return;

    if (g_state.deviceManager && g_state.device) {
        // Ensure the queue is idle before teardown
        g_state.loader.deviceInterface.waitIdle(g_state.queue);
        g_state.loader.deviceInterface.destroyDevice(g_state.deviceManager, g_state.device);
        g_state.device = nullptr;
        g_state.queue = nullptr;
    }
    if (g_state.deviceManager) {
        g_state.loader.deviceInterface.destroyDeviceManager(g_state.deviceManager);
        g_state.deviceManager = nullptr;
    }

    NvFlowLoaderDestroy(&g_state.loader);

    g_state.ctxIface = nullptr;
    g_state.context = nullptr;
    g_state.callback = nullptr;
    g_state.callbackUser = nullptr;
    g_state.initialized = false;
}

#ifdef _WIN32
// Optional utility: set a custom directory where nvflow.dll and nvflowext.dll reside
FUB_API void Fub_SetDllDirectoryW(const wchar_t* path)
{
    std::lock_guard<std::mutex> lock(g_state.mtx);
    if (!path) return;
    g_state.dllDirW = path;
    // Prepend to DLL search path for the current process
    ::SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS | LOAD_LIBRARY_SEARCH_USER_DIRS);
    ::AddDllDirectory(g_state.dllDirW.c_str());
}
#endif

} // extern "C"
