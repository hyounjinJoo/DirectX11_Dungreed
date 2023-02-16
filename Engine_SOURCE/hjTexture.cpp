#include "hjTexture.h"

namespace hj::graphics
{
	Texture::Texture()
		: Resource(eResourceType::Texture)
		, mDesc{}
	{
	}

	Texture::~Texture()
	{
	}

	HRESULT Texture::Load(const std::wstring& name)
	{
		std::filesystem::path parentPath = std::filesystem::current_path().parent_path();
		std::wstring fullPath = parentPath.wstring() + L"\\Resources\\Texture\\" + name;

		wchar_t szExtension[256] = {};
		_wsplitpath_s(name.c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExtension, 256);

		std::wstring extension(szExtension);

		if (extension == L".dds" || extension == L".DDS")
		{
			if (FAILED(LoadFromDDSFile(fullPath.c_str(), DDS_FLAGS::DDS_FLAGS_NONE, nullptr, mImage)))
				return S_FALSE;
		}
		else if (extension == L".tga" || extension == L".TGA")
		{
			if (FAILED(LoadFromTGAFile(fullPath.c_str(), nullptr, mImage)))
				return S_FALSE;
		}
		else // WIC (png, jpg, jpeg, bmp )
		{
			if (FAILED(LoadFromWICFile(fullPath.c_str(), WIC_FLAGS::WIC_FLAGS_NONE, nullptr, mImage)))
				return S_FALSE;
		}

		CreateShaderResourceView
		(
			GetDevice()->GetID3D11Device(),
			mImage.GetImages(),
			mImage.GetImageCount(),
			mImage.GetMetadata(),
			mSRV.GetAddressOf()
		);

		mSRV->GetResource((ID3D11Resource**)mTexture.GetAddressOf());

		return S_OK;
	}

	void Texture::BindShader(eShaderStage stage, UINT slot)
	{
		GetDevice()->SetShaderResource(stage, slot, mSRV.GetAddressOf());
	}
	void Texture::Clear()
	{
		ID3D11ShaderResourceView* srv = nullptr;

		GetDevice()->SetShaderResource(eShaderStage::VS, 0, &srv);
		GetDevice()->SetShaderResource(eShaderStage::DS, 0, &srv);
		GetDevice()->SetShaderResource(eShaderStage::GS, 0, &srv);
		GetDevice()->SetShaderResource(eShaderStage::HS, 0, &srv);
		GetDevice()->SetShaderResource(eShaderStage::CS, 0, &srv);
		GetDevice()->SetShaderResource(eShaderStage::PS, 0, &srv);
	}
}