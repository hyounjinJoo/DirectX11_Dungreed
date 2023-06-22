#pragma once
#include "hjWeapon.h"
#include "hjActor.h"
#include "hjArrow.h"

namespace hj::object::item::weapon
{
	class OakBow :
		public Weapon, public Actor
	{
	public:
		OakBow();
		virtual ~OakBow();

		virtual void Update() override;

		virtual void Attack() final;
		void ChangeToIdle();
		void Shot();

	private:
		void CreateAnimation();

		float mAttackCoolTime;
	};
}