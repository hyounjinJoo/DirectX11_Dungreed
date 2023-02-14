
#include "globals.hlsli"

float4 PS_Test(VTX_OUT _in) : SV_Target
{
    float4 color = (float) 0.f;
    
    color = defaultTexture.Sample(pointSampler, _in.vUV);
    
    if (color.a == 0.f)
        discard;
    
    return color;
}