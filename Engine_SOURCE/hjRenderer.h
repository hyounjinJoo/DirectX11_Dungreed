#pragma once

#include "CommonInclude.h"
#include "hjMath.h"
#include "hjGraphicDevice_DX11.h"

#include "hjMesh.h"
#include "hjShader.h"

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

	// �޽�
	extern Mesh* mesh;

	// ��� ����
	//extern Microsoft::WRL::ComPtr<ID3DBlob> errorBlob;
	extern Microsoft::WRL::ComPtr<ID3D11Buffer> triangleConstantBuffer;

	extern Shader* shader;
	// ���ؽ� ���̴�
	//extern Microsoft::WRL::ComPtr<ID3DBlob> triangleVSBlob;
	//extern Microsoft::WRL::ComPtr<ID3D11VertexShader> triangleVS;

	// �ȼ� ���̴�
	//extern Microsoft::WRL::ComPtr<ID3DBlob> trianglePSBlob;
	//extern Microsoft::WRL::ComPtr<ID3D11PixelShader> trianglePS;

	// ��ǲ ���̾ƿ� ( ���� ���� )
	//extern Microsoft::WRL::ComPtr<ID3D11InputLayout> triangleLayout;
#pragma endregion

	void Initialize();
	void Release();
}