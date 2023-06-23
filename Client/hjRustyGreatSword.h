#pragma once
#include "hjActor.h"
#include "hjWeapon.h"

namespace hj
{
	class AttackScript;
}

namespace hj::object::item::weapon
{
	class RustyGreatSword :
		public Weapon, public Actor
	{
	public:
		RustyGreatSword();
		virtual ~RustyGreatSword();

		virtual void Initialize() final;
		virtual void Update() final;
		virtual void FixedUpdate() final;
		virtual void Render() final;

		virtual void Attack() final;

	private:
		void CreateAnimation();
		AttackScript* mAttackScript;

	private:
		void DamageOn();
		void DamageOff();
	};
}