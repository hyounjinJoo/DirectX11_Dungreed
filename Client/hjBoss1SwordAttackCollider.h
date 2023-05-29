#pragma once
#include <hjGameObject.h>

namespace hj
{
	class Boss1Sword;

	class Boss1SwordAttackCollider :
		public GameObject
	{
	public:
		Boss1SwordAttackCollider();
		virtual ~Boss1SwordAttackCollider();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void SetOwnerObject(Boss1Sword* sword);
		Boss1Sword* GetSword() { return mOwnerObject; }

	private:
		Boss1Sword* mOwnerObject;
		class Boss1SwordAttackColliderScript* mScript;
	};


	class Boss1SwordAttackColliderScript :
		public Script
	{
	public:
		Boss1SwordAttackColliderScript();
		virtual ~Boss1SwordAttackColliderScript();

		virtual void OnCollisionEnter(Collider* collider) override;

		void SetCollider(class Collider2D* collider);
	private:
		void AttackPlayer();

		Collider2D* mRectCollider;
	};

}