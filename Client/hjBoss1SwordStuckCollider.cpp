#include "hjBoss1SwordStuckCollider.h"
#include "hjCollider2D.h"
#include "hjBoss1Sword.h"
#include "hjRoomNotPass.h"

namespace hj
{
	Boss1SwordStuckCollider::Boss1SwordStuckCollider()
		: mOwnerObject(nullptr)
	{
		mScript = AddComponent<Boss1SwordStuckColliderScript>();

		Collider2D* collider = AddComponent<Collider2D>();
		
		if (collider)
		{
			Vector2 size = Vector2(1.f, 1.f);
			collider->SetSize(size);
			mScript->SetCollider(collider);
		}
	}

	Boss1SwordStuckCollider::~Boss1SwordStuckCollider()
	{
		mOwnerObject = nullptr;
		mScript = nullptr;
	}

	void Boss1SwordStuckCollider::Initialize()
	{
		GameObject::Initialize();
	}

	void Boss1SwordStuckCollider::Update()
	{
		GameObject::Update();
	}

	void Boss1SwordStuckCollider::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void Boss1SwordStuckCollider::Render()
	{
		GameObject::Render();
	}

	void Boss1SwordStuckCollider::SetOwnerObject(Boss1Sword* sword)
	{
		Boss1Sword* verifiedSword = dynamic_cast<Boss1Sword*>(sword);
		if (verifiedSword)
			mOwnerObject = verifiedSword;
	}

}

namespace hj
{
	Boss1SwordStuckColliderScript::Boss1SwordStuckColliderScript()
		: mRectCollider(nullptr)
	{
	}

	Boss1SwordStuckColliderScript::~Boss1SwordStuckColliderScript()
	{
	}

#define NOT_DETERMINE_CASE		0
#define LEFT_COLLISION_CASE		1
#define RIGHT_COLLISION_CASE	2
#define BOTTOM_COLLISION_CASE	3
#define TOP_COLLISION_CASE		4
	void Boss1SwordStuckColliderScript::OnCollisionEnter(Collider* collider)
	{
		if (nullptr == mRectCollider || nullptr == collider->GetOwner())
			return;

		Boss1Sword* sword = dynamic_cast<Boss1SwordStuckCollider*>(GetOwner())->GetSword();
		if (nullptr == sword)
			return;

		RoomNotPass* notPass = dynamic_cast<RoomNotPass*>(collider->GetOwner());
		if (nullptr == notPass)
			return;

		Vector2 thisCenter = GetOwner()->GetWorldPositionXY();
		Vector4 notPassLTRB = notPass->GetWorldLTRB();

		UINT enterCase = NOT_DETERMINE_CASE;
		if (thisCenter.y >= notPassLTRB.y)
			enterCase = TOP_COLLISION_CASE;
		else if (thisCenter.x <= notPassLTRB.x)
			enterCase = RIGHT_COLLISION_CASE;
		else if (thisCenter.x >= notPassLTRB.z)
			enterCase = LEFT_COLLISION_CASE;
		else if (thisCenter.y <= notPassLTRB.w)
			enterCase = BOTTOM_COLLISION_CASE;

		float newRotZ = 0.f;
		Vector2 newPos = thisCenter;
		Vector2 hitEffectScale = sword->GetHitEffectScaleXY();
		switch (enterCase)
		{
		case TOP_COLLISION_CASE:
			newRotZ = 0.f;
			newPos.y = notPassLTRB.y;
			hitEffectScale.y *= 0.5f;
			newPos.y += hitEffectScale.y;
			break;
		case LEFT_COLLISION_CASE:
			newRotZ -= XM_PI * 0.5f;
			newPos.x = notPassLTRB.z;
			newPos.x += hitEffectScale.y * 0.5f;
			break;
		case RIGHT_COLLISION_CASE:
			newRotZ += XM_PI * 0.5f;
			newPos.x = notPassLTRB.x;
			newPos.x -= hitEffectScale.y * 0.5f;
			break;
		case BOTTOM_COLLISION_CASE:
			newRotZ = XM_PI;
			newPos.y = notPassLTRB.w;
			hitEffectScale.y *= 0.5f;
			newPos.y -= hitEffectScale.y;
			break;
		default:
			break;
		}

		sword->ChangeSwordState(Boss1SwordState::Stuck);
		float notPassPosZ = notPass->GetWorldPositionZ();
		sword->SetPositionZ(3.1f);
		sword->SetHitEffectPosAndRot(newPos, newRotZ);		
	}

	void Boss1SwordStuckColliderScript::SetCollider(Collider2D* collider)
	{
		if (nullptr == collider)
			return;

		mRectCollider = collider;
	}
}