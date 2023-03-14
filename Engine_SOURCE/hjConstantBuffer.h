#pragma once
#include "hjGraphics.h"

using namespace hj::enums;

namespace hj::graphics
{
	class ConstantBuffer : GpuBuffer
	{
	public:
		ConstantBuffer(eCBType type);
		virtual ~ConstantBuffer();

		bool Create(size_t size);
		void Bind(void* data);
		void SetPipeline(eShaderStage stage);
		void Clear();

	private:
		const eCBType mType;
	};
}