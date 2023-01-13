#pragma once

#include <wrl.h>
#include "hjMath.h"

// 다이렉트X11을 사용하기 위한 헤더
#include <d3d11.h>
// 다이렉트X11을 사용한 것을 컴파일 해주는 컴파일러 헤더
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

namespace hj::graphics
{
	enum class ValidationMode
	{
		Disabled,
		Enabled,
		GPU,
	};

	enum class eShaderState
	{
		VS,
		PS,
		Count,
	};


}