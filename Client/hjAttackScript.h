#pragma once
#include <hjScript.h>

namespace hj
{
	class Player;
	class PlayerScript;
	class Actor;

	class AttackScript :
		public Script
	{
	public:
		AttackScript();
		virtual ~AttackScript();

		virtual void Update() override;
		virtual void FixedUpdate() override;

		virtual void OnCollisionStay(Collider* collider) override;

		void SetPlayer(Player* ownerPlayer);
		void SetDamageOn(bool damageOn) { mbDamageOn = damageOn; }

	private:
		Player* mOwnerPlayer;
		PlayerScript* mPlayerScript;
		float mDashDamage;
		bool mbDamageOn;


#pragma region Damageó���� ���� �Լ� �� Variables
	private:
		std::set<Actor*> mDamagedObjects;

	public:
		void ClearDamagedObjects();

	private:
		void AddDamagedObject(Actor* damagedObject);
		void DeleteDamagedObject(Actor* damagedObject);
		bool IsExistDamagedObjectInSet(Actor* damagedObject);
		
#pragma endregion
	};
}