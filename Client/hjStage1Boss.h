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
		Dead,
		End
	};
		
	// ���� �����
	// ���� �ִ� �����

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

		virtual void Damaged(float damage) override;
		// ���� �� �ܿ� �������� ��� ó���� ���ΰ�?
		// 
		// 3. �ǰ� �� ������ ǥ��
	private:
		void CreateBodyAnimation();
		virtual void ProcessDamaged(float damage) override;
		// 4. ����� DieFX�� ����Ͽ� ���� ��ġ�� ǥ�� �� ���� ��ġ���� ���ɿ����� ���� �������� Ŀ�� ��ġ�� ȿ���� �����ȴ�.
		virtual void Dead();
		// 5. DieFX�� ������ ��, ����� ���� �����Ǿ� ��������. 
		void CreateJaws();
		//std::vector<GameObject*> m

		void AdjustColliderPosAndSize();

		bool IsAttackEnd();
		void ChangeState(Boss1State nextState);
		void ChangeAnimation(const std::wstring& animWstr, bool loop);
		void SelectAttackPattern();
		void ExecuteAttackPattern();
		void ProcessDead();

	private:
		void PatternSwordAttack();
		void PatternBulletAttack();
		void PatternLaserAttack1();
		void PatternLaserAttack2();

	private:
		class Animator* mBodyAnimator;
		class Stage1BossHand* mLeftHand;		// ������ ������ �� ��ü, Laser�� �������ش�.
		Stage1BossHand* mRightHand;

		std::vector<GameObject*> mBullets;
		std::vector<GameObject*> mSwords;

		class Boss1Sword* mBoss1Sword;
		class Boss1Laser* mLaser;

		// ������ ������ ȿ����
		// 1. �� ��� ȿ��
		GameObject* mBackground;
		// 2. ��ƼŬ
		GameObject* mAuraManager;

		float mCurrentHP;
		float mMaximumHP;
		GameObject* mDamageBody;
		class Collider2D* mDamageCollider;

		Boss1State mBossState;						// ����
		Boss1AttackPattern mBossAttackPattern;		// ���� ����

		bool mbAttackExecuted;


	};
}