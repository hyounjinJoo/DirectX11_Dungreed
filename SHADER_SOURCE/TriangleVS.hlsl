
struct VTX_IN
{
    float3 vPos : POSITION;
    float4 vColor : COLOR;
};

struct VTX_OUT
{
    float4 vPos : SV_Position;
    float4 vColor : COLOR;
};

VTX_OUT main(VTX_IN _in)
{
    VTX_OUT output = (VTX_OUT) 0.0f;
    
    output.vPos = float4(_in.vPos, 1.0f);
    output.vColor = _in.vColor;
    
	return output;
}