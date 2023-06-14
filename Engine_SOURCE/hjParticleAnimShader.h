#pragma once
#include "hjComputeShader.h"
#include "hjStructuredBuffer.h"

namespace hj::graphics
{
	class ParticleAnimShader : public ComputeShader
	{
	public:
		ParticleAnimShader();
		~ParticleAnimShader();

		virtual void Binds() override;
		virtual void Clear() override;

		void SetStrcutedBuffer(StructuredBuffer* buffer) { mBuffer = buffer; }
		void SetSharedStructuredBuffer(StructuredBuffer* buffer) { mSharedBuffer = buffer; }
		void SetAnimStructuredBuffer(StructuredBuffer* buffer) { mAnimationBuffer = buffer; }

	private:
		StructuredBuffer* mBuffer;
		StructuredBuffer* mSharedBuffer;
		StructuredBuffer* mAnimationBuffer;
	};
}