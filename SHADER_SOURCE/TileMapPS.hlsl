
#include "globals.hlsli"

struct tTileData
{
    int ImgIdx;
    float2 LTUV;
    int Padding;
};


StructuredBuffer<tTileData> TileDataBuffer : register(t16);

struct VSOut
{
    float4 Pos : SV_Position;
    float3 WorldPos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

#define TileCountX cbiData1
#define TileCountY cbiData2
#define SliceSizeUV cbxy1
float4 main(VSOut In) : SV_TARGET
{
    float4 OutColor = (float4) 0.f;
    
    float2 UV = In.UV * float2(TileCountX, TileCountY);
    
    int2 TileRowCol = floor(UV);
    int TileDataIdx = TileRowCol.y * TileCountX + TileRowCol.x;
    
    if (-1 == TileDataBuffer[TileDataIdx].ImgIdx)
        discard;
    
    float2 LeftTopUV = TileDataBuffer[TileDataIdx].LTUV;
    
    float2 ImgUV = frac(UV);
    
    float2 SampleUV = LeftTopUV + ImgUV * SliceSizeUV;
    
    OutColor = defaultTexture.Sample(pointSampler, SampleUV);
    
    if (OutColor.a == 0.f)
        discard;
    
    return OutColor;
}