#include "globals.hlsli"

struct VSOut
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
};


float4 main(VSOut In) : SV_TARGET
{
    float4 Out = cbxyzw1;
    return Out;
}