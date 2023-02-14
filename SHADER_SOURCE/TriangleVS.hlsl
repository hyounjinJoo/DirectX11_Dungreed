
#include "globals.hlsli"

VTX_OUT VS_Test(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.f;
    
    float4 worldPosition = mul(_in.vPos, world);
    float4 viewPosition = mul(worldPosition, view);
    float4 projPosition = mul(viewPosition, projection);
        
    output.vPos = projPosition;
    output.vColor = _in.vColor;
    output.vUV = _in.vUV;
    
	return output;
}