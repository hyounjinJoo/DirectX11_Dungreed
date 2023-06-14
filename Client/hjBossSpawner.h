#pragma once
#include "hjActor.h"

namespace hj
{
	class BossSpawner :
		public Actor
	{
	public:
		BossSpawner();
		virtual ~BossSpawner();

	private:
		class BossSpawnerScript* mScript;
	};

	class BossSpawnerScript :
		public Script
	{
	public:
		BossSpawnerScript();
		virtual ~BossSpawnerScript();

		virtual void OnCollisionEnter(Collider* collider) override;

		void SetCollider(class Collider2D* collider);
	private:
		Collider2D* mRectCollider;
	};
}

