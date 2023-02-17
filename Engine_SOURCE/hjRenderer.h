#pragma once

#include "CommonInclude.h"
#include "hjMath.h"
#include "hjGraphicDevice_DX11.h"

#include "hjMesh.h"
#include "hjShader.h"
#include "hjConstantBuffer.h"
#include "hjCamera.h"

using namespace hj::math;
using namespace hj::graphics;

namespace hj::renderer
{
	struct Vertex
	{
		Vector4 pos;
		Vector4 color;
		Vector2 uv;
	};

	CBUFFER(TransformCB, CBSLOT_TRANSFORM)
	{
		Matrix world;
		Matrix view;
		Matrix projection;
	};

	CBUFFER(MaterialCB, CBSLOT_MATERIAL)
	{
		int iData;
		float fData;
		Vector2 xy;
		Vector3 xyz;
		Vector4 xyzw;
		Matrix matrix;
	};

#pragma region extern Variables
	extern Vertex vertexes[4];
	extern ConstantBuffer* constantBuffers[];
	extern Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerStates[];
	extern Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerStates[];
	extern Microsoft::WRL::ComPtr<ID3D11DepthStencilState> depthstencilStates[];
	extern Microsoft::WRL::ComPtr<ID3D11BlendState> blendStates[];

	extern std::vector<Camera*> cameras;
#pragma endregion

	void Initialize();
	void Render();
	void Release();
}