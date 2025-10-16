// Ensure export decorations for definitions in this TU during analysis
#ifndef FLOW_UNITY_BRIDGE_EXPORTS
#define FLOW_UNITY_BRIDGE_EXPORTS 1
#endif

#ifdef _WIN32
#define NOMINMAX  // Prevent Windows min/max macros
#endif

#include "../include/UnityPhysXFlow.h"

#include <mutex>
#include <string>
#include <cstdio>
#include <cmath>
#include <algorithm>

#ifdef _OPENMP
#include <omp.h>
#endif

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
    // Temp buffers for multi-threaded simulation
    std::vector<float> densityTemp;
    std::vector<float> velocityTemp;
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
    size_t numCells = sizeX * sizeY * sizeZ;
    g.densityData.resize(numCells, 0.0f);
    g.velocityData.resize(numCells * 3, 0.0f); // 3 components per cell
    g.densityTemp.resize(numCells, 0.0f);
    g.velocityTemp.resize(numCells * 3, 0.0f);

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
    if (dt <= 0.f) dt = 0.016f;

    GridState& grid = it->second;
    const int sX = grid.sizeX, sY = grid.sizeY, sZ = grid.sizeZ;
    const float cs = grid.cellSize;
    
    const float halfX = sX * cs * 0.5f;
    const float halfY = sY * cs * 0.5f;
    const float halfZ = sZ * cs * 0.5f;
    
    // Improved simulation parameters for continuous motion
    const float buoyancy = 5.0f;           // Stronger upward force (increased)
    const float dissipation = 0.99f;       // Slower dissipation to maintain density
    const float velocityDamping = 0.995f;  // Minimal damping for continuous flow
    const float emitterStrength = 5.0f;    // Stronger continuous emission
    const float emitterVelocity = 8.0f;    // Strong initial velocity impulse
    const float densityThreshold = 0.001f; // Clear very low density
    
    dt = std::min(dt, 0.033f);
    
    // Step 1: Add emitter sources (PARALLELIZED)
    for (const auto& pair : g_state.emitters) {
        const EmitterState& emitter = pair.second;
        
        float emitterGridX = (emitter.x + halfX) / cs;
        float emitterGridY = (emitter.y + halfY) / cs;
        float emitterGridZ = (emitter.z + halfZ) / cs;
        float radiusInCells = emitter.radius / cs;
        
        int minX = std::max(0, (int)(emitterGridX - radiusInCells) - 1);
        int maxX = std::min(sX - 1, (int)(emitterGridX + radiusInCells) + 1);
        int minY = std::max(0, (int)(emitterGridY - radiusInCells) - 1);
        int maxY = std::min(sY - 1, (int)(emitterGridY + radiusInCells) + 1);
        int minZ = std::max(0, (int)(emitterGridZ - radiusInCells) - 1);
        int maxZ = std::min(sZ - 1, (int)(emitterGridZ + radiusInCells) + 1);
        
        const float radiusSq = radiusInCells * radiusInCells;
        
        // Parallelize the emitter loop
        #pragma omp parallel for collapse(3) if(maxZ - minZ > 4)
        for (int z = minZ; z <= maxZ; z++) {
            for (int y = minY; y <= maxY; y++) {
                for (int x = minX; x <= maxX; x++) {
                    float dx = x - emitterGridX;
                    float dy = y - emitterGridY;
                    float dz = z - emitterGridZ;
                    float distSq = dx*dx + dy*dy + dz*dz;
                    
                    if (distSq < radiusSq) {
                        int idx = x + y * sX + z * sX * sY;
                        float dist = std::sqrt(distSq);
                        float falloff = 1.0f - (dist / radiusInCells);
                        falloff = falloff * falloff * falloff;
                        
                        // Add density (use emitterStrength to control rate)
                        #pragma omp atomic
                        grid.densityData[idx] += emitter.density * falloff * dt * emitterStrength;
                        
                        // Add upward velocity impulse (stronger for continuous motion)
                        #pragma omp atomic
                        grid.velocityData[idx * 3 + 1] += falloff * emitterVelocity * dt;
                    }
                }
            }
        }
    }
    
    // Step 2: Advection (PARALLELIZED)
    std::copy(grid.densityData.begin(), grid.densityData.end(), grid.densityTemp.begin());
    std::copy(grid.velocityData.begin(), grid.velocityData.end(), grid.velocityTemp.begin());
    
    const float dtOverCs = dt / cs;
    
    #pragma omp parallel for collapse(3) if(sZ > 8)
    for (int z = 2; z < sZ - 2; z++) {
        for (int y = 2; y < sY - 2; y++) {
            for (int x = 2; x < sX - 2; x++) {
                int idx = x + y * sX + z * sX * sY;
                int vidx = idx * 3;
                
                float px = x - grid.velocityTemp[vidx + 0] * dtOverCs;
                float py = y - grid.velocityTemp[vidx + 1] * dtOverCs;
                float pz = z - grid.velocityTemp[vidx + 2] * dtOverCs;
                
                px = std::max(1.5f, std::min(px, (float)(sX - 2.5f)));
                py = std::max(1.5f, std::min(py, (float)(sY - 2.5f)));
                pz = std::max(1.5f, std::min(pz, (float)(sZ - 2.5f)));
                
                int x0 = (int)px, y0 = (int)py, z0 = (int)pz;
                float fx = px - x0, fy = py - y0, fz = pz - z0;
                
                int i000 = x0 + y0 * sX + z0 * sX * sY;
                int i100 = i000 + 1;
                int i010 = i000 + sX;
                int i110 = i000 + sX + 1;
                int i001 = i000 + sX * sY;
                int i101 = i001 + 1;
                int i011 = i001 + sX;
                int i111 = i001 + sX + 1;
                
                // Trilinear interpolation for density
                float d000 = grid.densityTemp[i000];
                float d100 = grid.densityTemp[i100];
                float d010 = grid.densityTemp[i010];
                float d110 = grid.densityTemp[i110];
                float d001 = grid.densityTemp[i001];
                float d101 = grid.densityTemp[i101];
                float d011 = grid.densityTemp[i011];
                float d111 = grid.densityTemp[i111];
                
                float d00 = d000 + (d100 - d000) * fx;
                float d10 = d010 + (d110 - d010) * fx;
                float d01 = d001 + (d101 - d001) * fx;
                float d11 = d011 + (d111 - d011) * fx;
                float d0 = d00 + (d10 - d00) * fy;
                float d1 = d01 + (d11 - d01) * fy;
                
                grid.densityData[idx] = (d0 + (d1 - d0) * fz) * dissipation;
                
                // Velocity self-advection
                for (int c = 0; c < 3; c++) {
                    float v000 = grid.velocityTemp[i000 * 3 + c];
                    float v100 = grid.velocityTemp[i100 * 3 + c];
                    float v010 = grid.velocityTemp[i010 * 3 + c];
                    float v110 = grid.velocityTemp[i110 * 3 + c];
                    float v001 = grid.velocityTemp[i001 * 3 + c];
                    float v101 = grid.velocityTemp[i101 * 3 + c];
                    float v011 = grid.velocityTemp[i011 * 3 + c];
                    float v111 = grid.velocityTemp[i111 * 3 + c];
                    
                    float v00 = v000 + (v100 - v000) * fx;
                    float v10 = v010 + (v110 - v010) * fx;
                    float v01 = v001 + (v101 - v001) * fx;
                    float v11 = v011 + (v111 - v011) * fx;
                    float v0 = v00 + (v10 - v00) * fy;
                    float v1 = v01 + (v11 - v01) * fy;
                    
                    grid.velocityData[vidx + c] = (v0 + (v1 - v0) * fz) * velocityDamping;
                }
            }
        }
    }
    
    // Step 3: Buoyancy (PARALLELIZED)
    #pragma omp parallel for collapse(3) if(sZ > 8)
    for (int z = 0; z < sZ; z++) {
        for (int y = 0; y < sY; y++) {
            for (int x = 0; x < sX; x++) {
                int idx = x + y * sX + z * sX * sY;
                float density = grid.densityData[idx];
                
                // Apply buoyancy force (lower threshold for better response)
                if (density > 0.001f) {
                    grid.velocityData[idx * 3 + 1] += density * buoyancy * dt;
                }
            }
        }
    }
    
    // Step 4: Clamp and clear low density values (PARALLELIZED)
    const int numCells = (int)grid.densityData.size();
    #pragma omp parallel for if(numCells > 1000)
    for (int i = 0; i < numCells; i++) {
        // Clamp density
        if (grid.densityData[i] > 10.0f) {
            grid.densityData[i] = 10.0f;
        } else if (grid.densityData[i] < densityThreshold) {
            grid.densityData[i] = 0.0f;  // Clear very low density to prevent accumulation
        }
        
        // Clamp velocity to prevent instability
        int vidx = i * 3;
        for (int c = 0; c < 3; c++) {
            if (grid.velocityData[vidx + c] > 20.0f) grid.velocityData[vidx + c] = 20.0f;
            else if (grid.velocityData[vidx + c] < -20.0f) grid.velocityData[vidx + c] = -20.0f;
        }
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
