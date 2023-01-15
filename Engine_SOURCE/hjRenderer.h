#pragma once

#include "hjGraphics.h"
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

	void SetUpState();
	void LoadBuffer();
	void LoadShader();

	void Initialize();
	void Release();
}