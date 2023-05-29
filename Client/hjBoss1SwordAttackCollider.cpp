#include "hjBoss1SwordAttackCollider.h"
#include "hjBoss1Sword.h"
#include "hjPlayer.h"
#include "hjCollider2D.h"

namespace hj
{
	Boss1SwordAttackCollider::Boss1SwordAttackCollider()
		: mOwnerObject(nullptr)
	{
		mScript = AddComponent<Boss1SwordAttackColliderScript>();

		Collider2D* collider = AddComponent<Collider2D>();

		if (collider)
		{
			mScript->SetCollider(collider);
		}
	}

	Boss1SwordAttackCollider::~Boss1SwordAttackCollider()
	{
		mOwnerObject = nullptr;
		mScript = nullptr;
	}

	void Boss1SwordAttackCollider::Initialize()
	{
		GameObject::Initialize();
	}

	void Boss1SwordAttackCollider::Update()
	{
		GameObject::Update();
	}

	void Boss1SwordAttackCollider::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void Boss1SwordAttackCollider::Render()
	{
		GameObject::Render();
	}

	void Boss1SwordAttackCollider::SetOwnerObject(Boss1Sword* sword)
	{
		Boss1Sword* verifiedSword = dynamic_cast<Boss1Sword*>(sword);
		if (verifiedSword)
			mOwnerObject = verifiedSword;
	}
}

namespace hj
{

	Boss1SwordAttackColliderScript::Boss1SwordAttackColliderScript()
		: mRectCollider(nullptr)
	{
	}

	Boss1SwordAttackColliderScript::~Boss1SwordAttackColliderScript()
	{
	}

	void Boss1SwordAttackColliderScript::OnCollisionEnter(Collider* collider)
	{
		if (nullptr == mRectCollider || nullptr == collider->GetOwner())
			return;

		Boss1Sword* sword = dynamic_cast<Boss1SwordAttackCollider*>(GetOwner())->GetSword();
		if (nullptr == sword)
			return;

		Player* player = dynamic_cast<Player*>(collider->GetOwner());
		if (nullptr == player)
			return;

		float test = 10.f;
		player->Damaged(test);

		GetOwner()->Pause();
	}

	void Boss1SwordAttackColliderScript::SetCollider(Collider2D* collider)
	{
		if (nullptr == collider)
			return;

		mRectCollider = collider;
	}
}