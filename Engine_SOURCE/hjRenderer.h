#pragma once

#include "CommonInclude.h"
#include "hjMath.h"
#include "hjGraphicDevice_DX11.h"

using namespace hj::math;
using namespace hj::graphics;

namespace hj::renderer
{
	struct Vertex
	{
		Vector3 pos;
		Vector4 color;
	};

#pragma region extern Variables
	// 정점 데이터
	extern Vertex vertexes[4];

	// 버텍스 버퍼
	extern ID3D11Buffer* triangleBuffer;
	extern ID3DBlob* errorBlob;
	extern ID3D11Buffer* triangleIndexBuffer;
	extern ID3D11Buffer* triangleConstantBuffer;

	// 버텍스 쉐이더
	extern ID3DBlob* triangleVSBlob;
	extern ID3D11VertexShader* triangleVS;

	// 픽셀 쉐이더
	extern ID3DBlob* trianglePSBlob;
	extern ID3D11PixelShader* trianglePS;

	// 인풋 레이아웃 ( 정점 정보 )
	extern ID3D11InputLayout* triangleLayout;
#pragma endregion

	void Initialize();
	void Release();
}