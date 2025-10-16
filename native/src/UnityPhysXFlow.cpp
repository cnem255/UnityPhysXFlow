// Ensure export decorations for definitions in this TU during analysis
#ifndef FLOW_UNITY_BRIDGE_EXPORTS
#define FLOW_UNITY_BRIDGE_EXPORTS 1
#endif

#include "../include/UnityPhysXFlow.h"

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

#include <unordered_map>
#include <vector>

struct EmitterState {
    int32_t handle;
    float x, y, z;
    float radius;
    float density;
    // Flow-specific emitter data would go here
};

struct GridState {
    int32_t handle;
    int sizeX, sizeY, sizeZ;
    float cellSize;
    std::vector<float> densityData;
    std::vector<float> velocityData; // 3 floats per cell (vx, vy, vz)
    // Flow-specific grid data would go here
};

struct BridgeState {
    NvFlowLoader loader{};
    NvFlowDeviceManager* deviceManager = nullptr;
    NvFlowDevice* device = nullptr;
    NvFlowDeviceQueue* queue = nullptr;
    NvFlowContextInterface* ctxIface = nullptr;
    NvFlowContext* context = nullptr;

    UpfEventCallback callback = nullptr;
    void* callbackUser = nullptr;

    std::mutex mtx;
    bool initialized = false;

#ifdef _WIN32
    std::wstring dllDirW;
#endif

    // Simulation state
    int32_t nextEmitterHandle = 1;
    int32_t nextGridHandle = 1;
    std::unordered_map<int32_t, EmitterState> emitters;
    std::unordered_map<int32_t, GridState> grids;
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
    std::string json = "{\"captureId\":" + std::to_string((unsigned long long)captureID) + ",\"entries\":[";
    for (NvFlowUint i = 0; i < numEntries; i++) {
        const NvFlowProfilerEntry& e = entries[i];
        json += "{\"label\":\"";
        if (e.label) {
            const char* p = e.label;
            while (*p) { if (*p == '"') json += '\\'; json += *p++; }
        } else { json += ""; }
        json += "\",\"cpuMs\":" + std::to_string((double)e.cpuDeltaTime) + ",\"gpuMs\":" + std::to_string((double)e.gpuDeltaTime) + "}";
        if (i + 1u < numEntries) json += ",";
    }
    json += "]}";
    g_state.callback(2, json.c_str(), g_state.callbackUser);
}

extern "C" {

UPF_API int32_t Upf_Init()
{
    std::lock_guard<std::mutex> lock(g_state.mtx);
    if (g_state.initialized) return 0;

    NvFlowLoaderInitDeviceAPI(&g_state.loader, flowPrintError, nullptr, eNvFlowContextApi_vulkan);
    if (!g_state.loader.deviceInterface.getContext) {
        flowPrintError("NvFlow device interface not available", nullptr);
        return -1;
    }

    NvFlowThreadPoolInterface* threadPool = nullptr;
    g_state.deviceManager = g_state.loader.deviceInterface.createDeviceManager(0, threadPool, 0);
    if (!g_state.deviceManager) { flowPrintError("Failed to create NvFlowDeviceManager", nullptr); return -2; }

    NvFlowDeviceDesc devDesc{};
    devDesc.deviceIndex = 0;
    devDesc.enableExternalUsage = NV_FLOW_FALSE;
    devDesc.logPrint = nullptr;

    g_state.device = g_state.loader.deviceInterface.createDevice(g_state.deviceManager, &devDesc);
    if (!g_state.device) { flowPrintError("Failed to create NvFlowDevice", nullptr); return -3; }

    g_state.queue = g_state.loader.deviceInterface.getDeviceQueue(g_state.device);
    if (!g_state.queue) { flowPrintError("Failed to get NvFlowDeviceQueue", nullptr); return -4; }

    g_state.ctxIface = g_state.loader.deviceInterface.getContextInterface(g_state.queue);
    g_state.context = g_state.loader.deviceInterface.getContext(g_state.queue);
    if (!g_state.ctxIface || !g_state.context) { flowPrintError("Failed to get NvFlow Context", nullptr); return -5; }

    g_state.loader.deviceInterface.enableProfiler(g_state.context, nullptr, &flowProfilerReport);

    g_state.initialized = true;
    return 0;
}

UPF_API void Upf_RegisterCallback(UpfEventCallback cb, void* user_data)
{
    std::lock_guard<std::mutex> lock(g_state.mtx);
    g_state.callback = cb;
    g_state.callbackUser = user_data;
}

UPF_API void Upf_Step(float dt)
{
    if (!g_state.initialized) return;
    if (dt < 0.f) dt = 0.f;

    uint64_t flushedFrame = 0;
    g_state.loader.deviceInterface.flush(g_state.queue, &flushedFrame, nullptr, nullptr);

    if (g_state.callback) {
        char msg[128];
        snprintf(msg, sizeof(msg), "flushedFrame=%llu, dt=%f", (unsigned long long)flushedFrame, dt);
        g_state.callback(0, msg, g_state.callbackUser);
    }
}

UPF_API void Upf_EmitTestEvent(const char* message)
{
    if (g_state.callback) {
        g_state.callback(99, message ? message : "", g_state.callbackUser);
    }
}

UPF_API void Upf_Shutdown()
{
    std::lock_guard<std::mutex> lock(g_state.mtx);
    if (!g_state.initialized) return;

    if (g_state.deviceManager && g_state.device) {
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
UPF_API void Upf_SetDllDirectoryW(const wchar_t* path)
{
    std::lock_guard<std::mutex> lock(g_state.mtx);
    if (!path) return;
    g_state.dllDirW = path;
    ::SetDefaultDllDirectories(LOAD_LIBRARY_SEARCH_DEFAULT_DIRS | LOAD_LIBRARY_SEARCH_USER_DIRS);
    ::AddDllDirectory(g_state.dllDirW.c_str());
}
#endif

// --- Simulation API Implementation ---

UPF_API int32_t Upf_CreateEmitter(float x, float y, float z, float radius, float density)
{
    std::lock_guard<std::mutex> lock(g_state.mtx);
    if (!g_state.initialized) return -1;

    int32_t handle = g_state.nextEmitterHandle++;
    EmitterState& e = g_state.emitters[handle];
    e.handle = handle;
    e.x = x; e.y = y; e.z = z;
    e.radius = radius;
    e.density = density;

    // TODO: Create actual Flow emitter using NvFlowExt or Context API

    return handle;
}

UPF_API void Upf_DestroyEmitter(int32_t emitterHandle)
{
    std::lock_guard<std::mutex> lock(g_state.mtx);
    auto it = g_state.emitters.find(emitterHandle);
    if (it == g_state.emitters.end()) return;

    // TODO: Destroy Flow emitter

    g_state.emitters.erase(it);
}

UPF_API void Upf_SetEmitterParams(int32_t emitterHandle, float x, float y, float z, float radius, float density)
{
    std::lock_guard<std::mutex> lock(g_state.mtx);
    auto it = g_state.emitters.find(emitterHandle);
    if (it == g_state.emitters.end()) return;

    EmitterState& e = it->second;
    e.x = x; e.y = y; e.z = z;
    e.radius = radius;
    e.density = density;

    // TODO: Update Flow emitter parameters
}

UPF_API int32_t Upf_CreateGrid(int sizeX, int sizeY, int sizeZ, float cellSize)
{
    std::lock_guard<std::mutex> lock(g_state.mtx);
    if (!g_state.initialized) return -1;

    int32_t handle = g_state.nextGridHandle++;
    GridState& g = g_state.grids[handle];
    g.handle = handle;
    g.sizeX = sizeX; g.sizeY = sizeY; g.sizeZ = sizeZ;
    g.cellSize = cellSize;
    g.densityData.resize(sizeX * sizeY * sizeZ, 0.0f);
    g.velocityData.resize(sizeX * sizeY * sizeZ * 3, 0.0f); // 3 components per cell

    // TODO: Create actual Flow grid using NvFlowExt or Context API

    return handle;
}

UPF_API void Upf_DestroyGrid(int32_t gridHandle)
{
    std::lock_guard<std::mutex> lock(g_state.mtx);
    auto it = g_state.grids.find(gridHandle);
    if (it == g_state.grids.end()) return;

    // TODO: Destroy Flow grid

    g_state.grids.erase(it);
}

UPF_API void Upf_StepGrid(int32_t gridHandle, float dt)
{
    std::lock_guard<std::mutex> lock(g_state.mtx);
    auto it = g_state.grids.find(gridHandle);
    if (it == g_state.grids.end()) return;

    // TODO: Step Flow simulation for specific grid

    // For now, just invoke global step
    if (dt > 0.f) {
        // Placeholder: actual Flow simulation step would go here
    }
}

UPF_API const void* Upf_ExportGridDensity(int32_t gridHandle, int* outSizeX, int* outSizeY, int* outSizeZ, int* outFormat)
{
    std::lock_guard<std::mutex> lock(g_state.mtx);
    auto it = g_state.grids.find(gridHandle);
    if (it == g_state.grids.end()) return nullptr;

    GridState& g = it->second;
    if (outSizeX) *outSizeX = g.sizeX;
    if (outSizeY) *outSizeY = g.sizeY;
    if (outSizeZ) *outSizeZ = g.sizeZ;
    if (outFormat) *outFormat = 0; // 0 = float32

    // TODO: Export actual Flow grid density data
    return g.densityData.data();
}

UPF_API const void* Upf_ExportGridVelocity(int32_t gridHandle, int* outSizeX, int* outSizeY, int* outSizeZ, int* outFormat)
{
    std::lock_guard<std::mutex> lock(g_state.mtx);
    auto it = g_state.grids.find(gridHandle);
    if (it == g_state.grids.end()) return nullptr;

    GridState& g = it->second;
    if (outSizeX) *outSizeX = g.sizeX;
    if (outSizeY) *outSizeY = g.sizeY;
    if (outSizeZ) *outSizeZ = g.sizeZ;
    if (outFormat) *outFormat = 1; // 1 = float32x3 (vec3)

    // TODO: Export actual Flow grid velocity data
    return g.velocityData.data();
}

} // extern "C"
