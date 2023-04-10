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
