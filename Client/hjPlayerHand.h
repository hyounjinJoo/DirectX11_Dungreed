#pragma once
#include "hjHand.h"
namespace hj
{
	class ArmRotatorScript;
	class PlayerHand :
		public Hand
	{
	public:
		PlayerHand();
		virtual ~PlayerHand();

	public:
		virtual void FixedUpdate() override;

	public:
		virtual void SetHandOwner(class Actor* owner) override;
		virtual void Attack() override;

	private:
		void CreateAnimation();

		class Actor* mWeaponMelee;
		class Actor* mWeaponRanged;

		void ResetMeleeWeaponInfo();

	};
}

