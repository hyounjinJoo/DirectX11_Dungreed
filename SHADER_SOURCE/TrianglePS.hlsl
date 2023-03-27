
#include "functions.hlsli"

struct VSOut
{
    float4 Pos : SV_Position;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

float4 main(VSOut In) : SV_Target
{   
    float4 color = (float) 0.f;    
    //color = defaultTexture.Sample(pointSampler, In.UV);
    color = drawOutline(defaultTexture, pointSampler, 1.f, In.UV, 0.8f, float4(1.f, 1.f, 0.f, 1.f));
    
    return color;
    
}