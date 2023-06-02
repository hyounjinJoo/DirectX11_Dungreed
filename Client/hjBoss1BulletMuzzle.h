#pragma once
#include <hjGameObject.h>

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
		public GameObject
	{
	public:
		Boss1BulletMuzzle();
		virtual ~Boss1BulletMuzzle();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;

	public:
		void ChangeMuzzleState(MuzzleState state);
		MuzzleState GetMuzzleState() { return mMuzzleState; }

	public:
		void CheckNeedToShot();
		void CheckLastBulletPause();
		void Shot();
		void EndPattern();
		
		
	private:
		std::vector<class Boss1Bullet*> mBullets;

	private:
		MuzzleState mMuzzleState;

		float mShotDelayTimer;
		float mShotDelayTime;

		UINT mNextShotBulletIndex;

		float mRotateDir;
	};
}