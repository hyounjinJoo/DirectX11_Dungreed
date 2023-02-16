#include "globals.hlsli"

float4 main(VSOut In) : SV_TARGET
{
    float4 color = (float) 0.f;    
    color = defaultTexture.Sample(pointSampler, In.UV);
    
    if (color.a == 0.f)
        discard;
    
    return color;
}