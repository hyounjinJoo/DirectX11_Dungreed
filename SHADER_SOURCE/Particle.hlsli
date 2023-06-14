
struct Particle
{
    float4 position;
    float4 direction;
    
    float lifeTime;
    float time;
    float speed;
    uint active;
    
    float delayThres;
    float delayTimer;
};

struct ParticleShared
{
    uint gActiveCount;
};

struct ParticleFrame
{
    float2 animLeftTop;
    float2 animSpriteSize;
    float2 animOffset;
    float2 animAtlasSize;
    
    float2 animCanvasSize;
    uint animUsed;
    uint animCanvasUsed;
    
    uint animInverse;
    uint animType;
    float2 animpadding;
};

struct AnimationParticle
{
    uint loop;
    uint animationIndex;
    float duration;
    float framePlayTime;
    
    Particle defaultParticleInfo;
    ParticleFrame animInfo;    
};

struct AnimationParticleShared
{
    ParticleFrame frameInfo;
};

