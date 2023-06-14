#include "pch.h"
#include "hjParticleAnimShader.h"
#include "hjRenderer.h"
#include "hjConstantBuffer.h"
#include "hjTime.h"

namespace hj::graphics
{
	ParticleAnimShader::ParticleAnimShader()
		: ComputeShader(128, 1, 1)
	{
	}

	ParticleAnimShader::~ParticleAnimShader()
	{
	}

	void ParticleAnimShader::Binds()
	{
		mBuffer->BindUAV(eShaderStage::CS, 0);
		mSharedBuffer->BindUAV(eShaderStage::CS, 1);
		mAnimationBuffer->BindUAV(eShaderStage::CS, 2);

		mGroupX = mBuffer->GetStride() / mThreadGroupCountX + 1;
		mGroupY = 1;
		mGroupZ = 1;
	}

	void ParticleAnimShader::Clear()
	{
		mBuffer->Clear();
		mSharedBuffer->Clear();
		mAnimationBuffer->Clear();
	}
}
