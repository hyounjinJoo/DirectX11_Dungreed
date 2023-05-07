#include "hjStructuredBuffer.h"
#include "hjGraphicDevice_DX11.h"

namespace hj::graphics
{
	StructuredBuffer::StructuredBuffer()
		: mSRV(nullptr)
		, mType(eSRVType::SRV)
		, mSize(0)
		, mStride(0)
		, mSRVSlot(0)
		, mUAVSlot(0)
		, mWriteBuffer(nullptr)
		, mReadBuffer(nullptr)
	{


	}

	StructuredBuffer::~StructuredBuffer()
	{

	}

	bool StructuredBuffer::Create(UINT size, UINT stride, eSRVType type, void* data, bool cpuAccess)
	{
		mType = type;
		mSize = size;
		mStride = stride;


		SetDescription();
		CreateBuffer(data);
		CreateView();

		if (cpuAccess)
		{
			CreateRWBuffer();
		}

		return true;
	}

	void StructuredBuffer::SetData(void* data, UINT bufferCount)
	{
		if (mStride < bufferCount)
		{
			Create(mSize, bufferCount, eSRVType::SRV, data);
		}
		else
		{
			GetDevice()->SetData(mWriteBuffer.Get(), data, mSize * bufferCount);
		}
		GetDevice()->CopyResource(buffer.Get(), mWriteBuffer.Get());
	}

	void StructuredBuffer::GetData(void* data, UINT size)
	{
		GetDevice()->CopyResource(mReadBuffer.Get(), buffer.Get());

		//read buffer -> systemmemory
		if (size == 0)
		{
			GetDevice()->SetData(mReadBuffer.Get(), data, mSize * mStride);
		}
		else
		{
			GetDevice()->SetData(mReadBuffer.Get(), data, size);
		}
	}

	void StructuredBuffer::BindSRV(eShaderStage stage, UINT slot)
	{
		mSRVSlot = slot;

		GetDevice()->BindShaderResource(stage, slot, mSRV.GetAddressOf());
	}

	void StructuredBuffer::BindUAV(eShaderStage stage, UINT slot)
	{
		mUAVSlot = slot;
		UINT i = -1;
		GetDevice()->BindUnorderedAccessView(slot, 1, mUAV.GetAddressOf(), &i);
	}

	void StructuredBuffer::Clear()
	{
		ID3D11ShaderResourceView* srv = nullptr;
		GetDevice()->BindShaderResource(eShaderStage::VS, mSRVSlot, &srv);
		GetDevice()->BindShaderResource(eShaderStage::HS, mSRVSlot, &srv);
		GetDevice()->BindShaderResource(eShaderStage::DS, mSRVSlot, &srv);
		GetDevice()->BindShaderResource(eShaderStage::GS, mSRVSlot, &srv);
		GetDevice()->BindShaderResource(eShaderStage::PS, mSRVSlot, &srv);
		GetDevice()->BindShaderResource(eShaderStage::CS, mSRVSlot, &srv);

		ID3D11UnorderedAccessView* uav = nullptr;
		UINT i = -1;
		GetDevice()->BindUnorderedAccessView(mUAVSlot, 1, &uav, &i);
	}

	void StructuredBuffer::SetDescription()
	{
		desc.ByteWidth = mSize * mStride;
		desc.StructureByteStride = mSize;
		desc.Usage = D3D11_USAGE::D3D11_USAGE_DEFAULT;
		desc.CPUAccessFlags = 0;
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
		desc.MiscFlags = D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		if (mType == eSRVType::UAV)
		{
			desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE
				| D3D11_BIND_FLAG::D3D11_BIND_UNORDERED_ACCESS;
		}
		else if (mType == eSRVType::SRV)
		{
			desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
		}
	}

	bool StructuredBuffer::CreateBuffer(void* data)
	{
		if (data)
		{
			D3D11_SUBRESOURCE_DATA tSub = {};
			tSub.pSysMem = data;

			if (!(GetDevice()->CreateBuffer(&desc, &tSub, buffer.GetAddressOf())))
				return false;
		}
		else
		{
			if (!(GetDevice()->CreateBuffer(&desc, nullptr, buffer.GetAddressOf())))
				return false;
		}

		return true;
	}

	bool StructuredBuffer::CreateView()
	{
		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.BufferEx.NumElements = mStride;
		srvDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;

		if (!(GetDevice()->CreateShaderResourceView(buffer.Get(), &srvDesc, mSRV.GetAddressOf())))
			return false;

		if (mType == eSRVType::UAV)
		{
			D3D11_UNORDERED_ACCESS_VIEW_DESC uavDesc = {};
			uavDesc.Buffer.NumElements = mStride;
			uavDesc.ViewDimension = D3D11_UAV_DIMENSION_BUFFER;

			if (!(GetDevice()->CreateUnorderedAccessView(buffer.Get(), &uavDesc, mUAV.GetAddressOf())))
				return false;
		}

		return false;
	}

	bool StructuredBuffer::CreateRWBuffer()
	{
		//mWriteBuffer(nullptr)
		//mReadBuffer(nullptr)

		D3D11_BUFFER_DESC wDesc = { desc };
		wDesc.MiscFlags = D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		wDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		wDesc.Usage = D3D11_USAGE_DYNAMIC;
		wDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		if (!(GetDevice()->CreateBuffer(&wDesc, nullptr, mWriteBuffer.GetAddressOf())))
			return false;

		D3D11_BUFFER_DESC rDesc = { desc };
		rDesc.MiscFlags = D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;
		rDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
		rDesc.Usage = D3D11_USAGE_DEFAULT;
		rDesc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;

		if (!(GetDevice()->CreateBuffer(&rDesc, nullptr, mReadBuffer.GetAddressOf())))
			return false;

		return true;
	}

}
