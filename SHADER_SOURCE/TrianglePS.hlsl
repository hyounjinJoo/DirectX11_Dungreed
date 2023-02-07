
struct VTX_OUT
{
    float4 vPos : SV_Position;
    float4 vColor : COLOR;
    float2 vUV : TEXCOORD;
};

Texture2D defaultTexture : register(t0);
SamplerState samplerState : register(s0);

float4 PS_Test(VTX_OUT _in) : SV_Target
{
    float4 color = (float) 0.f;
    
    color = defaultTexture.Sample(samplerState, _in.vUV);
    
    if (color.a == 0.f)
        discard;
    
    return color;
}