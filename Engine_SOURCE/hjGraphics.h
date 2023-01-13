#pragma once

#include <wrl.h>
#include "hjMath.h"

// ���̷�ƮX11�� ����ϱ� ���� ���
#include <d3d11.h>
// ���̷�ƮX11�� ����� ���� ������ ���ִ� �����Ϸ� ���
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