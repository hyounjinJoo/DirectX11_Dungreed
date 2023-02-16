#include "globals.hlsli"

VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.f;
    
    float4 worldPosition = mul(In.Pos, world);
    float4 viewPosition = mul(worldPosition, view);
    float4 projPosition = mul(viewPosition, projection);
        
    Out.Pos = projPosition;
    Out.Color = In.Color;
    Out.UV = In.UV;
    
    return Out;
}