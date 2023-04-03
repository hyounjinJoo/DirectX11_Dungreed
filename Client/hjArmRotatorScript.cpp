#include "hjArmRotatorScript.h"
#include "hjMath.h"
#include "hjInput.h"
#include "hjPlayerHand.h"
#include "hjAnimator.h"

namespace hj
{
	ArmRotatorScript::ArmRotatorScript()
		: mbUsingMouseRotation(false)
		, mMinDistanceX(20.f)
		, mMaxDistanceX(0.f)
		, mMinDistanceY(0.f)
		, mMaxDistanceY(30.f)
		, mOffsetAngle(20.f)
	{
		SetName(WIDE("ArmRotatorScript"));
	}

	ArmRotatorScript::~ArmRotatorScript()
	{
	}

	void ArmRotatorScript::Initialize()
	{
		Script::Initialize();
	}

	void ArmRotatorScript::Update()
	{
		Script::Update();
	}

	void ArmRotatorScript::FixedUpdate()
	{
		Script::FixedUpdate();

		if (mbUsingMouseRotation)
		{
			RotateArm(Input::GetMouseWorldPosition());
		}
	}

	void ArmRotatorScript::Render()
	{
		Script::Render();
	}

	void ArmRotatorScript::RotateArm(const Vector2& targetWorldPos)
	{
		if (!mGrappedObject)
			return;

		Vector3 armPos = GetOwner()->GetWorldPosition();
		Vector3 bodyPos = mBody->GetWorldPosition();

		Vector3 dir = Vector3(targetWorldPos.x, targetWorldPos.y, 0.f) - bodyPos;
		dir.z = 0.f;
		dir.Normalize();

		float dot = Vector3::Right.Dot(dir);
		float angle = RadianToDegree(acosf(dot));
		Vector3 cross = Vector3::Right.Cross(dir);

#define STRAIGHT_ANGLE 180.f
#define RIGHT_ANGLE 90.f
#define ANGLE_CORRECTION 60.f

		if (dir.x > 0.f)
		{
			angle = (cross.z < 0.f) ? -angle + mOffsetAngle : angle + mOffsetAngle;
		}
		else
		{
			angle = (cross.z < 0.f) ? angle - STRAIGHT_ANGLE + mOffsetAngle : -angle + STRAIGHT_ANGLE + mOffsetAngle;
		}

		float distanceFromCenterX = std::lerp(mMinDistanceX, mMaxDistanceX, (angle + ANGLE_CORRECTION) / STRAIGHT_ANGLE);
		float distanceFromCenterY = std::lerp(mMinDistanceY, mMaxDistanceY, (angle + ANGLE_CORRECTION) / STRAIGHT_ANGLE);
		
		mGrappedObject->SetPositionXY(Vector2(distanceFromCenterX, distanceFromCenterY));
		GetOwner()->SetRotationZ(DegreeToRadian(angle));
	}
}