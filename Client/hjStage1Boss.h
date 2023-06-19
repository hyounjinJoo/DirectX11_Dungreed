#pragma once
#include "hjMonster.h"

namespace hj
{
	// ���� ����
	enum class Boss1AttackPattern
	{
		SwordAttack = 0,	// �� 6���� ��ȯ�Ͽ� Aim �� ��ô
		BulletAttack,		// ��/��/��/���� ���ڰ� ������� Bullet�� ��ȯ�Ǿ� �߻�Ǹ� ���ڰ� ����� �ð�/�ݽð�� ȸ��.
		LaserPattern1,		// ù ��° Laser�� ���� �ϴ����� �����ǰ� �� ��° Laser�� ĳ������ ���̸� �����Ͽ� �߻�ȴ�.
		LaserPattern2,		// Laser�� �� �� �߻�Ǹ� �� �� ��� ���̸� ����
		End
	};

	// ���� ����
	enum class Boss1State
	{
		StartReady = 0,			// ���� ���� �� ���� �Ұ� UI�� ������ ������
		Idle,
		Attack,
		AttackEnd,
		Dead,
		End
	};
		
	// ���� �����
	// ���� �ִ� �����
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
		// ���� �� �ܿ� �������� ��� ó���� ���ΰ�?
		// 
		// 3. �ǰ� �� ������ ǥ��

	public:
		void ChangeBoss1State(Boss1State nextState);

	private:
		void SpawnEffect();

		void CreateBodyAnimation();
		virtual void ProcessDamaged(int damage) override;
		// 5. DieFX�� ������ ��, ����� ���� �����Ǿ� ��������. 
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
		Boss1Hand* mLeftHand;		// ������ ������ �� ��ü, Laser�� �������ش�.
		Boss1Hand* mRightHand;

		std::vector<Boss1Sword*> mSwords;

		class Boss1BulletMuzzle* mBulletMuzzle;

		// ������ ������ ȿ����
		// 1. �� ��� ȿ��
		Boss1BackgroundEffect* mBackground;
		// 2. ��ƼŬ
		std::vector<Boss1BackgroundSmallEffect*> mSmallBackgrounds;

		int mCurrentHP;
		int mMaximumHP;
		Actor* mDamageBody;
		class Collider2D* mDamageCollider;

		Boss1State mBossState;						// ����
		Boss1AttackPattern mBossAttackPattern;		// ���� ����

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