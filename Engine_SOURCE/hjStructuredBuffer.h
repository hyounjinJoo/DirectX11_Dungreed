#pragma once
#include "hjEntity.h"
#include "hjGraphics.h"

namespace hj::graphics
{
	class StructuredBuffer : public GpuBuffer
	{
	public:
		StructuredBuffer();
		~StructuredBuffer();

		bool Create(UINT size, UINT stride, eSRVType type, void* data);
		void SetData(void* data, UINT bufferCount);
		void Bind(eShaderStage stage, UINT slot);

		UINT GetSize() { return mSize; }
		UINT GetStride() { return mStride; }
		UINT GetTotalSize() { return mSize * mStride; }

	private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV;
		eSRVType mType;

		UINT mSize;
		UINT mStride;
	};
}
