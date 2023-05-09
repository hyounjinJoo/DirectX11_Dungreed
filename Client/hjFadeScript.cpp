#include "hjFadeScript.h"
#include "hjRenderer.h"
#include "hjTime.h"
#include "hjInput.h"
#include "hjMeshRenderer.h"
#include "hjComponent.h"
#include "hjGameObject.h"

namespace hj
{
	FadeScript::FadeScript()
		: Script()
		, mFadeState(eFadeState::End)
		, mCurrentTime(0.f)
		, mTotalTransitionTime(3.f)
		, mFadeInStartColor(0.f, 0.f, 0.f, 0.f)
		, mFadeInEndColor(1.f, 0.f, 1.f, 1.f)
		, mFadeOutStartColor(1.f, 0.f, 1.f, 1.f)
		, mFadeOutEndColor(1.f, 1.f, 0.f, 0.f)
		, mCurrentColor(0.f, 0.f, 0.f, 0.f)

	{
	}

	FadeScript::~FadeScript()
	{
	}

	void FadeScript::Initialize()
	{
	}

	void FadeScript::Update()
	{
		switch (mFadeState)
		{
		case eFadeState::FadeInStart:
			mCurrentTime += Time::ActualDeltaTime();
			mCurrentColor = Vector4::Lerp(mFadeInStartColor, mFadeInEndColor, mCurrentTime / mTotalTransitionTime);
			if (mCurrentTime >= mTotalTransitionTime)
			{
				mFadeState = eFadeState::FadeInEnd;
				mCurrentTime = 0.f;
			}
			break;
		case eFadeState::FadeOutStart:
			mCurrentTime += Time::ActualDeltaTime();
			mCurrentColor = Vector4::Lerp(mFadeOutStartColor, mFadeOutEndColor, mCurrentTime / mTotalTransitionTime);
			if (mCurrentTime >= mTotalTransitionTime)
			{
				mFadeState = eFadeState::FadeOutEnd;
				mCurrentTime = 0.f;
			}
			break;
		default:
			break;
		}
	}

	void FadeScript::FixedUpdate()
	{
		MeshRenderer* MR = GetOwner()->GetComponent<MeshRenderer>();
		std::shared_ptr<Material> MTRL = MR->GetMaterial();
		MTRL->SetData(eGPUParam::Vector4_1, &mCurrentColor);
	}

	void FadeScript::Render()
	{
	}

	void FadeScript::SetFadeColor(eFadeState state, const Vector4& color)
	{
		switch (state)
		{
		case eFadeState::FadeInStart:
			mFadeInStartColor = color;
			break;
		case eFadeState::FadeInEnd:
			mFadeInEndColor = color;
			break;
		case eFadeState::FadeOutStart:
			mFadeOutStartColor = color;
			break;
		case eFadeState::FadeOutEnd:
			mFadeOutEndColor = color;
			break;
		default:
			break;
		}
	}
}