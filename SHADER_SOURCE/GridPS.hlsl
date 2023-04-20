#include "globals.hlsli"

struct VSOut
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
    
    float2 WorldPos : POSITION;
};

float4 main(VSOut In) : SV_TARGET
{
    // �� ����
    float4 Out = ALPHA(GREY, 0.5f);

    // �밢Ÿ�� ����
    float4 OutDiagonal = ALPHA(GREY, 0.1f);
    float4 OutInverseDiagonal = ALPHA(WHITE, 0.1f);

    // Ÿ���� ������ �� ĭ�� ũ�⸦ ��Ÿ����.
    // 33�� 100�� ���εȴ�.
    const float width = 42.6f;
    const float height = 42.6f;

    // ���� ������ 329.9f ����
    const float widthX10 = 426.f;
    const float heightX10 = 426.f;
    
    // �ȼ��� ���� ��ǥ
    int worldX = (int)In.WorldPos.x;
    int worldY = (int)In.WorldPos.y;

    // �� �β�
    const float thickness = 1.f * cameraScale.x;

    // �� ĭ ���� �ȼ��� ��ǥ
    float calcX = abs((float)(worldX) % width);
    float calcY = abs((float)(worldY) % height);
    
    float calcX10 = abs((float)(worldX) % widthX10);
    float calcY10 = abs((float)(worldY) % heightX10);
    
    // ������ ���� �׸���
    if (calcX <= thickness || calcY <= thickness)
    {
        float4 axisXColor = RED;
        float4 axisYColor = GREEN;
        
        // ��ǥ�� �׸���
        if (abs(worldX) <= thickness)
            return axisYColor;
        if (abs(worldY) <= thickness)
            return axisXColor;
        
        if (abs(worldX) <= thickness * 4.f)
            return axisYColor;
        if (abs(worldY) <= thickness * 4.f)
            return axisXColor;
        
        if (abs(calcX10) <= thickness || abs(calcY10) <= thickness)
            return WHITE;
        
        return Out;
    }

    
    
    // Ÿ���� ũ��
    const int halfWidth = width * 0.5f;
    const int halfHeight = height * 0.5f;

    // Ÿ�� ĥ�ϱ�
    if (calcX < halfWidth && calcY < halfHeight
        || calcX > halfWidth && calcY > halfHeight)
    {
        // 1��и�
        if (worldY > 0.f)
        {
            if (worldX > 0.f)
                return OutDiagonal;
            else if(worldX < 0.f)
                return OutInverseDiagonal;
        }
        // 2��и�
        else if(worldY < 0.f)
        {
            if (worldX > 0.f)
                return OutInverseDiagonal;
            else if (worldX < 0.f)
                return OutDiagonal;
        }
    }

    if (worldY > 0.f)
    {
        // 3��и�
        if (worldX > 0.f)
            return OutInverseDiagonal;
        else if (worldX < 0.f)
            return OutDiagonal;
    }
    else
    {
        // 4��и�
        if (worldX > 0.f)
            return OutDiagonal;
        else if (worldX < 0.f)
            return OutInverseDiagonal;
    }
	
    return float4(1.f, 0.f, 1.f, 1.f);        
}