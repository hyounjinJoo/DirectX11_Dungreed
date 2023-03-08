#include "hjCollider.h"

namespace hj
{
	UINT32 Collider::colliderID = 0;

	Collider::Collider()
		: Component(eComponentType::Collider)
		, mType(eColliderType::None)
		, mbTrigger(false)
		, mColliderID(colliderID++)
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