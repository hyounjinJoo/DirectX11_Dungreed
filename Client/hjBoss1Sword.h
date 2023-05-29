#pragma once
#include <hjGameObject.h>

namespace hj
{
	enum class Boss1SwordState
	{
		Spawn,
		Aim,
		Shot,
		Stuck,
		End,
	};

	class Boss1Sword :
		public GameObject
	{
	public:
		Boss1Sword();
		virtual ~Boss1Sword();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		Vector2 GetHitEffectScaleXY();
		void SetHitEffectPosAndRot(const Vector2& newPos, float rotZ);

		void ChangeSwordState(Boss1SwordState nextState);
		Boss1SwordState GetSwordState() { return mSwordState; }

		void PlayEndHitFX();

		void SetSpawnPosXY(const Vector2& posXY) { mSpawnPosXY = posXY; }

	private:
		void Spawn();
		void CheckSpawnEnd();
		void Aim();
		void Shot();
		void Move();
		void Stuck();

	private:
		void CreateAnimation();
		void AimPlayer();
		void AimComplete();

		void SpawnComplete();

		void AttackEnd();

		GameObject* mPlayer;
		GameObject* mChargeEffectObj;
		GameObject* mHitEffectObj;
		class Boss1SwordStuckCollider* mSwordMoveCollider;
		class Boss1SwordAttackCollider* mSwordAttackCollider;

		Boss1SwordState mSwordState;

		float mCurSpawnedTime;
		float mSpawnEndTime;

		float mCurAimingTime;
		float mAimingLimitTime;

		float mCurEndTime = 0.f;
		float mEndLimitTime = 3.f;

		Vector2 mMoveDir;
		float mMoveSpeed;
		Vector2 mMoveVelocity;

		Vector2 mSpawnPosXY;
	};
}