#include "hjStructuredBuffer.h"
#include "hjGraphicDevice_DX11.h"

namespace hj::graphics
{
	StructuredBuffer::StructuredBuffer()
		: mSRV(nullptr)
		, mType(eSRVType::None)
		, mSize(0)
		, mStride(0)
	{
	}

	StructuredBuffer::~StructuredBuffer()
	{
	}
	
	bool StructuredBuffer::Create(UINT size, UINT stride, eSRVType type, void* data)
	{
		mType = type;
		mSize = size;
		mStride = stride;

		desc.ByteWidth = mSize * mStride;
		desc.StructureByteStride = mSize;
		desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_SHADER_RESOURCE;
		desc.MiscFlags = D3D11_RESOURCE_MISC_FLAG::D3D11_RESOURCE_MISC_BUFFER_STRUCTURED;

		if (data)
		{
			D3D11_SUBRESOURCE_DATA tSub = {};
			tSub.pSysMem = data;

			if(!(GetDevice()->CreateBuffer(&desc, &tSub, buffer.GetAddressOf())))
				return false;
		}
		else
		{
			if (!(GetDevice()->CreateBuffer(&desc, nullptr, buffer.GetAddressOf())))
				return false;
		}

		D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = {};
		srvDesc.BufferEx.NumElements = mStride;
		srvDesc.ViewDimension = D3D_SRV_DIMENSION_BUFFEREX;

		if (!(GetDevice()->CreateShaderResourceView(buffer.Get(), &srvDesc, mSRV.GetAddressOf())))
			return false;

		return true;
	}
	
	void StructuredBuffer::SetData(void* data, UINT bufferCount)
	{
		if (mStride < bufferCount)
		{
			Create(mSize, bufferCount, eSRVType::None, data);
		}
		else
		{
			GetDevice()->BindBuffer(buffer.Get(), data, mSize * bufferCount);
		}
	}
	
	void StructuredBuffer::Bind(eShaderStage stage, UINT slot)
	{
		GetDevice()->BindShaderResource(stage, slot, mSRV.GetAddressOf());
	}
}