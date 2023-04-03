#include "globals.hlsli"

struct VSOut
{
    float4 Pos : SV_Position;
    float3 WorldPos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

float4 main(VSOut In) : SV_TARGET
{
    float4 color = (float) 0.f;
    float red   = cbiData1 > 255 ? 1.f : cbiData1 / 255.f;
    float green = cbiData2 > 255 ? 1.f : cbiData2 / 255.f;
    float blue  = cbiData3 > 255 ? 1.f : cbiData3 / 255.f;
    float alpha = cbiData4 > 255 ? 1.f : cbiData4 / 255.f;
    
    color = float4(red, green, blue, alpha);
    
    if (color.a == 0.f)
        discard;
    
    return color;
}