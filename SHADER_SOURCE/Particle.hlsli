
struct Particle
{
    float4 position;
    float4 direction;
    
    float lifeTime;
    float time;
    float speed;
    uint active;
};

struct ParticleShared
{
    uint gActiveCount;
};