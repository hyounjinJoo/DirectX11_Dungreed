#pragma once

#include <wrl.h>
#include "hjMath.h"

// 다이렉트X11을 사용하기 위한 헤더
#include <d3d11.h>
// 다이렉트X11을 사용한 것을 컴파일 해주는 컴파일러 헤더
#include <d3dcompiler.h>

#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")

#define CB_GETBINDSLOT(name) __CBUFFERBINDSLOT__##name##__
#define CBUFFER(name, slot) static const int CB_GETBINDSLOT(name) = slot; struct alignas(16) name

#define CBSLOT_TRANSFORM	0
#define CBSLOT_MATERIAL		1
#define CBSLOT_GRID			2
#define CBSLOT_ANIMATION	3

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

	enum class eRSType
	{
		SolidBack,
		SolidFront,
		SolidNone,
		WireframeNone,
		End,
	};

	enum class eDSType
	{
		LessEqual,
		Greater,
		NoWrite,
		None,
		End,
	};

	enum class eBSType
	{
		Default,
		AlphaBlend,
		OneOne,
		End,
	};

	enum class eRenderingMode
	{
		Opaque,
		Cutout,
		Transparent,
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

		D3D11_BUFFER_DESC desc = {};
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer;

		GpuBuffer() = default;
		virtual ~GpuBuffer() = default;
	};

	enum class eCBType
	{
		Transform,
		Material,
		Grid,
		Animation,
		End,
	};

	enum class eGPUParam
	{
		Int_1,
		Int_2,
		Int_3,
		Int_4,
		Float_1,
		Float_2,
		Float_3,
		Float_4,
		Vector2_1,
		Vector2_2,
		Vector2_3,
		Vector2_4,
		Vector3_1,
		Vector3_2,
		Vector3_3,
		Vector3_4,
		Vector4_1,
		Vector4_2,
		Vector4_3,
		Vector4_4,
		Matrix_1,
		Matrix_2,
		Matrix_3,
		Matrix_4,
	};

	enum class eSRVType
	{
		None,
		End,
	};

	enum class eAnimationType
	{
		None,
		SecondDimension,
		ThirdDimension,
		End,
	};

	struct DebugMesh
	{
		enums::eColliderType type;
		enums::eCollisionState state;
		math::Vector3 position;
		math::Vector3 rotation;
		math::Vector3 scale;
		float radius;
		float duration;
		float time;
	};
}