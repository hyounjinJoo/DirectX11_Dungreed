#include "globals.hlsli"

struct VSOut
{
    float4 Pos : SV_Position;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

float4 main(VSOut In) : SV_TARGET
{
    float4 color = (float) 0.f;    
    color = defaultTexture.Sample(pointSampler, In.UV);
    
    if (color.a == 0.f)
        discard;
    
    return color;
}