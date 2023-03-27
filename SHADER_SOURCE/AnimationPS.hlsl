#include "functions.hlsli"

struct VSOut
{
    float4 Pos : SV_Position;
    float3 WorldPos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

float4 main(VSOut In) : SV_TARGET
{
    float4 Out = (float) 0.f;
    float2 UV = (float) 0.f;
    
    if (cbAnimInverse == 1)
    {
        In.UV.x = 1.f - In.UV.x;
    }
    
    if (cbAnimUsed)
    {
        if (cbAnimCanvasUsed)
        {
            float2 renderCanvasSize = cbAnimCanvasSize / cbAnimAtlasSize;
            float2 spriteSize = cbAnimSpriteSize / cbAnimAtlasSize;
            float2 spriteLT = cbAnimLeftTop / cbAnimAtlasSize;
            float2 spriteOffset = cbAnimOffset / cbAnimAtlasSize;
            
            UV = In.UV * renderCanvasSize;
            UV = UV - (renderCanvasSize - spriteSize) / 2.f + spriteLT + spriteOffset;
            
            if (UV.x < spriteLT.x || spriteLT.x + spriteSize.x < UV.x || UV.y < spriteLT.y || spriteLT.y + spriteSize.y < UV.y)
            {
                discard;
            }
        }
        else
        {
            float2 spriteLT = cbAnimLeftTop / cbAnimAtlasSize;
            float2 spriteSize = cbAnimSpriteSize / cbAnimAtlasSize;
            float2 spriteOffset = cbAnimOffset / cbAnimAtlasSize;
            
            UV = In.UV / cbAnimAtlasSize;
            UV = UV - spriteOffset;
            
            if (UV.x < spriteLT.x || spriteLT.x + spriteSize.x < UV.x || UV.y < spriteLT.y || spriteLT.y + spriteSize.y < UV.y)
            {
                discard;
            }
        }
    }
    else
    {        
        UV = In.UV;
    }
    
    if(cbAnimUsed)
        Out = atlasTexture.Sample(pointSampler, UV);
    else
        Out = defaultTexture.Sample(pointSampler, UV);
    
    if (Out.a == 0.f)
        discard;
    
    LightColor lightColor = (LightColor) 0.f;
    for (uint i = 0; i < numberOfLight; ++i)
    {
        CalculateLight(lightColor, In.WorldPos.xyz, i);
    }
    
    Out *= lightColor.diffuse;
    
    return Out;
}