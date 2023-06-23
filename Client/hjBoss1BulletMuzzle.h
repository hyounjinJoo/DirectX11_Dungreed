#pragma once
#include "hjActor.h"

namespace hj
{
	enum class MuzzleState
	{
		ReadyToShot,
		ShotStart,
		ShotEnd,
		End,
	};

	class Boss1BulletMuzzle :
		public Actor
	{
	public:
		Boss1BulletMuzzle();
		virtual ~Boss1BulletMuzzle();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;

		virtual void Death() override;

	public:
		void ChangeMuzzleState(MuzzleState state);
		MuzzleState GetMuzzleState() { return mMuzzleState; }

	public:
		void CheckNeedToShot();
		void CheckLastBulletPause();
		void Shot();
		void EndPattern();
		
		void PauseAllBullet();
		
	private:
		std::vector<class Boss1Bullet*> mBullets;

	private:
		MuzzleState mMuzzleState;

		float mShotDelayTimer;
		float mShotDelayTime;

		UINT mNextShotBulletIndex;

		float mRotateDir;
		GameObject* mAttackSoundObj;
	};
}