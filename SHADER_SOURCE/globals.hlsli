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
    float2 cbAnimSize;
    float2 cbAnimOffset;
    float2 cbAnimAtlasSize;
    
    float2 cbAnimCanvasSize;
    uint cbAnimUsed;
    uint cbAnimCanvasUsed;
    
    uint cbAnimInverse;
    float3 cbAnimpadding;
}

SamplerState pointSampler : register(s0);
SamplerState linearSampler : register(s1);
SamplerState anisotropicSampler : register(s2);

Texture2D defaultTexture : register(t0);
//Texture2D defaultTexture2 : register(t1);
//Texture2D defaultTexture3 : register(t2);

#define RED     float4(1.f, 0.f, 0.f, 1.f)
#define GREEN   float4(0.f, 1.f, 0.f, 1.f)
#define BLUE    float4(0.f, 0.f, 1.f, 1.f)
#define WHITE   float4(1.f, 1.f, 1.f, 1.f)
#define BLACK   float4(0.f, 0.f, 0.f, 1.f)
#define GREY    float4(0.5f, 0.5f, 0.5f, 1.f)
#define YELLOW  float4(1.f, 1.f, 0.f, 1.f)
#define MAGENTA float4(1.f, 0.f, 1.f, 1.f)
#define ALPHA(target, value) float4(target.r, target.g, target.b, (float)value)

float4 drawOutline(Texture2D tex, SamplerState sampleState, float lineThickness, float2 texcoord, float threshold, float4 outlineColor)
{
    float2 texelSize = (float) 0.f;
    tex.GetDimensions(texelSize.x, texelSize.y);
    
    float texelSizeDetermine = 5.f;
    texelSize.x = 1.f / (texelSize.x * texelSizeDetermine) * lineThickness;
    texelSize.y = 1.f / (texelSize.y * texelSizeDetermine) * lineThickness;
    
    float4 sobelX = (float) 0.f;
    float4 sobelY = (float) 0.f;
    
    sobelX = tex.Sample(sampleState, texcoord + float2(texelSize.x, -texelSize.y)) * -1.0 +
             tex.Sample(sampleState, texcoord + float2(texelSize.x, 0)) * -2.0 +
             tex.Sample(sampleState, texcoord + float2(texelSize.x, texelSize.y)) * -1.0 +
             tex.Sample(sampleState, texcoord + float2(-texelSize.x, -texelSize.y)) +
             tex.Sample(sampleState, texcoord + float2(-texelSize.x, 0)) * 2.0 +
             tex.Sample(sampleState, texcoord + float2(-texelSize.x, texelSize.y));
                                                    
    sobelY = tex.Sample(sampleState, texcoord + float2(texelSize.x, -texelSize.y)) +
             tex.Sample(sampleState, texcoord + float2(texelSize.x, texelSize.y)) * -1.0 +
             tex.Sample(sampleState, texcoord + float2(0, -texelSize.y)) * 2.0 +
             tex.Sample(sampleState, texcoord + float2(0, texelSize.y)) * -2.0 +
             tex.Sample(sampleState, texcoord + float2(-texelSize.x, -texelSize.y)) +
             tex.Sample(sampleState, texcoord + float2(-texelSize.x, texelSize.y)) * -1.0;

    
    float4 sobel = (float) 0.f;
    sobel = sqrt(sobelX * sobelX + sobelY * sobelY);

    float4 edgeColor = (float) 0.f;
    edgeColor = outlineColor * step(threshold, dot(sobel, float4(0.08f, 0.08f, 0.08f, 0.f)));
        
    outlineColor = edgeColor * 2.0;

    float4 result = (float) 0.f;
    result = tex.Sample(sampleState, texcoord);;
    result = result * (1.0 - outlineColor.a) + outlineColor;
    
    return result;
}