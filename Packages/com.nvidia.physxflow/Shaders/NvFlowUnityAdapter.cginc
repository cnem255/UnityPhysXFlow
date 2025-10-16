// Unity-compatible adapter for NVIDIA Flow shaders
// Bridges Unity's shader system with NVIDIA Flow HLSL code

#ifndef NVFLOW_UNITY_ADAPTER_CGINC
#define NVFLOW_UNITY_ADAPTER_CGINC

#include "UnityCG.cginc"

// Unity uses different types - create aliases
#define NvFlowUint uint
#define NvFlowInt4 int4
#define NvFlowFloat3 float3
#define NvFlowFloat4 float4
#define NvFlowFloat4x4 float4x4

// Simplified sparse level parameters for Unity
struct NvFlowSparseLevelParams
{
    int3 blockDimLessOne;
    int pad0;
    float3 blockSizeWorld;
    float pad1;
};

// Simplified layer parameters for Unity
struct NvFlowRayMarchLayerParams
{
    float3 blockSizeWorld;
    float minBlockSizeWorld;
    float3 blockSizeWorldInv;
    float maxBlockSizeWorld;
    float3 cellSize;
    float stepSize;
    float3 cellSizeInv;
    float stepSizeInv;
    int4 locationMin;
    int4 locationMax;
    float3 worldMin;
    uint enableBlockWireframe;
    float3 worldMax;
    uint enableRawMode;
    
    float alphaScale;
    float colorScale;
    float densityMultiplier;
    float pad0;
};

// NVIDIA shader compatibility - texture sampling
#define NvFlowSample3D(tex, sampler, uv) tex3D(tex, uv)

// Box intersection test (from NvFlowRayMarchCommon.hlsli)
bool NvFlowIntersectBox(float3 rayDir, float3 rayDirInv, float3 boxMin, float3 boxMax, out float tnear, out float tfar)
{
    // compute intersection of ray with all six bbox planes
    float3 tbot = boxMin * rayDirInv;
    float3 ttop = boxMax * rayDirInv;

    // re-order intersections to find smallest and largest on each axis
    float3 tmin = min(ttop, tbot);
    float3 tmax = max(ttop, tbot);

    // find the largest tmin and the smallest tmax
    tnear = max(max(tmin.x, tmin.y), max(tmin.x, tmin.z));
    tfar = min(min(tmax.x, tmax.y), min(tmax.x, tmax.z));

    return tfar > tnear;
}

// Hash function for noise (from NvFlowRayMarchCommon.hlsli)
uint NvFlowRayMarchHash(uint inputValue)
{
    uint state = inputValue * 747796405u + 2891336453u;
    uint word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

float NvFlowRayMarchRandNorm(uint inputValue)
{
    return float(NvFlowRayMarchHash(inputValue) & 0xFFFF) * (1.0 / 65535.0);
}

// Generate noise from ray direction
float NvFlowRayMarchNoiseFromDir(float3 rayDir)
{
    float2 uv;
    if (abs(rayDir.x) > abs(rayDir.y) && abs(rayDir.x) > abs(rayDir.z))
    {
        uv = rayDir.yz;
    }
    else if (abs(rayDir.y) > abs(rayDir.x) && abs(rayDir.y) > abs(rayDir.z))
    {
        uv = rayDir.xz;
    }
    else
    {
        uv = rayDir.xy;
    }
    float maxAxis = max(abs(rayDir.x), max(abs(rayDir.y), abs(rayDir.z)));
    if (maxAxis > 0.0)
    {
        uv *= (1.0 / maxAxis);
    }
    uv = 0.5 * uv + 0.5;
    uint hashInput = uint(65535.0 * uv.x) ^ (uint(65535.0 * uv.y) << 16u);
    return NvFlowRayMarchRandNorm(hashInput);
}

// Simplified volumetric raymarching inspired by NVIDIA Flow
// This is a Unity-friendly adaptation of the full NVIDIA raymarcher
float4 NvFlowRayMarchVolume(
    sampler3D densityTex,
    sampler3D velocityTex,
    float3 rayOrigin,
    float3 rayDir,
    float rayMinT,
    float rayMaxT,
    float stepSize,
    int maxSteps,
    float densityScale,
    float4 fluidColor,
    float absorptionScale
)
{
    float3 rayDirInv = 1.0 / (rayDir + 0.0001); // Avoid division by zero
    float3 rayStep = rayDir * stepSize;
    float3 rayPos = rayOrigin + rayDir * rayMinT;
    
    float4 accumulatedColor = float4(0, 0, 0, 0);
    float transmittance = 1.0;
    
    float rayNoise = NvFlowRayMarchNoiseFromDir(rayDir);
    float tOffset = rayNoise * stepSize;
    rayPos += rayDir * tOffset;
    
    float currentT = rayMinT + tOffset;
    
    // Ray marching loop - use [loop] to prevent unrolling
    [loop]
    for (int step = 0; step < maxSteps && currentT < rayMaxT; step++)
    {
        // Check if we're still in valid bounds [0,1]
        if (any(rayPos < 0.0) || any(rayPos > 1.0))
            break;
        
        // Early exit if fully opaque
        if (transmittance < 0.01)
            break;
        
        // Sample density
        float density = tex3Dlod(densityTex, float4(rayPos, 0)).r * densityScale;
        
        if (density > 0.001)
        {
            // Calculate absorption
            float absorption = density * stepSize * absorptionScale;
            absorption = saturate(absorption);
            
            // Get velocity for color modulation (optional visual effect)
            float3 velocity = tex3Dlod(velocityTex, float4(rayPos, 0)).rgb;
            float velocityMag = length(velocity);
            
            // Color contribution
            float3 sampleColor = fluidColor.rgb;
            
            // Optional: modulate by velocity for visual interest
            sampleColor = lerp(sampleColor, sampleColor * 1.5, saturate(velocityMag * 0.5));
            
            // Front-to-back compositing
            float alpha = (1.0 - exp(-absorption));
            accumulatedColor.rgb += transmittance * alpha * sampleColor;
            accumulatedColor.a += transmittance * alpha;
            transmittance *= (1.0 - alpha);
        }
        
        // Advance ray
        rayPos += rayStep;
        currentT += stepSize;
    }
    
    return accumulatedColor;
}

#endif // NVFLOW_UNITY_ADAPTER_CGINC
