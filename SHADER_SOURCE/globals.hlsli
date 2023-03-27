#include "Light.hlsli"
#include "defines.hlsli"

cbuffer Transform : register(b0)
{
    row_major matrix world;
    row_major matrix view;
    row_major matrix projection;    
}

cbuffer MaterialData : register(b1)
{
    int cbiData1;
    int cbiData2;
    int cbiData3;
    int cbiData4;
    
    float cbfData1;
    float cbfData2;
    float cbfData3;
    float cbfData4;
    
    float2 cbxy1;
    float2 cbxy2;
    float2 cbxy3;
    float2 cbxy4;
    
    float3 cbxyz1;
    float  cbxyzPadding1;
    float3 cbxyz2;
    float  cbxyzPadding2;
    float3 cbxyz3;
    float  cbxyzPadding3;
    float3 cbxyz4;
    float  cbxyzPadding4;
    
    float4 cbxyzw1;
    float4 cbxyzw2;
    float4 cbxyzw3;
    float4 cbxyzw4;
    
    matrix cbmat1;
    matrix cbmat2;
    matrix cbmat3;
    matrix cbmat4;
}

cbuffer Grid : register(b2)
{
    float4  cameraPosition;
    float2  cameraScale;
    float2  resolution;
    float   meshScale;

    float3  cbGridPadding;
}

cbuffer Animation : register(b3)
{
    float2 cbAnimLeftTop;
    float2 cbAnimSpriteSize;
    float2 cbAnimOffset;
    float2 cbAnimAtlasSize;
    
    float2 cbAnimCanvasSize;
    uint cbAnimUsed;
    uint cbAnimCanvasUsed;
    
    uint cbAnimInverse;
    uint cbAnimType;
    float2 cbAnimpadding;
}

cbuffer NumberOfLight : register(b4)
{
    uint numberOfLight;
}

cbuffer GlobalCBuffer : register(b5)
{
    float globalDeltaTime;
    float globalAccTime;
    float2 globalResolution;
    float2 globalNoiseResolution;
    float2 globalPadding;
};

SamplerState pointSampler : register(s0);
SamplerState linearSampler : register(s1);
SamplerState anisotropicSampler : register(s2);

StructuredBuffer<LightAttribute> lightAttributes : register(t13);

Texture2D defaultTexture : register(t0);
//Texture2D defaultTexture2 : register(t1);
//Texture2D defaultTexture3 : register(t2);
Texture2D noiseTexture : register(t10);

//Atlas texture
Texture2D atlasTexture : register(t12);


float4 drawOutline(Texture2D tex, SamplerState sampleState, float lineThickness, float2 texcoord, float threshold, float4 outlineColor);
void CalculateLight(in out LightColor pLightColor, float3 position, int idx);