#include "globals.hlsli"

struct VSIn
{
    float4 Pos : POSITION;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD;
    
    float2 WorldPos : POSITION;
};

VSOut main(VSIn In)
{
    VSOut Out = (VSOut) 0.f;

    // 그려질 메쉬의 크기와 메인 카메라의 비율을 계산하여 화면에 최종적으로 그려질 크기를 구해낸다.
    float calcMeshScale = meshScale + cameraScale.x;
    
    // 메쉬의 월드상 위치로 카메라의 이동에 따라 영향을 받아 그려진다.
    float2 worldPos = float2(0.f, 0.f);
    // 그리드의 위치를 카메라 비율과 해상도를 적용한다.
    // 카메라의 이동량에 최종적으로 그려질 크기만큼의 비율을 적용해야 한다.
    // 2.f를 곱해준 이유는 meshScale이 0.5배로 적용되기 때문에 이를 되돌리기 위해서.
    worldPos.x = In.Pos.x * cameraScale.x * resolution.x + (cameraPosition.x / calcMeshScale * 2.f);
    worldPos.y = In.Pos.y * cameraScale.y * resolution.y + (cameraPosition.y / calcMeshScale * 2.f);
    
    // PS로 넘겨주기 위한 최종 버텍스 정보는 그려질 크기를 반영한다.
    // z값과 w값이 아래와 같은 이유는 화면 제일 뒤에서 그려주기 위해서이다.
    Out.Pos = float4(In.Pos.xy * calcMeshScale, 0.999f, 1.f);
    Out.UV = In.UV;
    Out.WorldPos = worldPos;
    
    return Out;
}