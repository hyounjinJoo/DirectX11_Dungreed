#include "globals.hlsli"

struct GSOutput
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
};

float4 main(GSOutput In) : SV_TARGET
{
    float4 outColor = (float4) 0.0f;
    
    outColor = defaultTexture.Sample(anisotropicSampler, In.UV);
    
    outColor *= startColor;
    
    return outColor;
}