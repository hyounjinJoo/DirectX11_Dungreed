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

	enum class eShaderStage
	{
		VS,
		HS,
		DS,
		GS,
		PS,
		CS,
		Count,
	};

	enum class eSamplerType
	{
		Point,
		Linear,
		Anisotropic,
		End,
	};

	struct GpuBuffer
	{
		enum class eType
		{
			Buffer,
			Texture,
			UnknownType,
		} type = eType::UnknownType;

		D3D11_BUFFER_DESC desc;
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

		GpuBuffer() = default;
		virtual ~GpuBuffer() = default;
	};
}