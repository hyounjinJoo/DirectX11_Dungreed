#pragma once
#include "hjComputeShader.h"
#include "hjStructuredBuffer.h"

namespace hj::graphics
{
	class ParticleShader : public ComputeShader
	{
	public:
		ParticleShader();
		~ParticleShader();

		virtual void Binds() override;
		virtual void Clear() override;

		void SetStrcutedBuffer(StructuredBuffer* buffer) { mBuffer = buffer; }
		void SetSharedStructuredBuffer(StructuredBuffer* buffer) { mSharedBuffer = buffer; }

	private:
		StructuredBuffer* mBuffer;
		StructuredBuffer* mSharedBuffer;
	};
}