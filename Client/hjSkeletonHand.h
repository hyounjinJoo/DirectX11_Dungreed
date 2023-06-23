#pragma once
#include "hjHand.h"
#include "hjArmRotatorScript.h"

namespace hj
{
	class SkeletonHand :
		public Hand
	{
	public:
		SkeletonHand();
		virtual ~SkeletonHand();

	public:
		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	public:
		virtual void SetHandOwner(class Actor* owner) override;

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
	};
}