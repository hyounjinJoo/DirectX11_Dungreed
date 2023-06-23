#include "hjBossSpawner.h"
#include "hjPlayer.h"
#include "hjStage1BossRoom.h"
#include "hjCollider2D.h"
#include "hjTestScene.h"
#include "hjSceneManager.h"

namespace hj
{
	BossSpawner::BossSpawner()
	{
		mScript = AddComponent<BossSpawnerScript>();
		mScript->SetCollider(AddComponent<Collider2D>());
	}
	BossSpawner::~BossSpawner()
	{
		mScript = nullptr;
	}
}

namespace hj
{
	BossSpawnerScript::BossSpawnerScript()
		: mRectCollider(nullptr)
	{
	}

	BossSpawnerScript::~BossSpawnerScript()
	{
		mRectCollider = nullptr;
	}
	
	void BossSpawnerScript::OnCollisionEnter(Collider* collider)
	{
		if (nullptr == mRectCollider || nullptr == collider->GetOwner())
			return;

		GameObject* other = collider->GetOwner();

		if (dynamic_cast<Player*>(other))
		{
			dynamic_cast<Stage1BossRoom*>(dynamic_cast<Actor*>(GetOwner())->GetOwnerRoom())->SpawnBoss();
			TestScene* scene = dynamic_cast<TestScene*>(SceneManager::GetActiveScene());
			scene->PlayBossBgm();

			GetOwner()->Death();
		}
	}
	void BossSpawnerScript::SetCollider(class Collider2D* collider)
	{
		if (collider)
			mRectCollider = collider;
	}
}