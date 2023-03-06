#include "globals.hlsli"

float4 main( float4 pos : POSITION ) : SV_POSITION
{
    float4 worldPosition = mul(pos, world);
    float4 viewPosition = mul(worldPosition, view);
    float4 projPosition = mul(viewPosition, projection);
	
    return projPosition;
}