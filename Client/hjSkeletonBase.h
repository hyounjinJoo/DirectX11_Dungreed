#pragma once
#include "hjMonster.h"

namespace hj
{
	enum class eSkeletonState
	{
		Idle,
		Chase,
		CanAttack,
		Attack,
		Death,
		End
	};

	enum class eSkeletonType
	{
		Sword,
		GreatSword,
		Bow,
		End,
	};

	class SkeletonBase :
		public Monster
	{
	public:
		SkeletonBase();
		virtual ~SkeletonBase();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		virtual void ProcessDamaged(int damage) override;
		void ChangeState(eSkeletonState state);
		void SetType(eSkeletonType type);

		bool IsFlip() { return mbIsFlip; }
		virtual void Damaged(int damage);

		class GameObject* GetCenter() { return mCenterObj; }

		virtual void SetOwnerRoom(RoomBase* room) override;

	private:
		void CreateAnimation();
		void SpawnBodyParts();

		void FlipBasedOnPlayerPos();

	private:
		void CreateSword();
		void CreateGreatSword();
		void CreateBow();

	private:
		eSkeletonType mSkelType;
		eSkeletonState mSkelState;

		class Animator* mAnimator;
		class RigidBody* mRigidBody;

		bool mbIsFlip;

		class Actor* mWeapon;

		GameObject* mCenterObj;
		class SkeletonHand* mHand;
		class ArmRotatorScript* mArmScript;

		float mChaseDistance;
		float mCanAttackDistance;

		float mReadyAttackTimer;
		float mReadyAttackTime;

		Actor* mMonsterBody;
		
	private:
		void Idle();
		void StartChase();
		void Chase();
		void Attack();
		virtual void Death() override;
	};
}