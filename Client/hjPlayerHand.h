#pragma once
#include "hjActor.h"
namespace hj
{
	class ArmRotatorScript;
	class PlayerHand :
		public Actor
	{
		enum class handState
		{
			Normal,
			Inverse,
			End
		};

	public:
		PlayerHand();
		virtual ~PlayerHand();

		void Initialize() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() override;

	public:
		void SetHandOwner(class Player* owner);
		void InverseHandPosZ(bool inverse);

		Transform* GetWeaponTR();
		Transform* GetHandOwnerTR()
		{
			if (mHandOwnerTR)
			{
				return mHandOwnerTR;
			}

			return nullptr;
		}
		
		ArmRotatorScript* GetRotatorScript()
		{
			if (mRotatorScript)
			{
				return mRotatorScript;
			}

			return nullptr;
		}

	public:
		void Attack();

	private:
		void CreateAnimation();

	private:
		class Actor* mWeapon;
		Player* mHandOwner;
		Transform* mHandOwnerTR;
		Transform* mHandTransform;
		handState mHandState;
		class Animator* mAnimator;
		ArmRotatorScript* mRotatorScript;
	};
}

