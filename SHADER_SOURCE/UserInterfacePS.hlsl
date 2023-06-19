#include "globals.hlsli"

struct VSOut
{
    float4 Pos : SV_Position;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

#define USE_UV      cbiData1
#define SPRITE_START    cbxy1
#define SPRITE_END      cbxy2
#define USE_CANVAS  cbiData2
#define SIZE_CANVAS  cbxy3
#define SIZE_ATLAS   cbxy4

#define USE_ADD_DIFFUSE_COLOR cbiData3
#define DIFFUSE_COLOR cbxyzw1

float4 main(VSOut In) : SV_TARGET
{
    float4 color = (float) 0.f;
    float2 UV = In.UV;
    
    if (USE_CANVAS == 1)
    {
        float2 renderCanvasSize = SIZE_CANVAS / SIZE_ATLAS;
        float2 spriteSize = (SPRITE_END - SPRITE_START) / SIZE_ATLAS;
        float2 spriteLT = SPRITE_START / SIZE_ATLAS;
        
        UV = UV * renderCanvasSize;
        UV = UV - (renderCanvasSize - spriteSize) / 2.f + spriteLT;            
        
        if (UV.x < spriteLT.x || spriteLT.x + spriteSize.x < UV.x || UV.y < spriteLT.y || spriteLT.y + spriteSize.y < UV.y)
        {
            discard;
        }
        color = defaultTexture.Sample(pointSampler, UV);
    }
    else
    {
        UV = UV / SIZE_ATLAS;
        float2 spriteSize = (SPRITE_END - SPRITE_START) / SIZE_ATLAS;
        float2 spriteLT = SPRITE_START / SIZE_ATLAS;
        
        if (UV.x < spriteLT.x || spriteLT.x + spriteSize.x < UV.x || UV.y < spriteLT.y || spriteLT.y + spriteSize.y < UV.y)
        {
            discard;
        }
        
        color = defaultTexture.Sample(pointSampler, UV);
    }
    
    if (color.a == 0.f)
        discard;
        
    if (USE_ADD_DIFFUSE_COLOR == true)
    {
        color += DIFFUSE_COLOR;
    }
    
    return color;
}