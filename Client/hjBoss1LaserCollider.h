#pragma once
#include <hjGameObject.h>

namespace hj
{
	class Boss1Laser;

	class Boss1LaserCollider :
		public GameObject
	{
	public:
		Boss1LaserCollider();
		virtual ~Boss1LaserCollider();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void SetOwnerObject(Boss1Laser* laser);
		Boss1Laser* GetLaser();

	private:
		Boss1Laser* mOwnerObject;
		class Boss1LaserColliderScript* mScript;
	};


	class Boss1LaserColliderScript :
		public Script
	{
	public:
		Boss1LaserColliderScript();
		Boss1LaserColliderScript(const Boss1LaserColliderScript& script);
		virtual ~Boss1LaserColliderScript();

		virtual void OnCollisionStay(Collider* collider) override;

		void SetCollider(class Collider2D* collider);

		virtual Component* Clone() const override;
	private:
		Collider2D* mRectCollider;
	};

}