#include "hjShader.h"
#include "hjGraphicDevice_DX11.h"

using namespace hj::graphics;

namespace hj
{
    Shader::Shader()
		: Resource(eResourceType::GraphicShader)
		, mTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
    {
    }
    Shader::~Shader()
    {
    }
    HRESULT Shader::Load(const std::wstring& path)
    {
        return E_NOTIMPL;
    }

    void Shader::Create(graphics::eShaderStage stage, const std::wstring& file, const std::string& funcName)
    {
        mErrorBlob = nullptr;

        // Shader »ý¼º
		std::filesystem::path path = std::filesystem::current_path().parent_path();
		path += "\\SHADER_SOURCE\\";

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
    }

    void Shader::Binds()
    {
		GetDevice()->BindPrimitiveTopology(mTopology);
		GetDevice()->BindInputLayout(mInputLayout.Get());

		GetDevice()->BindVertexShader(mVS.Get(), nullptr, 0);
		GetDevice()->BindPixelShader(mPS.Get(), nullptr, 0);
    }

	inline void Shader::PrintErrorBlobMessage()
	{
		OutputDebugStringA((char*)mErrorBlob->GetBufferPointer());
	}

	inline void Shader::ReleaseErrorBlob()
	{
		mErrorBlob->Release();
		mErrorBlob = nullptr;
	}
}