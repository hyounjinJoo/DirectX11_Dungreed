#include "hjArmRotatorScript.h"
#include "hjMath.h"
#include "hjInput.h"
#include "hjPlayerHand.h"
#include "hjAnimator.h"
#include "hjPlayer.h"

namespace hj
{
	ArmRotatorScript::ArmRotatorScript()
		: mbUsingMouseRotation(false)
		, mMinDistanceX(-20.f)
		, mMaxDistanceX(50.f)
		, mMinDistanceY(50.f)
		, mMaxDistanceY(30.f)
		, mOffsetAngle(20.f)
		, mbInverseX(false)
		, mBody(nullptr)
		, mGrappedObject(nullptr)
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

	void ArmRotatorScript::InverseArmAxis(Axis axis)
	{
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

		float distanceFromCenterX = 0.f;
		float distanceFromCenterY = 0.f;

		angle = (cross.z < 0.f) ? 360.f - angle : angle;

		float alpha = 0.f;
		float newRot = 0.f;
		if ((angle >= 0.0f && angle <= 90.0f) || (angle >= 270.0f))
		{
			if (angle <= 90.f)
			{
				alpha = angle / 180.f + 0.5f;
				angle += mOffsetAngle;
			}
			else
			{
				alpha = (angle - 270.f) / 180.f;
				angle += mOffsetAngle;
			}
			distanceFromCenterX = std::lerp(mMaxDistanceX, mMinDistanceX, alpha);
			distanceFromCenterY = std::lerp(mMaxDistanceY, mMinDistanceY, alpha);

			newRot = DegreeToRadian(angle);
		}
		else if ((angle > 90.f))
		{
			if (angle <= 180.f)
			{
				alpha = (180.f - angle) / 180.f + 0.5f;
				angle = 180.f -angle + mOffsetAngle;
			}
			else
			{
				alpha = (270.f - angle) / 180.f;
				angle = 540.f - angle + mOffsetAngle;
			}
			distanceFromCenterX = -std::lerp(mMaxDistanceX, mMinDistanceX, alpha);
			distanceFromCenterY = std::lerp(mMaxDistanceY, mMinDistanceY, alpha);
			newRot = DegreeToRadian(angle);
		}
		
		Vector2 newPos = Vector2(distanceFromCenterX, distanceFromCenterY);
		newPos.Rotate(angle);

		mGrappedObject->SetPosition(newPos);
		GetOwner()->SetRotationZ(newRot);
	}
}