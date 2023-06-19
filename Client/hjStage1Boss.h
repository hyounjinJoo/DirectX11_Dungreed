#pragma once
#include "hjMonster.h"

namespace hj
{
	// 보스 패턴
	enum class Boss1AttackPattern
	{
		SwordAttack = 0,	// 검 6개를 소환하여 Aim 후 투척
		BulletAttack,		// 상/하/좌/우의 십자가 모양으로 Bullet이 소환되어 발사되며 십자가 모양이 시계/반시계로 회전.
		LaserPattern1,		// 첫 번째 Laser는 제일 하단으로 고정되고 두 번째 Laser가 캐릭터의 높이를 추적하여 발사된다.
		LaserPattern2,		// Laser가 세 번 발사되며 세 개 모두 높이를 추적
		End
	};

	// 보스 상태
	enum class Boss1State
	{
		StartReady = 0,			// 보스 생성 후 보스 소개 UI가 끝나기 전까지
		Idle,
		Attack,
		AttackEnd,
		Dead,
		End
	};
		
	// 보스 생명력
	// 보스 최대 생명력
	class Boss1Sword;
	class Boss1Hand;
	class Boss1BackgroundEffect;
	class Boss1BackgroundSmallEffect;
	class Boss1Parts;

	class Stage1Boss :
		public Monster
	{
	public:
		Stage1Boss();
		virtual ~Stage1Boss();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		virtual void Death();

	public:
		Vector3 GetBodyPos() { return mDamageBody->GetWorldPosition(); }
		void EndDead() { ChangeBoss1State(Boss1State::End); }
		virtual void Damaged(int damage) override;
		// 본인 몸 외에 나머지를 어떻게 처리할 것인가?
		// 
		// 3. 피격 시 빨갛게 표현

	public:
		void ChangeBoss1State(Boss1State nextState);

	private:
		void SpawnEffect();

		void CreateBodyAnimation();
		virtual void ProcessDamaged(int damage) override;
		// 5. DieFX가 끝나면 턱, 연결부 뼈가 생성되어 떨어진다. 
		void CreateJaws();
		//std::vector<GameObject*> m

		void AdjustColliderPosAndSize();

		void ChangeAnimation(const std::wstring& animWstr, bool loop);
		void SelectAttackPattern();
		void EndAttackPattern();
		void ExecuteAttackPattern();
		void ProcessDead();
		void ProcessEndAll();
		void SpawnBodyParts();

	private:
		void PatternSwordAttack();
		void PatternBulletAttack();
		void PatternLaserAttack1();
		void PatternLaserAttack2();

	private:
		bool ProcessLaserAttackAndCheckShotEnd(Boss1Hand* HandToShot, bool bIsContinuousShot);

	private:
		void CreateSwords();

	private:
		class Animator* mBodyAnimator;
		Boss1Hand* mLeftHand;		// 보스가 보유한 손 객체, Laser를 관리해준다.
		Boss1Hand* mRightHand;

		std::vector<Boss1Sword*> mSwords;

		class Boss1BulletMuzzle* mBulletMuzzle;

		// 보스가 보유한 효과들
		// 1. 뒷 배경 효과
		Boss1BackgroundEffect* mBackground;
		// 2. 파티클
		std::vector<Boss1BackgroundSmallEffect*> mSmallBackgrounds;

		int mCurrentHP;
		int mMaximumHP;
		Actor* mDamageBody;
		class Collider2D* mDamageCollider;

		Boss1State mBossState;						// 상태
		Boss1AttackPattern mBossAttackPattern;		// 공격 패턴

		bool mbAttackExecuted;

		float mSpawnBossTimer = 0.f;
		float mSpawnBossTimeLimit = 5.f;

		float mCurIdleTime = 0.f;
		float mIdleLimitTime = 2.f;

		float mCurSwordSpawnTimer = 0.f;
		float mSwordSpawnInterval = 0.2f;

		class BossDeathParticleSystem* mParticle;

		Boss1Parts* mBossDeadPartHead;
		Boss1Parts* mBossDeadPartJaw;
		Boss1Parts* mBossDeadPartLeftBig;
		Boss1Parts* mBossDeadPartLeftSmall;
		Boss1Parts* mBossDeadPartRightBig;
		Boss1Parts* mBossDeadPartRightSmall;

		class CameraScript* mCameraScript;
	};
}