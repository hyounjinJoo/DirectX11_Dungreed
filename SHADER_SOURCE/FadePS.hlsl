#include "globals.hlsli"

struct VSOut
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
};


float4 main(VSOut In) : SV_TARGET
{
    if (cbxyzw1.a == 0.f)
        discard;

    float4 Out = cbxyzw1;
    return Out;
}