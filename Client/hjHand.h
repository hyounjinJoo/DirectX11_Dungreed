#pragma once
#include "hjActor.h"
#include "hjArmRotatorScript.h"

namespace hj
{
	class Hand :
		public Actor
	{
	public:
		Hand();
		virtual ~Hand();

	public:
		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	public:
		virtual void SetHandOwner(class Actor* owner) {};
		void SetWeapon(Actor* weapon);
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
		virtual void Attack() {};

	protected:
		void InitRotatorScript();

	protected:
		class Actor* mHandOwner;
		class Actor* mWeapon;
		Transform* mHandOwnerTR;
		class Animator* mAnimator;
		ArmRotatorScript* mRotatorScript;
	};
}