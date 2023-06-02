#pragma once
#include <hjGameObject.h>

namespace hj
{
	class Boss1BulletColliderScript :
		public Script
	{
	public:
		Boss1BulletColliderScript();
		Boss1BulletColliderScript(const Boss1BulletColliderScript& script);
		virtual ~Boss1BulletColliderScript();

		virtual void OnCollisionEnter(Collider* collider) override;

		void SetCollider(class Collider2D* collider);

		virtual Component* Clone() const override;
	private:
		Collider2D* mCircleCollider;
	};
}