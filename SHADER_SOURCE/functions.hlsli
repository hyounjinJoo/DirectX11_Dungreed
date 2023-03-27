#include "globals.hlsli"

static float Gaussian5x5[25] =
{
    0.003765f, 0.015019f, 0.023792f, 0.015019f, 0.003765f,
	0.015019f, 0.059912f, 0.094907f, 0.059912f, 0.015019f,
	0.023792f, 0.094907f, 0.150342f, 0.094907f, 0.023792f,
	0.015019f, 0.059912f, 0.094907f, 0.059912f, 0.015019f,
	0.003765f, 0.015019f, 0.023792f, 0.015019f, 0.003765f
};

float GaussianSample(int2 UV, Texture2D InputTex)
{
    float Output = 0.f;

    for (int i = 0; i < 5; ++i)
    {
        for (int j = 0; j < 5; ++j)
        {
            int2 convertUV = UV + int2(j - 2, i - 2);

            if (convertUV.x < 0)
                convertUV.x = 0;
            else if (convertUV.x > globalNoiseResolution.x)
                convertUV.x = globalNoiseResolution.x - 1;

            if (convertUV.y < 0)
                convertUV.y = 0;
            else if (convertUV.y > globalNoiseResolution.y)
                convertUV.y = globalNoiseResolution.y - 1;

            Output += InputTex[convertUV].r * Gaussian5x5[i * 5 + j];
        }
    }

    return Output;
}

float Rand(float Key)
{
    float2 UV = float2(cos(Key + globalAccTime), sin(globalAccTime));
	
    UV = frac(UV);

    return GaussianSample(UV * globalNoiseResolution, noiseTexture);
}

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

#define DIRECTIONALLIGHT 0
#define POINTLIGHT 1
#define SPOTLIGHT 2
void CalculateLight(in out LightColor pLightColor, float3 position, int idx)
{
    if (DIRECTIONALLIGHT == lightAttributes[idx].type)
    {
        pLightColor.diffuse += lightAttributes[idx].color.diffuse;
    }
    else if(POINTLIGHT == lightAttributes[idx].type)
    {
        float length = distance(lightAttributes[idx].position.xy, position.xy);
        
        if (length < lightAttributes[idx].radius)
        {
            float ratio = 1.f - (length / lightAttributes[idx].radius);
            pLightColor.diffuse += lightAttributes[idx].color.diffuse * ratio;
        }
    }
    else if (SPOTLIGHT == lightAttributes[idx].type)
    {
        
    }
}