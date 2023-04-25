#include "hjRoomRectFloor.h"
#include "hjCollider2D.h"
#include "hjRigidBody.h"

namespace hj
{
	RoomRectFloor::RoomRectFloor()
		: mScript(nullptr)
	{
		mScript = AddComponent<FloorScript>();
		Collider2D* colliderFloor = AddComponent<Collider2D>();
		mScript->SetCollider(colliderFloor);
	}

	RoomRectFloor::~RoomRectFloor()
	{
	}

	void RoomRectFloor::Initialize()
	{
		GameObject::Initialize();
	}
	
	void RoomRectFloor::Update()
	{
		GameObject::Update();
	}
	
	void RoomRectFloor::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}
	
	void RoomRectFloor::Render()
	{
		GameObject::Render();
	}
}

namespace hj
{
	FloorScript::FloorScript()
		: mRectCollider(nullptr)
	{
	}

	FloorScript::~FloorScript()
	{
	}

	void FloorScript::Initialize()
	{
	}
	void FloorScript::Update()
	{
	}
	void FloorScript::FixedUpdate()
	{
	}
	void FloorScript::Render()
	{
	}

	void FloorScript::OnCollisionEnter(Collider* collider)
	{
		if (nullptr == collider->GetOwner())
			return;

		GameObject* colliderOwner = collider->GetOwner();
		
		RigidBody* colliderRigidBody = colliderOwner->GetComponent<RigidBody>();
		if (colliderRigidBody)
		{
			colliderRigidBody->SetGround(true);
		}
	}

	void FloorScript::OnCollisionStay(Collider* collider)
	{
		if (nullptr == collider->GetOwner())
			return;

		float floorHeight = mRectCollider->GetPosition().y + GetOwner()->GetScaleY() * 0.5f;

		float actorHeightHalf = collider->GetOwner()->GetScaleY() * 0.5f + 0.1f;
		collider->GetOwner()->SetPositionY(floorHeight + actorHeightHalf);
	}

	void FloorScript::SetCollider(class Collider2D* collider)
	{
		if (collider)
			mRectCollider = collider;
	}

}