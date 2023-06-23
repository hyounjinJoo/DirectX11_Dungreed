#pragma once
#include "hjActor.h"

namespace hj
{
	struct MonsterStatus
	{
		int currentHP;
		int maxHP;

		int minAttackDamage;
		int maxAttackDamage;

		int exp;
	};

	class Monster :
		public Actor
	{
	public:
		Monster();
		~Monster();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		virtual void Damaged(float damage) {};

		int GetCurrentHP() { return mMonsterStatus.currentHP; }
		int GetMaxHP() { return mMonsterStatus.maxHP; }
		int GetMinAttackDamage() { return mMonsterStatus.minAttackDamage; }
		int GetMaxAttackDamage() { return mMonsterStatus.maxAttackDamage; }
		int GetExp() { return mMonsterStatus.exp; }

		void SetCurrentHP(int curHP) { mMonsterStatus.currentHP = curHP; }
		void SetMaxHP(int maxHP) { mMonsterStatus.maxHP = maxHP; }
		void SetMinAttackDamage(int damage) { mMonsterStatus.minAttackDamage = damage; }
		void SetMaxAttackDamage(int damage) { mMonsterStatus.maxAttackDamage = damage; }
		void SetExp(int exp) { mMonsterStatus.exp = exp; }

		void SubtractCurrentHP(int value);
	protected:
		virtual void ProcessDamaged(float damage) {};
		void FindPlayer();
		void CalcChaseDir();
		float CalcDistanceFromPlayer();

	protected:
		float mDamagedEffectTimer;
		MonsterStatus mMonsterStatus;
		class Player* mPlayer;

		Vector2 mChaseDir;
		float mMoveSpeed;

		bool mbIsDeadMonster;
	};
}