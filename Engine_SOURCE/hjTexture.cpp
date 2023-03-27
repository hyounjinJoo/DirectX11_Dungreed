#include "hjTexture.h"

namespace hj::graphics
{
	Texture::Texture()
		: Resource(eResourceType::Texture)
		, mDesc{}
		, mTexture(nullptr)
		, mDSV(nullptr)
		, mRTV(nullptr)
		, mUAV(nullptr)
		, mSRV(nullptr)
	{
	}

	Texture::~Texture()
	{
		if (mTexture)
		{
			mTexture = nullptr;
		}
		if (mDSV)
		{
			mDSV = nullptr;
		}
		if (mRTV)
		{
			mRTV = nullptr;
		}
		if (mUAV)
		{
			mUAV = nullptr;
		}
		if (mSRV)
		{
			mSRV = nullptr;
		}
	}

	HRESULT Texture::Load(const std::wstring& name)
	{
		std::filesystem::path parentPath = std::filesystem::current_path().parent_path();
		std::wstring fullPath = parentPath.wstring() + L"\\..\\Resources\\Texture\\" + name;

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
		mTexture->GetDesc(&mDesc);
		
		mTextureSize.x = (float)mImage.GetImages()[0].width;
		mTextureSize.y = (float)mImage.GetImages()[0].height;

		return S_OK;
	}

	void Texture::BindShader(eShaderStage stage, UINT slot)
	{
		GetDevice()->BindShaderResource(stage, slot, mSRV.GetAddressOf());
	}

	void Texture::BindUnorderedAccessView(UINT startSlot)
	{
		UINT i = -1;
		GetDevice()->BindUnorderedAccessView(startSlot, 1, mUAV.GetAddressOf(), &i);
	}

	void Texture::ClearUnorderedAccessView(UINT startSlot)
	{
		ID3D11UnorderedAccessView* p = nullptr;
		UINT i = -1;
		GetDevice()->BindUnorderedAccessView(startSlot, 1, &p, &i);
	}

	void Texture::Clear(UINT slot)
	{
		ID3D11ShaderResourceView* srv = nullptr;

		GetDevice()->BindShaderResource(eShaderStage::VS, slot, &srv);
		GetDevice()->BindShaderResource(eShaderStage::DS, slot, &srv);
		GetDevice()->BindShaderResource(eShaderStage::GS, slot, &srv);
		GetDevice()->BindShaderResource(eShaderStage::HS, slot, &srv);
		GetDevice()->BindShaderResource(eShaderStage::CS, slot, &srv);
		GetDevice()->BindShaderResource(eShaderStage::PS, slot, &srv);
	}

	bool Texture::Create(UINT width, UINT height, DXGI_FORMAT format, UINT bindFlag)
	{
		if (mTexture)
			mTexture.Reset();

		mDesc.BindFlags = bindFlag;
		mDesc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		mDesc.CPUAccessFlags = 0;
		mDesc.Format = format;
		mDesc.Width = width;
		mDesc.Height = height;
		mDesc.ArraySize = 1;

		mDesc.SampleDesc.Count = 1;
		mDesc.SampleDesc.Quality = 0;

		mDesc.MipLevels = 0;
		mDesc.MiscFlags = 0;

		if (!GetDevice()->CreateTexture(&mDesc, mTexture.GetAddressOf()))
			return false;

		mTexture->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("TextureCreateFunc_1_Texture") - 1, "TextureCreateFunc_1_Texture");

		if (bindFlag & D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL)
		{
			if (mDSV)
				mDSV.Reset();

			if (!GetDevice()->CreateDepthStencilView(mTexture.Get(), nullptr, mDSV.GetAddressOf()))
				return false;

			mDSV->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("TextureCreateFunc_1_DSV") - 1, "TextureCreateFunc_1_DSV");
		}
		
		if (bindFlag & D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE)
		{
			if (mSRV)
				mSRV.Reset();

			D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
			tSRVDesc.Format = format;
			tSRVDesc.Texture2D.MipLevels = 1;
			tSRVDesc.Texture2D.MostDetailedMip = 0;
			tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;

			//if (!GetDevice()->CreateShaderResourceView(mTexture.Get(), &tSRVDesc, mSRV.GetAddressOf()))
			//	return false;
			if (!GetDevice()->CreateShaderResourceView(mTexture.Get(), nullptr, mSRV.GetAddressOf()))
				return false;

			mSRV->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("TextureCreateFunc_1_SRV") - 1, "TextureCreateFunc_1_SRV");
		}
		
		if (bindFlag & D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS)
		{
			if (mUAV)
				mUAV.Reset();

			D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
			tUAVDesc.Format = format;
			tUAVDesc.Texture2D.MipSlice = 0;
			tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;

			//if (!GetDevice()->CreateUnorderedAccessView(mTexture.Get(), &tUAVDesc, mUAV.GetAddressOf()))
			//	return false;

			if (!GetDevice()->CreateUnorderedAccessView(mTexture.Get(), nullptr, mUAV.GetAddressOf()))
				return false;

			mUAV->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("TextureCreateFunc_1_UAV") - 1, "TextureCreateFunc_1_UAV");
		}

		return true;
	}

	bool Texture::Create(Microsoft::WRL::ComPtr<ID3D11Texture2D> texture)
	{
		if (mTexture)
			mTexture.Reset();

		mTexture = texture;
		mTexture->GetDesc(&mDesc);

		if (mDesc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL)
		{
			if (mDSV)
				mDSV.Reset();

			if (!GetDevice()->CreateDepthStencilView(mTexture.Get(), nullptr, mDSV.GetAddressOf()))
				return false;

			mDSV->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("TextureCreateFunc_2_DSV") - 1, "TextureCreateFunc_2_DSV");
		}

		if (mDesc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_RENDER_TARGET)
		{
			if (mRTV)
				mRTV.Reset();

			if (!GetDevice()->CreateRenderTargetView(mTexture.Get(), nullptr, mRTV.GetAddressOf()))
				return false;

			mRTV->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("TextureCreateFunc_2_RTV") - 1, "TextureCreateFunc_2_RTV");
		}

		if (mDesc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE)
		{
			if (mSRV)
				mSRV.Reset();

			D3D11_SHADER_RESOURCE_VIEW_DESC tSRVDesc = {};
			tSRVDesc.Format = mDesc.Format;
			tSRVDesc.Texture2D.MipLevels = 1;
			tSRVDesc.Texture2D.MostDetailedMip = 0;
			tSRVDesc.ViewDimension = D3D11_SRV_DIMENSION::D3D11_SRV_DIMENSION_TEXTURE2D;

			//if (!GetDevice()->CreateShaderResourceView(mTexture.Get(), &tSRVDesc, mSRV.GetAddressOf()))
			//	return false;

			if (!GetDevice()->CreateShaderResourceView(mTexture.Get(), nullptr, mSRV.GetAddressOf()))
				return false;

			mSRV->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("TextureCreateFunc_2_SRV") - 1, "TextureCreateFunc_2_SRV");
		}

		if (mDesc.BindFlags & D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS)
		{
			if (mUAV)
				mUAV.Reset();

			D3D11_UNORDERED_ACCESS_VIEW_DESC tUAVDesc = {};
			tUAVDesc.Format = mDesc.Format;
			tUAVDesc.Texture2D.MipSlice = 0;
			tUAVDesc.ViewDimension = D3D11_UAV_DIMENSION::D3D11_UAV_DIMENSION_TEXTURE2D;

			//if (!GetDevice()->CreateUnorderedAccessView(mTexture.Get(), &tUAVDesc, mUAV.GetAddressOf()))
			//	return false;

			if (!GetDevice()->CreateUnorderedAccessView(mTexture.Get(), nullptr, mUAV.GetAddressOf()))
				return false;

			mUAV->SetPrivateData(WKPDID_D3DDebugObjectName, sizeof("TextureCreateFunc_2_UAV") - 1, "TextureCreateFunc_2_UAV");
		}

		return true;
	}

}