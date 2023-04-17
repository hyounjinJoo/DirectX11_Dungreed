#include "hjTitleBirdLeader.h"

#include <random>
#include "hjApplication.h"
#include "hjTime.h"

extern hj::Application application;

namespace hj
{

	TitleBirdLeader::TitleBirdLeader()
		: TitleBird()
		, mbIsStart(false)
		, mReservedDelayStartTime(0.f)
		, mLoopInterval(0.f)
		, mCurTime(0.f)
	{
		mMoveScript = AddComponent<LinearMoveScript>();
		mMoveScript->SetLoop(true);
		mMoveScript->SetSpeed(35.f);

		Transform* tr = GetTransform();
		Vector2 scale = Vector2::One;
		if (tr)
		{
			scale = tr->GetScaleXY();

			float halfWidth = static_cast<float>(application.GetWidth()) * 0.5f;
			float screenOffPosX = 0.f - halfWidth - scale.x;
			tr->SetPositionX(screenOffPosX);
		}

		mMoveScript->SetMoveDir(eMoveDir::Right, scale * 0.5f);
	}
	TitleBirdLeader::~TitleBirdLeader()
	{
	}
	void TitleBirdLeader::Initialize()
	{
		TitleBird::Initialize();

		if (nullptr == mMoveScript)
		{
			return;
		}
	}

	void TitleBirdLeader::Update()
	{
		TitleBird::Update();

		if (nullptr == mMoveScript)
		{
			return;
		}

		if (!mbIsStart)
		{
			mCurTime +=	Time::ActualDeltaTime();
		
			if (mCurTime >= mReservedDelayStartTime)
			{
				mbIsStart = true;
				
				mMoveScript->SetStart(true);
				mMoveScript->SetLoopIntervalTime(mLoopInterval);
				mMoveScript->SetStartEndY(CalcPosYRandom());
			}
		}

		if (mMoveScript->IsComplete())
		{
			mMoveScript->SetStartEndY(CalcPosYRandom());
		}
	}

	void TitleBirdLeader::FixedUpdate()
	{
		TitleBird::FixedUpdate();

		if (nullptr == mMoveScript)
		{
			return;
		}
	}

	void TitleBirdLeader::Render()
	{
		TitleBird::Render();
	}


	void TitleBirdLeader::SetLoopInterval(float loopIntervalTime)
	{
		mLoopInterval = loopIntervalTime;
	}

	void TitleBirdLeader::SetReservedDelayMoveStart(float reservedDelayStartTime)
	{
		mReservedDelayStartTime = reservedDelayStartTime;
	}

	bool TitleBirdLeader::GetMoveStart()
	{
		if (!mMoveScript)
			return false;

		return mMoveScript->IsStart();
	}

	float TitleBirdLeader::CalcPosYRandom()
	{
#define SCALE_FACTOR 3.f

		if (nullptr == mMoveScript)
			return 0.f;

		auto curTime = std::chrono::system_clock::now();
		auto duration = curTime.time_since_epoch();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
		std::mt19937 randomSeed(static_cast<UINT>(millis));

		float halfHeight = static_cast<float>(application.GetHeight()) * 0.5f;
		halfHeight -= GetScaleY() * SCALE_FACTOR;

		std::uniform_real_distribution<float> valueRange(-halfHeight, halfHeight);

		return valueRange(randomSeed);
	}
}