#include "pch.h"
#include "hjCollider.h"

namespace hj
{
	UINT32 Collider::colliderID = 1;

	Collider::Collider()
		: Component(eComponentType::Collider)
		, mColliderType(eColliderType::None)
		, mbTrigger(false)
		, mbMouseTrigger(false)
		, mbUseMouseCollision(false)
		, mColliderID(colliderID++)
		, mState(eCollisionState::CollisionNot)
	{
	}

	Collider::Collider(const Collider& collider2D)
		: Component(eComponentType::Collider)
		, mColliderType(collider2D.mColliderType)
		, mbTrigger(false)
		, mbMouseTrigger(false)
		, mbUseMouseCollision(false)
		, mColliderID(colliderID++)
		, mState(eCollisionState::CollisionNot)
	{

	}

	Collider::~Collider()
	{
	}

	void Collider::Initialize()
	{
	}

	void Collider::Update()
	{
	}

	void Collider::FixedUpdate()
	{
	}

	void Collider::Render()
	{
	}

}