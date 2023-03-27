
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