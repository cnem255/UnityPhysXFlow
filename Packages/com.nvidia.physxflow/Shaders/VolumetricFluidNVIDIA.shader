Shader "PhysXFlow/VolumetricFluidNVIDIA"
{
    Properties
    {
        _DensityTex ("Density Texture", 3D) = "white" {}
        _VelocityTex ("Velocity Texture", 3D) = "white" {}
        _DensityScale ("Density Scale", Float) = 1.0
        _Color ("Fluid Color", Color) = (0.2, 0.5, 1.0, 1.0)
        _StepSize ("Ray Step Size", Float) = 0.01
        _MaxSteps ("Max Ray Steps", Int) = 128
        _AbsorptionScale ("Absorption Scale", Float) = 100.0
    }

    SubShader
    {
        Tags 
        { 
            "Queue"="Transparent" 
            "RenderType"="Transparent" 
            "IgnoreProjector"="True" 
        }
        LOD 100

        Pass
        {
            Name "VolumetricRaymarch"
            Blend SrcAlpha OneMinusSrcAlpha
            ZWrite Off
            Cull Front

            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #pragma target 3.5
            #pragma multi_compile_fog
            
            #include "UnityCG.cginc"
            #include "NvFlowUnityAdapter.cginc"

            struct appdata
            {
                float4 vertex : POSITION;
                float3 normal : NORMAL;
            };

            struct v2f
            {
                float4 pos : SV_POSITION;
                float3 worldPos : TEXCOORD0;
                float3 objectPos : TEXCOORD1;
                float3 rayDir : TEXCOORD2;
                UNITY_FOG_COORDS(3)
            };

            sampler3D _DensityTex;
            sampler3D _VelocityTex;
            float _DensityScale;
            float4 _Color;
            float _StepSize;
            int _MaxSteps;
            float _AbsorptionScale;

            v2f vert(appdata v)
            {
                v2f o;
                o.pos = UnityObjectToClipPos(v.vertex);
                o.worldPos = mul(unity_ObjectToWorld, v.vertex).xyz;
                o.objectPos = v.vertex.xyz;
                
                // Calculate ray direction in world space
                float3 cameraToVertex = o.worldPos - _WorldSpaceCameraPos;
                o.rayDir = normalize(cameraToVertex);
                
                UNITY_TRANSFER_FOG(o, o.pos);
                return o;
            }

            float4 frag(v2f i) : SV_Target
            {
                // Setup ray in object space
                float3 rayOriginWorld = _WorldSpaceCameraPos;
                float3 rayDirWorld = normalize(i.worldPos - rayOriginWorld);
                
                // Transform to object space
                float3 rayOriginObj = mul(unity_WorldToObject, float4(rayOriginWorld, 1.0)).xyz;
                float3 rayDirObj = normalize(mul((float3x3)unity_WorldToObject, rayDirWorld));
                
                // Convert to UV space [0,1]
                float3 rayOriginUV = rayOriginObj * 0.5 + 0.5;
                float3 rayEndUV = i.objectPos * 0.5 + 0.5;
                
                // Calculate intersection with unit cube [0,1]
                float3 rayDirInv = 1.0 / (rayDirObj + 0.0001);
                float3 boxMin = -rayOriginUV;
                float3 boxMax = (float3(1, 1, 1) - rayOriginUV);
                
                float tNear, tFar;
                bool hit = NvFlowIntersectBox(rayDirObj, rayDirInv, boxMin, boxMax, tNear, tFar);
                
                if (!hit)
                {
                    discard;
                    return float4(0, 0, 0, 0);
                }
                
                // Clamp to valid range
                tNear = max(0.0, tNear);
                tFar = min(tFar, length(rayEndUV - rayOriginUV));
                
                if (tNear > tFar)
                {
                    discard;
                    return float4(0, 0, 0, 0);
                }
                
                // Perform volumetric raymarching using NVIDIA-inspired algorithm
                float4 color = NvFlowRayMarchVolume(
                    _DensityTex,
                    _VelocityTex,
                    rayOriginUV,
                    normalize(rayEndUV - rayOriginUV),
                    tNear,
                    tFar,
                    _StepSize,
                    _MaxSteps,
                    _DensityScale,
                    _Color,
                    _AbsorptionScale
                );
                
                // Apply fog
                UNITY_APPLY_FOG(i.fogCoord, color);
                
                return color;
            }
            ENDCG
        }
    }

    // Fallback for older hardware
    SubShader
    {
        Tags { "Queue"="Transparent" "RenderType"="Transparent" }
        
        Pass
        {
            Blend SrcAlpha OneMinusSrcAlpha
            ZWrite Off
            
            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #include "UnityCG.cginc"
            
            sampler3D _DensityTex;
            float4 _Color;
            
            struct v2f
            {
                float4 pos : SV_POSITION;
                float3 uv : TEXCOORD0;
            };
            
            v2f vert(appdata_base v)
            {
                v2f o;
                o.pos = UnityObjectToClipPos(v.vertex);
                o.uv = v.vertex.xyz * 0.5 + 0.5;
                return o;
            }
            
            float4 frag(v2f i) : SV_Target
            {
                float density = tex3D(_DensityTex, i.uv).r;
                return float4(_Color.rgb, density * _Color.a);
            }
            ENDCG
        }
    }

    FallBack "Transparent/VertexLit"
}
