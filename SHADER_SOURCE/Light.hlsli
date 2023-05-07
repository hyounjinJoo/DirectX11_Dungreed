struct LightColor
{
    float4 diffuse;
    float4 specular;
    float4 ambient;
    float4 emissive;
};

struct LightAttribute
{
    LightColor color;
    float4 position;
    float4 direction;
    
    int type;
    float radius;
    float angle;
    int padding;
};


StructuredBuffer<LightAttribute> lightAttributes : register(t13);
StructuredBuffer<LightAttribute> lightAttributes3D : register(t14);


#define DIRECTIONALLIGHT 0
#define POINTLIGHT 1
#define SPOTLIGHT 2
void CalculateLight(in out LightColor pLightColor, float3 position, int idx)
{
    if (DIRECTIONALLIGHT == lightAttributes[idx].type)
    {
        pLightColor.diffuse += lightAttributes[idx].color.diffuse;
    }
    else if (POINTLIGHT == lightAttributes[idx].type)
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