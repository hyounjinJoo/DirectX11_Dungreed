#include "hjConstantBuffer.h"
#include "hjGraphicDevice_DX11.h"

namespace hj::graphics
{
	ConstantBuffer::ConstantBuffer(eCBType type)
		: mType(type)
	{
	}
	ConstantBuffer::~ConstantBuffer()
	{
	}
	bool ConstantBuffer::Create(size_t size)
	{
		// 상수 버퍼 명세		
		desc.ByteWidth = size;
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		if (!GetDevice()->CreateBuffer(&desc, nullptr, buffer.GetAddressOf()))
			return false;

		return true;
	}
	void ConstantBuffer::Bind(void* data)
	{
		GetDevice()->BindConstantBuffer(buffer.Get(), data, desc.ByteWidth);
	}
	void ConstantBuffer::SetPipeline(eShaderStage stage)
	{
		GetDevice()->SetConstantBuffer(stage, mType, buffer.Get());
	}
}