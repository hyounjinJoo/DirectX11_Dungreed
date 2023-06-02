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
		Boss1SwordStuckCollider(const Boss1SwordStuckCollider& colliderObj);
		virtual ~Boss1SwordStuckCollider();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		virtual GameObject* Clone() const override;

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
		Boss1SwordStuckColliderScript(const Boss1SwordStuckColliderScript& script);
		virtual ~Boss1SwordStuckColliderScript();

		virtual Component* Clone() const override;

		virtual void OnCollisionEnter(Collider* collider) override;

		void SetCollider(class Collider2D* collider);
	private:
		Collider2D* mRectCollider;
	};

}