#include "hjArmRotatorScript.h"
#include "hjMath.h"
#include "hjInput.h"
#include "hjPlayerHand.h"

namespace hj
{
	ArmRotatorScript::ArmRotatorScript()
		: mbUsingMouseRotation(false)
		, mMinDistance(20.f)
		, mMaxDistance(40.f)
		, mOffsetAngle(50.f)
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

		if(mbUsingMouseRotation)
		{			
			RotateArm(Input::GetMouseWorldPosition());
		}
	}

	void ArmRotatorScript::FixedUpdate()
	{
		Script::FixedUpdate();
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

		Vector3 dir = Vector3(targetWorldPos.x, targetWorldPos.y, 0.f) - armPos;
		dir.z = 0.f;
		dir.Normalize();

		float dot = Vector3::Right.Dot(dir);
		float angle = RadianToDegree(acosf(dot));
		Vector3 cross = Vector3::Right.Cross(dir);

#define STRAIGHTANGLE 180.f
#define RIGHTANGLE 90.f

		float distanceFromCenter = 0.f;
		if (dir.x > 0.f)
		{
			angle = (cross.z < 0.f) ? -angle + mOffsetAngle : angle + mOffsetAngle;

			distanceFromCenter = std::lerp(mMinDistance, mMaxDistance, (angle + mMaxDistance) / STRAIGHTANGLE);
		}
		else
		{
			angle = (cross.z < 0.f) ? -angle - mOffsetAngle : angle - mOffsetAngle;

			float midPoint = (mMaxDistance - mMinDistance) * 0.5f + mMinDistance;

			if (-(STRAIGHTANGLE - mOffsetAngle) > angle && angle > -(STRAIGHTANGLE + mOffsetAngle))
			{
				distanceFromCenter = std::lerp(midPoint, mMinDistance, (angle + (STRAIGHTANGLE + mOffsetAngle)) / RIGHTANGLE);
			}
			else
			{
				distanceFromCenter = std::lerp(mMaxDistance, midPoint, (angle - (RIGHTANGLE - mOffsetAngle)) / RIGHTANGLE);
			}
		}

		mGrappedObject->SetPositionX(distanceFromCenter);
		GetOwner()->SetRotationZ(DegreeToRadian(angle));
	}
}