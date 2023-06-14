#pragma once
#include "hjActor.h"

namespace hj
{
	class Boss1BackgroundSmallEffect :
		public Actor
	{
	public:
		Boss1BackgroundSmallEffect();
		Boss1BackgroundSmallEffect(const Boss1BackgroundSmallEffect& fx);
		virtual ~Boss1BackgroundSmallEffect();

		virtual GameObject* Clone();

		virtual void Update() override;
		virtual void FixedUpdate() override;

		void SetStartDelay(float time) { mStartDelayTime = time; }

	private:
		void CreateAnimation();
		void Move();

	private:
		Vector2 mTargetPos;
		Vector2 mInitialPos;
		float mMoveTimer;
		float mMoveLimitTime;

		float mStartDelayTimer;
		float mStartDelayTime;

	};
}

