#include "hjRoomPassThrough.h"
#include "hjCollider2D.h"
#include "hjRigidBody.h"
#include "hjPlayer.h"
#include "hjPlayerScript.h"

namespace hj
{
	RoomPassThrough::RoomPassThrough()
	{
		mScript = AddComponent<PassThroughScript>();
		mScript->SetCollider(AddComponent<Collider2D>());
	}

	RoomPassThrough::~RoomPassThrough()
	{
	}

	void RoomPassThrough::Initialize()
	{
		GameObject::Initialize();
	}

	void RoomPassThrough::Update()
	{
		GameObject::Update();
	}

	void RoomPassThrough::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void RoomPassThrough::Render()
	{
		GameObject::Render();
	}
}

namespace hj
{
	PassThroughScript::PassThroughScript()
		: mRectCollider(nullptr)
	{
	}

	PassThroughScript::~PassThroughScript()
	{
	}

	void PassThroughScript::Initialize()
	{
	}

	void PassThroughScript::Update()
	{
	}

	void PassThroughScript::FixedUpdate()
	{
	}

	void PassThroughScript::Render()
	{
	}

#define NOT_DETERMINE_CASE		0
#define LEFT_COLLISION_CASE		1
#define RIGHT_COLLISION_CASE	2
#define BOTTOM_COLLISION_CASE	3
#define TOP_COLLISION_CASE		4
	void PassThroughScript::OnCollisionEnter(Collider* collider)
	{
		if (nullptr == mRectCollider || nullptr == collider->GetOwner())
			return;

		GameObject* other = collider->GetOwner();
		if (nullptr == other->GetComponent<RigidBody>())
			return;

		if (dynamic_cast<Player*>(other))
		{
			if ((dynamic_cast<Player*>(other))->GetPlayerScript()->IsDash())
				return;
		}

		RigidBody* otherRigidBody = other->GetComponent<RigidBody>();
		Vector2 otherBottom = other->GetWorldCenterBottom();

		Vector4 thisLTRB = mRectCollider->GetLTRB();
		Vector2 thisTop = Vector2(mRectCollider->GetPosition().x, thisLTRB.y);

		Vector2 otherVelocity = otherRigidBody->GetVelocity();
		Vector2 otherForce = otherRigidBody->GetForce();

		UINT enterCase = NOT_DETERMINE_CASE;
		if (otherBottom.y > thisLTRB.y - 12.8f)
			enterCase = TOP_COLLISION_CASE;
		else
		{
			if (otherBottom.y >= thisLTRB.y)
				enterCase = TOP_COLLISION_CASE;
		}

		Vector2 otherScale = dynamic_cast<Collider2D*>(collider)->GetSize() * other->GetScaleXY();
		Vector4 otherLTRB = other->GetWorldLTRB();

		if (TOP_COLLISION_CASE == enterCase)
		{
			otherRigidBody->SetGround(true);
			otherRigidBody->SetPathThroughFloor(true);
			otherRigidBody->ClearVelocityY();

			other->SetPositionY(thisLTRB.y + otherScale.y * 0.5f);
		}
	}

	void PassThroughScript::OnCollisionStay(Collider* collider)
	{
		if (nullptr == mRectCollider || nullptr == collider->GetOwner())
			return;

		GameObject* other = collider->GetOwner();
		if (nullptr == other->GetComponent<RigidBody>())
			return;

		if (dynamic_cast<Player*>(other))
		{
			if ((dynamic_cast<Player*>(other))->GetPlayerScript()->IsDash())
				return;
		}

		RigidBody* otherRigidBody = other->GetComponent<RigidBody>();
		Vector2 otherBottom = other->GetWorldCenterBottom();

		Vector4 thisLTRB = mRectCollider->GetLTRB();
		Vector2 thisTop = Vector2(mRectCollider->GetPosition().x, thisLTRB.y);

		Vector2 otherVelocity = otherRigidBody->GetVelocity();
		Vector2 otherForce = otherRigidBody->GetForce();

		UINT enterCase = NOT_DETERMINE_CASE;
		if (otherBottom.y > thisLTRB.y - 12.8f)
			enterCase = TOP_COLLISION_CASE;
		else
		{
			if (otherBottom.y >= thisTop.y)
				enterCase = TOP_COLLISION_CASE;
		}

		Vector2 otherScale = dynamic_cast<Collider2D*>(collider)->GetSize() * other->GetScaleXY();
		Vector4 otherLTRB = other->GetWorldLTRB();

		if (TOP_COLLISION_CASE == enterCase)
		{
			float forceY = otherRigidBody->GetForce().y;
			if (forceY <= 0.f && otherRigidBody->GetVelocity().y < 0.f)
			{
				otherRigidBody->SetGround(true);
				otherRigidBody->SetPathThroughFloor(true);
				other->SetPositionY(thisLTRB.y + otherScale.y * 0.5f);
			}
		}
	}

	void PassThroughScript::OnCollisionExit(Collider* collider)
	{
		if (nullptr == mRectCollider || nullptr == collider->GetOwner())
			return;

		GameObject* other = collider->GetOwner();
		if (nullptr == other->GetComponent<RigidBody>())
			return;

		RigidBody* otherRigidBody = other->GetComponent<RigidBody>();
		otherRigidBody->SetGround(false);
		otherRigidBody->SetPathThroughFloor(false);
	}

	void PassThroughScript::SetCollider(Collider2D* collider)
	{
		if (collider)
			mRectCollider = collider;
	}
}