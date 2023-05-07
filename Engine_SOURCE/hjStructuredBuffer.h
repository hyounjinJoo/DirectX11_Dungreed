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

		bool Create(UINT size, UINT stride, eSRVType type, void* data, bool cpuAccess = false);
		void SetData(void* data, UINT bufferCount);
		void GetData(void* data, UINT size = 0);
		void BindSRV(eShaderStage stage, UINT slot);
		void BindUAV(eShaderStage stage, UINT slot);

		void Clear();

		UINT GetSize() { return mSize; }
		UINT GetStride() { return mStride; }
		UINT GetTotalSize() { return mSize * mStride; }

	private:
		void SetDescription();
		bool CreateBuffer(void* data);
		bool CreateView();
		bool CreateRWBuffer();

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> mWriteBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer> mReadBuffer;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> mUAV;
		eSRVType mType;

		UINT mSize;
		UINT mStride;

		UINT mSRVSlot;
		UINT mUAVSlot;
	};
}
