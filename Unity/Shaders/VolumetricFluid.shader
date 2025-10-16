Shader "PhysXFlow/VolumetricFluid"
{
    Properties
    {
        _DensityTex ("Density Texture", 3D) = "white" {}
        _VelocityTex ("Velocity Texture", 3D) = "white" {}
        _DensityScale ("Density Scale", Float) = 1.0
        _Color ("Fluid Color", Color) = (0.2, 0.5, 1.0, 1.0)
        _StepSize ("Ray Step Size", Float) = 0.01
        _MaxSteps ("Max Ray Steps", Int) = 128
    }

    SubShader
    {
        Tags { "Queue"="Transparent" "RenderType"="Transparent" "IgnoreProjector"="True" }
        LOD 100

        Pass
        {
            Blend SrcAlpha OneMinusSrcAlpha
            ZWrite Off
            Cull Front

            CGPROGRAM
            #pragma vertex vert
            #pragma fragment frag
            #include "UnityCG.cginc"

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
            };

            sampler3D _DensityTex;
            sampler3D _VelocityTex;
            float _DensityScale;
            float4 _Color;
            float _StepSize;
            int _MaxSteps;

            v2f vert(appdata v)
            {
                v2f o;
                o.pos = UnityObjectToClipPos(v.vertex);
                o.worldPos = mul(unity_ObjectToWorld, v.vertex).xyz;
                o.objectPos = v.vertex.xyz;
                return o;
            }

            float4 frag(v2f i) : SV_Target
            {
                // Raymarch from camera to fragment in object space
                float3 rayOrigin = mul(unity_WorldToObject, float4(_WorldSpaceCameraPos, 1.0)).xyz;
                float3 rayDir = normalize(i.objectPos - rayOrigin);

                // Convert to UV space [0,1]
                rayOrigin = rayOrigin * 0.5 + 0.5;
                float3 rayEnd = i.objectPos * 0.5 + 0.5;

                float3 rayStep = rayDir * _StepSize;
                float3 rayPos = rayOrigin;

                float4 color = float4(0, 0, 0, 0);
                float transmittance = 1.0;

                for (int step = 0; step < _MaxSteps; step++)
                {
                    // Check bounds
                    if (any(rayPos < 0.0) || any(rayPos > 1.0))
                        break;

                    // Sample density
                    float density = tex3D(_DensityTex, rayPos).r * _DensityScale;

                    if (density > 0.001)
                    {
                        // Accumulate color and opacity
                        float alpha = saturate(density * _StepSize * 100.0);
                        float3 fluidColor = _Color.rgb;

                        // Optional: modulate by velocity for visual interest
                        float3 velocity = tex3D(_VelocityTex, rayPos).rgb;
                        fluidColor += velocity * 0.1;

                        color.rgb += transmittance * alpha * fluidColor;
                        color.a += transmittance * alpha;
                        transmittance *= (1.0 - alpha);

                        if (transmittance < 0.01)
                            break;
                    }

                    rayPos += rayStep;
                }

                return color;
            }
            ENDCG
        }
    }

    FallBack "Transparent/VertexLit"
}
