#include "globals.hlsli"

struct VSOut
{
    float4 Pos : SV_Position;
    float3 WorldPos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};


#define INVERSE -1

float4 main(VSOut In) : SV_TARGET
{
    float4 color = (float) 0.f;    
    if(cbiData1 == INVERSE)
        In.UV.x = 1.f - In.UV.x;
    
    color = defaultTexture.Sample(pointSampler, In.UV);
    
    if (color.a == 0.f)
        discard;
    
    LightColor lightColor = (LightColor) 0.f;
    for (uint i = 0; i < numberOfLight; ++i)
    {
        CalculateLight(lightColor, In.WorldPos.xyz, i);
    }
    
    color *= lightColor.diffuse;
    
    return color;
}