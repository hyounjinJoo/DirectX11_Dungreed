#pragma once
#include <hjGameObject.h>

namespace hj
{
	class Boss1Sword;

	class Boss1SwordCollider :
		public GameObject
	{
	public:
		Boss1SwordCollider();
		virtual ~Boss1SwordCollider();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void SetOwnerObject(Boss1Sword* sword);
		Boss1Sword* GetSword() { return mOwnerObject; }
	private:
		Boss1Sword* mOwnerObject;
		class Boss1SwordColliderScript* mScript;
	};


	class Boss1SwordColliderScript :
		public Script
	{
	public:
		Boss1SwordColliderScript();
		virtual ~Boss1SwordColliderScript();

		virtual void OnCollisionEnter(Collider* collider) override;

		void SetCollider(class Collider2D* collider);
	private:
		Collider2D* mRectCollider;
	};

}