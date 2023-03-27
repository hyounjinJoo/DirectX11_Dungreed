#include "hjPaintShader.h"
#include "hjConstantBuffer.h"
#include "hjRenderer.h"
#include "hjTime.h"

hj::graphics::PaintShader::PaintShader()
	: ComputeShader()
	, mTarget(nullptr)
	, mNoiseTexture(nullptr)
	, mCurrentTime(0.f)
	, mThresHold(1.f)
{
}

hj::graphics::PaintShader::~PaintShader()
{
}

void hj::graphics::PaintShader::Binds()
{
	if (!mTarget || !mNoiseTexture)
		return;

	mCurrentTime += Time::DeltaTime();
	if (mCurrentTime < mThresHold)
		return;

	mCurrentTime = 0.f;

	mNoiseTexture->BindShader(eShaderStage::CS, 10);

	renderer::GlobalCB globalCb = {};
	globalCb.globalDeltaTime = Time::DeltaTime();
	globalCb.globalNoiseResolution = mNoiseTexture->GetTexSize();
	globalCb.globalResolution = Vector2(1600.f, 900.f);
	globalCb.globalAccTime = Time::AccTime();

	ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::Global];
	cb->Bind(&globalCb);
	cb->SetPipeline(eShaderStage::CS);


	mTarget->BindUnorderedAccessView(0);
		
	mGroupX = static_cast<UINT>(mTarget->GetWidth() / mThreadGroupCountX) + 1;
	mGroupY = static_cast<UINT>(mTarget->GetHeight() / mThreadGroupCountY) + 1;
	mGroupZ = 1;

}

void hj::graphics::PaintShader::Clear()
{
	mTarget->ClearUnorderedAccessView(0);
}
