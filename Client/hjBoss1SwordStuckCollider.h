#pragma once
#include <hjGameObject.h>

namespace hj
{
	class Boss1Sword;

	class Boss1SwordStuckCollider :
		public GameObject
	{
	public:
		Boss1SwordStuckCollider();
		virtual ~Boss1SwordStuckCollider();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void SetOwnerObject(Boss1Sword* sword);
		Boss1Sword* GetSword() { return mOwnerObject; }
	private:
		Boss1Sword* mOwnerObject;
		class Boss1SwordStuckColliderScript* mScript;
	};


	class Boss1SwordStuckColliderScript :
		public Script
	{
	public:
		Boss1SwordStuckColliderScript();
		virtual ~Boss1SwordStuckColliderScript();

		virtual void OnCollisionEnter(Collider* collider) override;

		void SetCollider(class Collider2D* collider);
	private:
		Collider2D* mRectCollider;
	};

}