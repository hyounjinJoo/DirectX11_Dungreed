#pragma once

#include "CommonInclude.h"
#include "hjMath.h"
#include "hjGraphicDevice_DX11.h"

#include "hjMesh.h"

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

	// 메시
	extern Mesh* mesh;

	// 버텍스 버퍼
	extern Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
	extern Microsoft::WRL::ComPtr<ID3D11Buffer> triangleConstantBuffer;

	// 버텍스 쉐이더
	extern Microsoft::WRL::ComPtr<ID3DBlob> triangleVSBlob;
	extern Microsoft::WRL::ComPtr<ID3D11VertexShader> triangleVS;

	// 픽셀 쉐이더
	extern Microsoft::WRL::ComPtr<ID3DBlob> trianglePSBlob;
	extern Microsoft::WRL::ComPtr<ID3D11PixelShader> trianglePS;

	// 인풋 레이아웃 ( 정점 정보 )
	extern Microsoft::WRL::ComPtr<ID3D11InputLayout> triangleLayout;
#pragma endregion

	void Initialize();
	void Release();
}