#include "globals.hlsli"

RWStructuredBuffer<AnimationParticle> ParticleBuffer : register(u0);
RWStructuredBuffer<ParticleShared> ParticleSharedBuffer : register(u1);
RWStructuredBuffer<AnimationParticleShared> AnimationParticleSharedBuffer : register(u2);


//각각의 스레드가 동기화할 데이터가 필요하다

//1024개가 최대 스레드 개수
[numthreads(128, 1, 1)]
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (maxParticles <= DTid.x)
    {
        return;
    }
    
    if (ParticleBuffer[DTid.x].defaultParticleInfo.active == 0)
    {
        while (0 < ParticleSharedBuffer[0].gActiveCount)
        {
            int originValue = ParticleSharedBuffer[0].gActiveCount;
            int exchange = originValue - 1;
            // 스레드 동기화
            // dest값을 exchange값으로 바꾸는 동안
            // 다른스레드는 멈춘다.
            InterlockedCompareExchange(ParticleSharedBuffer[0].gActiveCount, originValue, exchange, exchange);
            //InterlockedExchange(ParticleSharedBuffer[0].gActiveCount, exchange, originValue);
            
            if (exchange == originValue)
            {
                ParticleBuffer[DTid.x].defaultParticleInfo.active = 1;
                ParticleBuffer[DTid.x].defaultParticleInfo.time = 0.0f;
                ParticleBuffer[DTid.x].defaultParticleInfo.lifeTime = startLifeTime;
                ParticleBuffer[DTid.x].defaultParticleInfo.delayTimer = 0.f;
                
                break;
            }
        }
    }
    else // active == 1
    {       
        ParticleBuffer[DTid.x].defaultParticleInfo.delayTimer += deltaTime;
        if (ParticleBuffer[DTid.x].defaultParticleInfo.delayThres > ParticleBuffer[DTid.x].defaultParticleInfo.delayTimer)
        {
            return;
        }
        
        ParticleBuffer[DTid.x].defaultParticleInfo.time += deltaTime;
        if (ParticleBuffer[DTid.x].defaultParticleInfo.lifeTime < ParticleBuffer[DTid.x].defaultParticleInfo.time)
        {
            if (ParticleBuffer[DTid.x].loop)
            {
                ParticleBuffer[DTid.x].animationIndex = 0;
                ParticleBuffer[DTid.x].framePlayTime = 0.f;
                ParticleBuffer[DTid.x].defaultParticleInfo.active = 1;
                ParticleBuffer[DTid.x].defaultParticleInfo.time = 0.0f;
                ParticleBuffer[DTid.x].defaultParticleInfo.lifeTime = startLifeTime;
            }
            else
            {
                ParticleBuffer[DTid.x].defaultParticleInfo.active = 0;
            }
        }
        else
        {
            ParticleBuffer[DTid.x].framePlayTime += deltaTime;
            if (ParticleBuffer[DTid.x].framePlayTime >= ParticleBuffer[DTid.x].duration)
            {
                ParticleBuffer[DTid.x].framePlayTime = 0.f;
                ParticleBuffer[DTid.x].animationIndex += 1;
                
                uint frameNums = 0;
                uint stride = 0;
                AnimationParticleSharedBuffer.GetDimensions(frameNums, stride);
                
                if (ParticleBuffer[DTid.x].animationIndex == frameNums)
                {
                    ParticleBuffer[DTid.x].animationIndex = 0;
                    ParticleBuffer[DTid.x].framePlayTime = 0.f;
                    if (ParticleBuffer[DTid.x].loop)
                    {
                        ParticleBuffer[DTid.x].defaultParticleInfo.active = 1;
                        ParticleBuffer[DTid.x].defaultParticleInfo.time = 0.0f;
                        ParticleBuffer[DTid.x].defaultParticleInfo.lifeTime = startLifeTime;
                    }
                    else
                    {
                        ParticleBuffer[DTid.x].defaultParticleInfo.active = 0;
                    }
                }
            }
            
            ParticleBuffer[DTid.x].animInfo.animLeftTop = AnimationParticleSharedBuffer[ParticleBuffer[DTid.x].animationIndex].frameInfo.animLeftTop;
            ParticleBuffer[DTid.x].animInfo.animSpriteSize = AnimationParticleSharedBuffer[ParticleBuffer[DTid.x].animationIndex].frameInfo.animSpriteSize;
            ParticleBuffer[DTid.x].animInfo.animOffset = AnimationParticleSharedBuffer[ParticleBuffer[DTid.x].animationIndex].frameInfo.animOffset;
            ParticleBuffer[DTid.x].animInfo.animAtlasSize = AnimationParticleSharedBuffer[ParticleBuffer[DTid.x].animationIndex].frameInfo.animAtlasSize;
            ParticleBuffer[DTid.x].animInfo.animCanvasSize = AnimationParticleSharedBuffer[ParticleBuffer[DTid.x].animationIndex].frameInfo.animCanvasSize;
            ParticleBuffer[DTid.x].animInfo.animUsed = AnimationParticleSharedBuffer[ParticleBuffer[DTid.x].animationIndex].frameInfo.animUsed;
            ParticleBuffer[DTid.x].animInfo.animCanvasUsed = AnimationParticleSharedBuffer[ParticleBuffer[DTid.x].animationIndex].frameInfo.animCanvasUsed;
            ParticleBuffer[DTid.x].animInfo.animInverse = AnimationParticleSharedBuffer[ParticleBuffer[DTid.x].animationIndex].frameInfo.animInverse;
            ParticleBuffer[DTid.x].animInfo.animType = AnimationParticleSharedBuffer[ParticleBuffer[DTid.x].animationIndex].frameInfo.animType;
        }
    }
}