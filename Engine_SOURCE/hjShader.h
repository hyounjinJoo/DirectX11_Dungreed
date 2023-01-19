#pragma once

#include "hjResource.h"
#include "hjGraphics.h"

namespace hj
{
	class Shader : public Resource
	{
	public:
		Shader();
		~Shader();

		virtual HRESULT Load(const std::wstring& path) override;

		void Create(graphics::eShaderStage stage, const std::wstring& file, const std::string& funcName);
		void Binds();

		ID3D11InputLayout* GetInputLayout() { return mInputLayout.Get(); }
		ID3D11InputLayout** GetInputLayoutAddressOf() { return mInputLayout.GetAddressOf(); }

		void* GetVSBlobBufferPointer() { return mVSBlob->GetBufferPointer(); }
		SIZE_T GetVSBlobBufferSize() { return mVSBlob->GetBufferSize(); }

		inline void PrintErrorBlobMessage();
		inline void ReleaseErrorBlob();

	private:
		Microsoft::WRL::ComPtr<ID3D11InputLayout> mInputLayout;
		D3D11_PRIMITIVE_TOPOLOGY mTopology;
		graphics::eShaderStage mStage;

		Microsoft::WRL::ComPtr<ID3DBlob> mVSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> mHSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> mDSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> mGSBlob;
		Microsoft::WRL::ComPtr<ID3DBlob> mPSBlob;

		Microsoft::WRL::ComPtr<ID3D11VertexShader>		mVS;
		Microsoft::WRL::ComPtr<ID3D11HullShader>		mHS;
		Microsoft::WRL::ComPtr<ID3D11DomainShader>		mDS;
		Microsoft::WRL::ComPtr<ID3D11GeometryShader>	mGS;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>		mPS;

		Microsoft::WRL::ComPtr<ID3DBlob> mErrorBlob;
	};
}