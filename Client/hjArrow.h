#pragma once
#include "hjActor.h"
#include "hjScript.h"

namespace hj
{
	class Arrow :
		public Actor
	{
	public:
		Arrow();
		virtual ~Arrow();

		virtual void Update() override;

		void DamageOn();

	private:
		void CreateAnimation();
		float mMoveSpeed;
		float mActivateTimer;
		float mActivateLimitTime;
	};
	
	class ArrowFx;
	class ArrowScript :
		public Script
	{
	public:
		ArrowScript();
		virtual ~ArrowScript();

	public:
		virtual void OnCollisionStay(Collider* collider) override;

	public:
		void SetArrowFx(ArrowFx* fx);
		void Hit();

	private:
		ArrowFx* mArrowFx;
	};
}