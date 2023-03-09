#include "globals.hlsli"

struct VSOut
{
    float4 Pos : SV_Position;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

#define MOVED       cbfData1
#define MOVE_DIR    cbiData1

#define LEFT        0
#define TOP         1
#define RIGHT       2
#define BOTTOM      3

float4 main(VSOut In) : SV_TARGET
{
    float4 color = (float) 0.f;
    float2 movedUV = In.UV;
    
    switch (MOVE_DIR)
    {
        case LEFT:
            movedUV.x += MOVED;
            break;
        case TOP:
            movedUV.y += MOVED;
            break;
        case RIGHT:
            movedUV.x += -MOVED;
            break;
        case BOTTOM:
            movedUV.y += -MOVED;
            break;
        default:
            break;
    }
    
    color = defaultTexture.Sample(pointSampler, movedUV);
    
    if (color.a == 0.f)
        discard;
    //color = float4(1.f, 0.f, 1.f, 1.f);
    return color;
}