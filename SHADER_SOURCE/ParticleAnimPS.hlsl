#include "globals.hlsli"

struct GSOutput
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
    float3 WorldPos : POSITION;
    uint iInstance : SV_InstanceID;
};

StructuredBuffer<AnimationParticle> particleBuffer : register(t16);
float4 main(GSOutput In) : SV_TARGET
{
    float4 Out = (float) 0.f;
    float2 UV = (float) 0.f;
    
    if (particleBuffer[In.iInstance].animInfo.animInverse == 1)
    {
        In.UV.x = 1.f - In.UV.x;
    }
    
    if (particleBuffer[In.iInstance].animInfo.animUsed)
    {
        if (particleBuffer[In.iInstance].animInfo.animCanvasUsed)
        {
            float2 renderCanvasSize = particleBuffer[In.iInstance].animInfo.animCanvasSize / particleBuffer[In.iInstance].animInfo.animAtlasSize;
            float2 spriteSize = particleBuffer[In.iInstance].animInfo.animSpriteSize / particleBuffer[In.iInstance].animInfo.animAtlasSize;
            float2 spriteLT = particleBuffer[In.iInstance].animInfo.animLeftTop / particleBuffer[In.iInstance].animInfo.animAtlasSize;
            //float2 spriteOffset = particleBuffer[In.iInstance].animInfo.animOffset / particleBuffer[In.iInstance].animInfo.animAtlasSize;
            float2 spriteOffset = (float2) 0.f;
            
            UV = In.UV * renderCanvasSize;
            UV = UV - (renderCanvasSize - spriteSize) / 2.f + spriteLT + spriteOffset;
            
            if (UV.x < spriteLT.x || spriteLT.x + spriteSize.x < UV.x || UV.y < spriteLT.y || spriteLT.y + spriteSize.y < UV.y)
            {
                discard;
            }
        }
        else
        {
            float2 spriteLT = particleBuffer[In.iInstance].animInfo.animLeftTop / particleBuffer[In.iInstance].animInfo.animAtlasSize;
            float2 spriteSize = particleBuffer[In.iInstance].animInfo.animSpriteSize / particleBuffer[In.iInstance].animInfo.animAtlasSize;
            //float2 spriteOffset = particleBuffer[In.iInstance].animInfo.animOffset / particleBuffer[In.iInstance].animInfo.animAtlasSize;
            float2 spriteOffset = (float2) 0.f;
            
            UV = In.UV / particleBuffer[In.iInstance].animInfo.animAtlasSize;
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
    
    if (particleBuffer[In.iInstance].animInfo.animUsed)
        Out = defaultTexture.Sample(pointSampler, UV);
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