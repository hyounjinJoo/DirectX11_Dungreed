#pragma once
#include "hjActor.h"

namespace hj
{
	class AttackScript;
	class SwingFx :
		public Actor
	{
	public:
		SwingFx();
		virtual ~SwingFx();

		void PlayFx();
		void StopFx();

		void SetDamageRange(int minDamage, int maxDamage);

	private:
		void CreateAnimation();

	private:
		AttackScript* mAttackScript;
		float mSpawnDistance;
	};
}
