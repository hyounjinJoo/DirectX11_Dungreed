#include "globals.hlsli"

struct VSOut
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
    
    float2 WorldPos : POSITION;
};

float4 main(VSOut In) : SV_TARGET
{
    // 선 색상
    float4 Out = ALPHA(GREY, 0.5f);

    // 대각타일 색상
    float4 OutDiagonal = ALPHA(GREY, 0.1f);
    float4 OutInverseDiagonal = ALPHA(WHITE, 0.1f);

    // 타일을 포함한 한 칸의 크기를 나타낸다.
    // 33이 100과 매핑된다.
    const float width = 42.6f;
    const float height = 42.6f;

    // 연산 문제로 329.9f 설정
    const float widthX10 = 426.f;
    const float heightX10 = 426.f;
    
    // 픽셀의 월드 좌표
    int worldX = (int)In.WorldPos.x;
    int worldY = (int)In.WorldPos.y;

    // 선 두께
    const float thickness = 1.f * cameraScale.x;

    // 한 칸 상의 픽셀의 좌표
    float calcX = abs((float)(worldX) % width);
    float calcY = abs((float)(worldY) % height);
    
    float calcX10 = abs((float)(worldX) % widthX10);
    float calcY10 = abs((float)(worldY) % heightX10);
    
    // 원점의 벡터 그리기
    if (calcX <= thickness || calcY <= thickness)
    {
        float4 axisXColor = RED;
        float4 axisYColor = GREEN;
        
        // 좌표축 그리기
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

    
    
    // 타일의 크기
    const int halfWidth = width * 0.5f;
    const int halfHeight = height * 0.5f;

    // 타일 칠하기
    if (calcX < halfWidth && calcY < halfHeight
        || calcX > halfWidth && calcY > halfHeight)
    {
        // 1사분면
        if (worldY > 0.f)
        {
            if (worldX > 0.f)
                return OutDiagonal;
            else if(worldX < 0.f)
                return OutInverseDiagonal;
        }
        // 2사분면
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
        // 3사분면
        if (worldX > 0.f)
            return OutInverseDiagonal;
        else if (worldX < 0.f)
            return OutDiagonal;
    }
    else
    {
        // 4사분면
        if (worldX > 0.f)
            return OutDiagonal;
        else if (worldX < 0.f)
            return OutInverseDiagonal;
    }
	
    return float4(1.f, 0.f, 1.f, 1.f);        
}