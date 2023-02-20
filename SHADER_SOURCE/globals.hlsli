struct VSIn
{
    float4 Pos : POSITION;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_Position;
    float4 Color : COLOR;
    float2 UV : TEXCOORD;
};

cbuffer Transform : register(b0)
{
    row_major matrix world;
    row_major matrix view;
    row_major matrix projection;    
}

cbuffer MaterialData : register(b1)
{
    int cbiData;
    float cbfData;
    float2 cbxy;
    float3 cbxyz;
    float4 cbxyzw;
    matrix cbmat;
}

SamplerState pointSampler : register(s0);
SamplerState linearSampler : register(s1);
SamplerState anisotropicSampler : register(s2);

Texture2D defaultTexture : register(t0);
//Texture2D defaultTexture2 : register(t1);
//Texture2D defaultTexture3 : register(t2);

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