#include "functions.hlsli"

RWTexture2D<float4> tex : register(u0);

//  SV_GroupID  : �����忡 ���� �׷��� ��ǥ
//  SV_GroupThreadID : �׷� ������, �������� ��ǥ
//  SV_GroupIndex : �׷� ������, �������� �ε��� ��ǥ(1����)
//  SV_DispatchThreadID : ��ü ������������� ȣ��� ��ǥ

[numthreads(16, 16, 1)] // �׷�� ������ ���� ( �ִ� 1024������ �������� )
void main(uint3 DTid : SV_DispatchThreadID)
{
    if (1024 <= DTid.x || 1024 <= DTid.y)
    {
        return;
    }
    
    float intervalSize = 102.f;
    float posX = floor(DTid.x / intervalSize);    
    float posY = floor(DTid.y / intervalSize);
        
    float RandTestR = Rand(posX);
    float RandTestG = Rand(posY);
    float RandTestB = Rand(posX * posY);
    tex[DTid.xy] = float4(RandTestR, RandTestG, RandTestB, 1.0f);
}