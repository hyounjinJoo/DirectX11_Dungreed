#include "globals.hlsli"

struct VSOut
{
    float4 Pos : SV_Position;
    float3 WorldPos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

#define MOVED       cbfData1
#define MOVE_DIR    cbiData1

#define LEFT        0
#define TOP         1
#define RIGHT       2
#define BOTTOM      3

#define SPRITEUVSTART cbxy1
#define SPRITEUVEND   cbxy2

float4 main(VSOut In) : SV_TARGET
{
    float4 color = (float) 0.f;
    float2 movedUV = In.UV;
    
    switch (MOVE_DIR)
    {
        case LEFT:
            {                
                movedUV.x += MOVED;
                if (SPRITEUVEND.x < movedUV.x)
                {
                    movedUV.x -= SPRITEUVEND.x;
                    movedUV.x += SPRITEUVSTART.x;
                }
            }
        break;
        case TOP:
            {                
                movedUV.y += MOVED;
                if (SPRITEUVEND.y < movedUV.y)
                {
                    movedUV.y -= SPRITEUVEND.y;
                    movedUV.y += SPRITEUVSTART.y;
                }
            }
            break;
        case RIGHT:
            {                
                movedUV.x += -MOVED;
                if (movedUV.x < SPRITEUVSTART.x)
                {
                    movedUV.x = SPRITEUVSTART.x - movedUV.x;
                    movedUV.x = SPRITEUVEND.x - movedUV.x;
                }
            }
            break;
        case BOTTOM:
            {
                movedUV.y += -MOVED;
                if (movedUV.y < SPRITEUVSTART.y)
                {
                    movedUV.y = SPRITEUVSTART.y - movedUV.y;
                    movedUV.y = SPRITEUVEND.y - movedUV.y;
                }
            }
            break;
        default:
            break;
    }
    
    if (movedUV.x < SPRITEUVSTART.x || SPRITEUVEND.x < movedUV.x || movedUV.y < SPRITEUVSTART.y || SPRITEUVEND.y < movedUV.y)
    {
        discard;
    }
    
    color = defaultTexture.Sample(pointSampler, movedUV);
    
    if (color.a == 0.f)
        discard;

    LightColor lightColor = (LightColor) 0.f;
    for (uint i = 0; i < numberOfLight; ++i)
    {
        CalculateLight(lightColor, In.WorldPos.xyz, i);
    }
    
    color *= lightColor.diffuse;
    
    return color;
}