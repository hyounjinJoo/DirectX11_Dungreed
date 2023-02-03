#pragma once

#include "CommonInclude.h"
#include "hjMath.h"
#include "hjGraphicDevice_DX11.h"

#include "hjMesh.h"
#include "hjShader.h"
#include "hjConstantBuffer.h"

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
	extern Vertex vertexes[4];
	extern Mesh* mesh;
	extern Shader* shader;
	extern ConstantBuffer* constantBuffers[];
#pragma endregion

	void Initialize();
	void Release();
}