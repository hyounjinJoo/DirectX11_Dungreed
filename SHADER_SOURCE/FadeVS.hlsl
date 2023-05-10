#include "globals.hlsli"

struct VSIn
{
    float4 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
};

VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.f;
        
    if (cbxyzw1.a != 0.f)
    {    
        float4 worldPosition = mul(In.Pos, world);
        float4 viewPosition = mul(worldPosition, view);
        float4 projPosition = mul(viewPosition, projection);
    
        Out.Pos = projPosition;
        Out.UV = In.UV;
    }
    
    return Out;
}