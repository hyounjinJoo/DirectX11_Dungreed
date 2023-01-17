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
	// ���� ������
	extern Vertex vertexes[4];

	// ���ؽ� ����
	extern ID3D11Buffer* triangleBuffer;
	extern ID3DBlob* errorBlob;
	extern ID3D11Buffer* triangleIndexBuffer;
	extern ID3D11Buffer* triangleConstantBuffer;

	// ���ؽ� ���̴�
	extern ID3DBlob* triangleVSBlob;
	extern ID3D11VertexShader* triangleVS;

	// �ȼ� ���̴�
	extern ID3DBlob* trianglePSBlob;
	extern ID3D11PixelShader* trianglePS;

	// ��ǲ ���̾ƿ� ( ���� ���� )
	extern ID3D11InputLayout* triangleLayout;
#pragma endregion

	void Initialize();
	void Release();
}