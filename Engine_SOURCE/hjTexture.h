
#pragma once

#include "..\External\DirectXTex\Include\DirectXTex.h"
#include "hjResource.h"
#include "hjGraphicDevice_DX11.h"


#ifdef _DEBUG
#pragma comment(lib, "..\\External\\DirectXTex\\lib\\Debug\\DirectXTex.lib") 
#else 
#pragma comment(lib, "..\\External\\DirectXTex\\lib\\Release\\DirectXTex.lib") 
#endif

namespace hj::graphics
{
	using namespace hj::math;
	class Texture : public Resource
	{
	public:
		Texture();
		virtual ~Texture();

		virtual HRESULT Load(const std::wstring& name) override;
		void BindShader(eShaderStage stage, UINT slot);
		void Clear();

		const Vector2& GetTexSize() { return mTextureSize; }

	private:
		ScratchImage mImage;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> mTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV;
		D3D11_TEXTURE2D_DESC mDesc;

		Vector2 mTextureSize;
	};
}