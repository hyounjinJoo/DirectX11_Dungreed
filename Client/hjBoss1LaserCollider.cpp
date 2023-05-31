#include "hjBoss1LaserCollider.h"
#include "hjPlayer.h"
#include "hjBoss1Laser.h"
#include "hjCollider2D.h"

namespace hj
{

	Boss1LaserCollider::Boss1LaserCollider()
		: mOwnerObject(nullptr)
	{
		mScript = AddComponent<Boss1LaserColliderScript>();

		Collider2D* collider = AddComponent<Collider2D>();
		collider->FixedUpdate();

		if (collider)
		{
			mScript->SetCollider(collider);
		}
	}

	Boss1LaserCollider::~Boss1LaserCollider()
	{
		mOwnerObject = nullptr;
		mScript = nullptr;
	}

	void Boss1LaserCollider::Initialize()
	{
		GameObject::Initialize();
	}

	void Boss1LaserCollider::Update()
	{
		GameObject::Update();
	}

	void Boss1LaserCollider::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void Boss1LaserCollider::Render()
	{
		GameObject::Render();
	}

	void Boss1LaserCollider::SetOwnerObject(Boss1Laser* laser)
	{
		Boss1Laser* verifiedLaser = dynamic_cast<Boss1Laser*>(laser);
		if (verifiedLaser)
			mOwnerObject = verifiedLaser;
	}

	Boss1Laser* Boss1LaserCollider::GetLaser()
	{
		if (mOwnerObject)
		{
			return mOwnerObject;
		}

		return nullptr;
	}

}

namespace hj
{
	Boss1LaserColliderScript::Boss1LaserColliderScript()
		: mRectCollider(nullptr)
	{
	}

	Boss1LaserColliderScript::~Boss1LaserColliderScript()
	{
		mRectCollider = nullptr;
	}

	void Boss1LaserColliderScript::OnCollisionEnter(Collider* collider)
	{
		if (nullptr == mRectCollider || nullptr == collider->GetOwner())
			return;

		Boss1Laser* laser = dynamic_cast<Boss1LaserCollider*>(GetOwner())->GetLaser();
		if (nullptr == laser)
			return;

		Player* player = dynamic_cast<Player*>(collider->GetOwner());
		if (nullptr == player)
			return;

		float test = 10.f;
		player->Damaged(test);

		GetOwner()->Pause();
	}

	void Boss1LaserColliderScript::SetCollider(Collider2D* collider)
	{
		if (nullptr == collider)
			return;

		mRectCollider = collider;
	}


}