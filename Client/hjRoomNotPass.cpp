#include "hjRoomNotPass.h"
#include "hjCollider2D.h"
#include "hjRigidBody.h"

namespace hj
{
	RoomNotPass::RoomNotPass()
	{
		mScript = AddComponent<NotPassScript>();
		mScript->SetCollider(AddComponent<Collider2D>());
	}

	RoomNotPass::~RoomNotPass()
	{
	}

	void RoomNotPass::Initialize()
	{
		GameObject::Initialize();
	}

	void RoomNotPass::Update()
	{
		GameObject::Update();
	}

	void RoomNotPass::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void RoomNotPass::Render()
	{
		GameObject::Render();
	}
}

namespace hj
{
	NotPassScript::NotPassScript()
		: mRectCollider(nullptr)
	{
	}

	NotPassScript::~NotPassScript()
	{
	}

	void NotPassScript::Initialize()
	{
	}

	void NotPassScript::Update()
	{
	}

	void NotPassScript::FixedUpdate()
	{
	}

	void NotPassScript::Render()
	{
	}

#define NOT_DETERMINE_CASE		0
#define LEFT_COLLISION_CASE		1
#define RIGHT_COLLISION_CASE	2
#define BOTTOM_COLLISION_CASE	3
#define TOP_COLLISION_CASE		4
	void NotPassScript::OnCollisionEnter(Collider* collider)
	{
		if (nullptr == mRectCollider || nullptr == collider->GetOwner())
			return;

		GameObject* other = collider->GetOwner();
		if (nullptr == other->GetComponent<RigidBody>())
			return;

		RigidBody* otherRigidBody = other->GetComponent<RigidBody>();
		Vector2 otherCenter = other->GetWorldPositionXY();

		Vector4 thisLTRB = mRectCollider->GetLTRB();
		Vector2 thisCenter = Vector2(mRectCollider->GetPosition().x, mRectCollider->GetPosition().y);

		Vector2 otherVelocity = otherRigidBody->GetVelocity();
		Vector2 otherForce = otherRigidBody->GetForce();

		UINT enterCase = NOT_DETERMINE_CASE;
		if (otherCenter.y > thisLTRB.y)
			enterCase = TOP_COLLISION_CASE;
		else if (otherCenter.x < thisLTRB.x)
			enterCase = LEFT_COLLISION_CASE;
		else if (otherCenter.x > thisLTRB.z)
			enterCase = RIGHT_COLLISION_CASE;
		else if (otherCenter.y < thisLTRB.w)
			enterCase = BOTTOM_COLLISION_CASE;
		else
		{
			if (otherCenter.y >= thisCenter.y || (otherVelocity.y < 0.f || otherForce.y < 0.f))
				enterCase = TOP_COLLISION_CASE;
			else if (otherCenter.x <= thisCenter.x || (otherVelocity.x > 0.f || otherForce.x > 0.f))
				enterCase = LEFT_COLLISION_CASE;
			else if (otherCenter.x > thisCenter.x || (otherVelocity.x < 0.f || otherForce.x < 0.f))
				enterCase = RIGHT_COLLISION_CASE;
			else if (otherCenter.y < thisCenter.y || (otherVelocity.y >= 0.f || otherForce.y >= 0.f))
				enterCase = BOTTOM_COLLISION_CASE;
		}

		Vector2 otherScale = dynamic_cast<Collider2D*>(collider)->GetSize() * other->GetScaleXY();
		Vector4 otherLTRB = other->GetWorldLTRB();

		if (TOP_COLLISION_CASE == enterCase)
		{
			otherRigidBody->SetGround(true);
			otherRigidBody->ClearVelocityY();

			other->SetPositionY(thisLTRB.y + otherScale.y * 0.5f);
		}
		else if (BOTTOM_COLLISION_CASE == enterCase)
		{
			otherRigidBody->ClearVelocityY();
			other->SetPositionY(thisLTRB.w - otherScale.y * 0.5f);
		}
		else if (LEFT_COLLISION_CASE == enterCase)
		{
			otherRigidBody->ClearVelocityX();
			other->SetPositionX(thisLTRB.x - otherScale.x * 0.5f);
		}
		else if (RIGHT_COLLISION_CASE == enterCase)
		{
			otherRigidBody->ClearVelocityX();
			other->SetPositionX(thisLTRB.z + otherScale.x * 0.5f);
		}
	}

	void NotPassScript::OnCollisionStay(Collider* collider)
	{
		if (nullptr == mRectCollider || nullptr == collider->GetOwner())
			return;

		GameObject* other = collider->GetOwner();
		if (nullptr == other->GetComponent<RigidBody>())
			return;

		RigidBody* otherRigidBody = other->GetComponent<RigidBody>();
		Vector2 otherCenter = other->GetWorldPositionXY();

		Vector4 thisLTRB = mRectCollider->GetLTRB();
		Vector2 thisCenter = Vector2(mRectCollider->GetPosition().x, mRectCollider->GetPosition().y);

		Vector2 otherVelocity = otherRigidBody->GetVelocity();
		Vector2 otherForce = otherRigidBody->GetForce();

		UINT enterCase = NOT_DETERMINE_CASE;

		if (otherCenter.y > thisLTRB.y)
			enterCase = TOP_COLLISION_CASE;
		else if (otherCenter.x < thisLTRB.x)
			enterCase = LEFT_COLLISION_CASE;
		else if (otherCenter.x > thisLTRB.z)
			enterCase = RIGHT_COLLISION_CASE;
		else if (otherCenter.y < thisLTRB.w)
			enterCase = BOTTOM_COLLISION_CASE;
		else
		{
			if (otherCenter.y >= thisCenter.y || (otherVelocity.y < 0.f || otherForce.y < 0.f))
				enterCase = TOP_COLLISION_CASE;
			else if (otherCenter.x <= thisCenter.x || (otherVelocity.x > 0.f || otherForce.x > 0.f))
				enterCase = LEFT_COLLISION_CASE;
			else if (otherCenter.x > thisCenter.x || (otherVelocity.x < 0.f || otherForce.x < 0.f))
				enterCase = RIGHT_COLLISION_CASE;
			else if (otherCenter.y < thisCenter.y || (otherVelocity.y >= 0.f || otherForce.y >= 0.f))
				enterCase = BOTTOM_COLLISION_CASE;
		}

		Vector2 otherScale = dynamic_cast<Collider2D*>(collider)->GetSize() * other->GetScaleXY();
		Vector4 otherLTRB = other->GetWorldLTRB();

		if (TOP_COLLISION_CASE == enterCase)
		{
			float forceY = otherRigidBody->GetForce().y;
			if (forceY <= 0.f && otherRigidBody->GetVelocity().y < 0.f)
			{
				otherRigidBody->SetGround(true);
				other->SetPositionY(thisLTRB.y + otherScale.y * 0.5f);
			}
		}
		else if (BOTTOM_COLLISION_CASE == enterCase)
		{
			if (otherRigidBody->GetVelocity().y >= 0.f)
				other->SetPositionY(thisLTRB.w - otherScale.y * 0.5f);
		}
		else if (LEFT_COLLISION_CASE == enterCase)
		{
			if (otherRigidBody->GetForce().x >= 0.f)
				other->SetPositionX(thisLTRB.x - otherScale.x * 0.5f);
		}
		else if (RIGHT_COLLISION_CASE == enterCase)
		{
			if (otherRigidBody->GetForce().x <= 0.f)
				other->SetPositionX(thisLTRB.z + otherScale.x * 0.5f);
		}
	}

	void NotPassScript::OnCollisionExit(Collider* collider)
	{

		if (nullptr == mRectCollider || nullptr == collider->GetOwner())
			return;

		GameObject* other = collider->GetOwner();
		if (nullptr == other->GetComponent<RigidBody>())
			return;

		RigidBody* otherRigidBody = other->GetComponent<RigidBody>();
		Vector2 otherCenter = other->GetWorldPositionXY();

		Vector4 thisLTRB = mRectCollider->GetLTRB();
		Vector2 thisCenter = Vector2(mRectCollider->GetPosition().x, mRectCollider->GetPosition().y);

		Vector2 otherVelocity = otherRigidBody->GetVelocity();
		Vector2 otherForce = otherRigidBody->GetForce();

		UINT enterCase = NOT_DETERMINE_CASE;

		if (otherCenter.y > thisLTRB.y)
			enterCase = TOP_COLLISION_CASE;
		else
		{
			if (otherCenter.y >= thisCenter.y || (otherVelocity.y < 0.f || otherForce.y < 0.f))
				enterCase = TOP_COLLISION_CASE;
		}

		otherRigidBody->SetGround(false);
	}

	void NotPassScript::SetCollider(Collider2D* collider)
	{
		if (collider)
			mRectCollider = collider;
	}
}