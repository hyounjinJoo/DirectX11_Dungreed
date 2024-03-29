#include "pch.h"
#include "hjShader.h"
#include "hjGraphicDevice_DX11.h"
#include "hjRenderer.h"

using namespace hj::graphics;

namespace hj
{
    Shader::Shader()
		: Resource(eResourceType::GraphicShader)
		, mTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
		, mRSType(eRSType::SolidBack)
		, mDSType(eDSType::LessEqual)
		, mBSType(eBSType::AlphaBlend)
		, mStage(eShaderStage::Count)
    {
    }
    Shader::~Shader()
    {
    }
    HRESULT Shader::Load(const std::wstring& path)
    {
        return E_NOTIMPL;
    }

    void Shader::Create(eShaderStage stage, const std::wstring& file, const std::string& funcName)
    {
        mErrorBlob = nullptr;

        // Shader ����
		std::filesystem::path path = std::filesystem::current_path().parent_path();
		path += "\\..\\SHADER_SOURCE\\";
		
		std::wstring shaderPath(path.c_str());
		shaderPath += file;

		if (stage == eShaderStage::VS)
		{
			D3DCompileFromFile(shaderPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
								,funcName.c_str(), "vs_5_0", 0, 0
								,mVSBlob.GetAddressOf()
								,&mErrorBlob);

			if (mErrorBlob)
			{
				PrintErrorBlobMessage();
				return;
			}
			GetDevice()->CreateVertexShader(mVSBlob->GetBufferPointer()
				, mVSBlob->GetBufferSize()
				, nullptr, mVS.GetAddressOf());
		}
		else if (stage == eShaderStage::PS)
		{
			D3DCompileFromFile(shaderPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
								,funcName.c_str(), "ps_5_0", 0, 0
								,mPSBlob.GetAddressOf()
								,&mErrorBlob);

			if (mErrorBlob)
			{
				PrintErrorBlobMessage();
				return;
			}

			GetDevice()->CreatePixelShader(mPSBlob->GetBufferPointer()
											,mPSBlob->GetBufferSize()
											,nullptr
											,mPS.GetAddressOf());
		}
		else if (stage == eShaderStage::GS)
		{
			D3DCompileFromFile(shaderPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
				, funcName.c_str(), "gs_5_0", 0, 0
				, mGSBlob.GetAddressOf()
				, mErrorBlob.GetAddressOf());

			if (mErrorBlob)
			{
				PrintErrorBlobMessage();
				return;
			}

			GetDevice()->CreateGeometryShader(mGSBlob->GetBufferPointer()
				, mGSBlob->GetBufferSize()
				, nullptr
				, mGS.GetAddressOf());
		}
    }

    void Shader::Binds()
    {
		GetDevice()->BindPrimitiveTopology(mTopology);
		GetDevice()->BindInputLayout(mInputLayout.Get());

		GetDevice()->BindVertexShader(mVS.Get(), nullptr, 0);
		GetDevice()->BindHullShader(mHS.Get(), nullptr, 0);
		GetDevice()->BindDomainShader(mDS.Get(), nullptr, 0);
		GetDevice()->BindGeometryShader(mGS.Get(), nullptr, 0);
		GetDevice()->BindPixelShader(mPS.Get(), nullptr, 0);

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rs = renderer::rasterizerStates[(UINT)mRSType];
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> ds = renderer::depthstencilStates[(UINT)mDSType];
		Microsoft::WRL::ComPtr <ID3D11BlendState> bs = renderer::blendStates[(UINT)mBSType];

		GetDevice()->BindRasterizerState(rs.Get());
		GetDevice()->BindDepthStencilState(ds.Get());
		GetDevice()->BindBlendState(bs.Get());
    }

	inline void Shader::PrintErrorBlobMessage()
	{
		OutputDebugStringA((char*)mErrorBlob->GetBufferPointer());
		mErrorBlob->Release();
		mErrorBlob = nullptr;
	}

	inline void Shader::ReleaseErrorBlob()
	{
		mErrorBlob->Release();
		mErrorBlob = nullptr;
	}
}