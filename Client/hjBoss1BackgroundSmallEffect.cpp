#include "hjBoss1BackgroundSmallEffect.h"
#include "hjAnimator.h"
#include "hjTime.h"
#include <random>

namespace hj
{
	Boss1BackgroundSmallEffect::Boss1BackgroundSmallEffect()
		: mMoveTimer(0.f)
		, mMoveLimitTime(2.5f)
		, mStartDelayTimer(0.f)
		, mStartDelayTime(0.f)
	{
		SetNameAndCreateSpriteRenderer(WIDE("Effect_Boss1_BackgroundSmallEffect"),
			WIDE("MTRL_Monster_Boss_Bellial"), WIDE("Mesh_Rect"));

		// 2. Animator 생성 및 Animation 추가
		Animator* animator = AddComponent<Animator>();
		if (animator)
		{
			CreateAnimation();
		}
	}

	Boss1BackgroundSmallEffect::Boss1BackgroundSmallEffect(const Boss1BackgroundSmallEffect& fx)
		: Actor(fx)
		, mMoveTimer(0.f)
		, mMoveLimitTime(fx.mMoveLimitTime)
		, mStartDelayTimer(0.f)
		, mStartDelayTime(0.f)
	{
	}

	Boss1BackgroundSmallEffect::~Boss1BackgroundSmallEffect()
	{
	}

	GameObject* Boss1BackgroundSmallEffect::Clone()
	{
		return new Boss1BackgroundSmallEffect(*this);
	}

	void Boss1BackgroundSmallEffect::Update()
	{
		if (mStartDelayTimer >= mStartDelayTime)
		{
			GameObject::Update();
		}
	}

	void Boss1BackgroundSmallEffect::FixedUpdate()
	{
		if (mStartDelayTimer < mStartDelayTime)
		{
			mStartDelayTimer += Time::FixedDeltaTime();
		}
		else
		{
			Move();
			GameObject::FixedUpdate();
		}
	}

	void Boss1BackgroundSmallEffect::CreateAnimation()
	{
		CheckHasMaterialAndTexture(eTextureSlot::T0);

		std::wstring path = WIDE("02_Object/02_Monster/01_Boss/Monster_Boss_1_Bellial.xml");
		std::wstring searchWstr = WIDE("SkellBossParticle");
		std::wstring animWstr = WIDE("SkellBossParticle");
		float frameDuration = 2.5f / 16.f;

		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD_OFFSET_TRIM_OFFSET, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration, true, 0, false);
		
		InsertAnimationToAnimator(eTextureSlot::T0, true);

		Animator* animator = GetComponent<Animator>();
		animator->Play(animWstr, false);
	}

	void Boss1BackgroundSmallEffect::Move()
	{
		Vector2 curPos = GetPositionXY();

		if (curPos != mTargetPos)
		{
			mMoveTimer += Time::FixedDeltaTime();
			float moveAlpha = mMoveTimer / mMoveLimitTime;

			Vector2 nextPos = Vector2(std::lerp(curPos.x, mTargetPos.x, moveAlpha), std::lerp(curPos.y, mTargetPos.y, moveAlpha));

			SetPositionXY(nextPos);
		}
		if (curPos == mTargetPos)
		{
			mMoveTimer = 0.f;
			SetPositionXY(mInitialPos);

			std::random_device rd;
			std::mt19937 randomSeed(rd());

#define RANGE_START 40.f
#define RANGE_END 120.f

			std::uniform_real_distribution<float> patternRange(RANGE_START, RANGE_END);
			Vector2 randomPos;

			float randomAxisSelector = (rand() % 2 == 0) ? -1.f : 1.f;
			randomPos.x = patternRange(randomSeed) * randomAxisSelector;

			randomAxisSelector = (rand() % 2 == 0) ? -1.f : 1.f;
			randomPos.y = patternRange(randomSeed) * randomAxisSelector;

			mTargetPos = randomPos;
			SetPositionXY(mTargetPos * 0.8f);

			std::wstring animWstr = WIDE("SkellBossParticle");
			Animator* animator = GetComponent<Animator>();
			animator->Play(animWstr, false);
			mStartDelayTime = 0.f;
		}
	}

}