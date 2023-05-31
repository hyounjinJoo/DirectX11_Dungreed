#include "pch.h"
#include "hjComputeShader.h"
#include "hjGraphicDevice_DX11.h"

namespace hj::graphics
{
	ComputeShader::ComputeShader(UINT threadGroupX, UINT threadGroupY, UINT threadGroupZ)
		: Resource(eResourceType::ComputeShader)
		, mCSBlob(nullptr)
		, mCS(nullptr)
		, mThreadGroupCountX(threadGroupX)
		, mThreadGroupCountY(threadGroupY)
		, mThreadGroupCountZ(threadGroupZ)
		, mGroupX(0)
		, mGroupY(0)
		, mGroupZ(0)
	{
	}

	ComputeShader::ComputeShader()
		: Resource(eResourceType::ComputeShader)
		, mCSBlob(nullptr)
		, mCS(nullptr)
		, mErrorBlob(nullptr)
		, mThreadGroupCountX(16)
		, mThreadGroupCountY(16)
		, mThreadGroupCountZ(1)
		, mGroupX(0)
		, mGroupY(0)
		, mGroupZ(0)
	{
	}

	ComputeShader::~ComputeShader()
	{
	}

	HRESULT ComputeShader::Load(const std::wstring& path)
	{
		return E_NOTIMPL;
	}

	bool ComputeShader::Create(const std::wstring& file, const std::string& funcName)
	{
		mErrorBlob = nullptr;

		// Compute Shader
		std::filesystem::path path = std::filesystem::current_path().parent_path();
		path += "\\..\\SHADER_SOURCE\\";

		std::wstring shaderPath(path.c_str());
		shaderPath += file;

		D3DCompileFromFile(shaderPath.c_str(), nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE
			, funcName.c_str(), "cs_5_0", 0, 0
			, mCSBlob.GetAddressOf()
			, mErrorBlob.GetAddressOf());

		if (mErrorBlob)
		{
			PrintErrorBlobMessage();
			return false;
		}

		GetDevice()->CreateComputeShader(mCSBlob->GetBufferPointer()
			, mCSBlob->GetBufferSize()
			, nullptr
			, mCS.GetAddressOf());

		return true;
	}

	void ComputeShader::OnExecute()
	{
		Binds();

		GetDevice()->BindComputeShader(mCS.Get(), nullptr, 0);
		GetDevice()->Dispatch(mGroupX, mGroupY, mGroupZ);

		Clear();
	}

	void ComputeShader::Binds()
	{
	}

	void ComputeShader::Clear()
	{
	}

	inline void ComputeShader::PrintErrorBlobMessage()
	{
		OutputDebugStringA((char*)mErrorBlob->GetBufferPointer());
		mErrorBlob->Release();
		mErrorBlob = nullptr;
	}

	inline void ComputeShader::ReleaseErrorBlob()
	{
		mErrorBlob->Release();
		mErrorBlob = nullptr;
	}

}