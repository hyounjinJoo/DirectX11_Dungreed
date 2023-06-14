#pragma once
#include "hjActor.h"

namespace hj
{
	class Monster :
		public Actor
	{
	public:
		Monster();
		~Monster();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		virtual void Damaged(float damage) {};

	protected:
		virtual void ProcessDamaged(float damage) {};

	protected:
		float mDamagedEffectTimer;
	};
}