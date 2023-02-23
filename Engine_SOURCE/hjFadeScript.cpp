#include "hjFadeScript.h"
#include "hjRenderer.h"
#include "hjTime.h"
#include "hjInput.h"
#include "hjMeshRenderer.h"
#include "hjComponent.h"
#include "hjGameObject.h"
#include <random>

namespace hj
{
	FadeScript::FadeScript()
		: Script()
		//, mFadeState(eFadeState::End)
		, mFadeState(eFadeState::FadeInStart)
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

		//if (mFadeState != eFadeState::FadeInStart && mFadeState != eFadeState::FadeOutStart)
		//{
		//	if (Input::GetKeyState(eKeyCode::I) == eKeyState::DOWN)
		//	{
		//		mFadeState = eFadeState::FadeInStart;
		//		//mCurrentColor.w += 1.f * Time::DeltaTime();
		//	}
		//	else if (Input::GetKeyState(eKeyCode::O) == eKeyState::DOWN)
		//	{
		//		mFadeState = eFadeState::FadeOutStart;
		//		//mCurrentColor.w -= 1.f * Time::DeltaTime();
		//	}
		//}


		switch (mFadeState)
		{
		case hj::FadeScript::eFadeState::FadeInStart:
			mCurrentTime += Time::DeltaTime();
			mCurrentColor = Vector4::Lerp(mFadeInStartColor, mFadeInEndColor, mCurrentTime / mTotalTransitionTime);
			if (mCurrentTime >= mTotalTransitionTime)
			{
				//mFadeState = eFadeState::FadeInEnd;
				{
					mFadeState = eFadeState::FadeOutStart;
					mFadeOutStartColor = mFadeInEndColor;
					SetFadeColor(eFadeState::FadeInStart, mFadeInStartColor);
					SetFadeColor(eFadeState::FadeInEnd, mFadeInEndColor);
				}
				mCurrentTime = 0.f;
			}
			break;
		case hj::FadeScript::eFadeState::FadeOutStart:
			mCurrentTime += Time::DeltaTime();
			mCurrentColor = Vector4::Lerp(mFadeOutStartColor, mFadeOutEndColor, mCurrentTime / mTotalTransitionTime);
			if (mCurrentTime >= mTotalTransitionTime)
			{
				//mFadeState = eFadeState::FadeOutEnd;
				{
					mFadeState = eFadeState::FadeInStart;
					mFadeInStartColor = mFadeOutEndColor;
					SetFadeColor(eFadeState::FadeOutStart, mFadeOutStartColor);
					SetFadeColor(eFadeState::FadeOutEnd, mFadeOutEndColor);
				}
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
		//switch (state)
		//{
		//case hj::FadeScript::eFadeState::FadeInStart:
		//	mFadeInStartColor = color;
		//	break;
		//case hj::FadeScript::eFadeState::FadeInEnd:
		//	mFadeInEndColor = color;
		//	break;
		//case hj::FadeScript::eFadeState::FadeOutStart:
		//	mFadeOutStartColor = color;
		//	break;
		//case hj::FadeScript::eFadeState::FadeOutEnd:
		//	mFadeOutEndColor = color;
		//	break;
		//default:
		//	break;
		//}

		auto curTime = std::chrono::system_clock::now();
		auto duration = curTime.time_since_epoch();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
		std::mt19937 randomSeed(millis);

		std::uniform_real_distribution<float> valueRange(0.f, 1.1f);

		switch (state)
		{
		case hj::FadeScript::eFadeState::FadeInStart:
			mFadeInStartColor = Vector4(valueRange(randomSeed), valueRange(randomSeed), valueRange(randomSeed), color.w);
			break;
		case hj::FadeScript::eFadeState::FadeInEnd:
			mFadeInEndColor = Vector4(valueRange(randomSeed), valueRange(randomSeed), valueRange(randomSeed), color.w);
			break;
		case hj::FadeScript::eFadeState::FadeOutStart:
			mFadeOutStartColor = Vector4(valueRange(randomSeed), valueRange(randomSeed), valueRange(randomSeed), color.w);
			break;
		case hj::FadeScript::eFadeState::FadeOutEnd:
			mFadeOutEndColor = Vector4(valueRange(randomSeed), valueRange(randomSeed), valueRange(randomSeed), color.w);
			break;
		default:
			break;
		}
	}
}