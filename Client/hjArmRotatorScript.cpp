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
		, mMinDistanceX(-10.f)
		, mMaxDistanceX(35.f)
		, mMinDistanceY(10.f)
		, mMaxDistanceY(45.f)
		, mManualDistance(0.f)
		, mOffsetAngle(20.f)
		, mbInverseX(false)
		, mbUseManualDistance(false)
		, mBody(nullptr)
		, mHand(nullptr)
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

	void ArmRotatorScript::SetArmRotatorFactor(float offsetDegree, bool useManualDist, float manualDist)
	{
		SetOffsetAngle(offsetDegree);
		SetUseManualDistance(useManualDist);
		SetManualDistance(manualDist);
	}

	void ArmRotatorScript::InverseArmAxis(Axis axis)
	{
	}

	void ArmRotatorScript::RotateArm(const Vector2& targetWorldPos)
	{
		if (!mHand)
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
		float signFactor = 1.f;

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
			signFactor = -1.f;
		}

		Vector2 handNextPos;
		distanceFromCenterX = std::lerp(mMaxDistanceX, mMinDistanceX, alpha) * signFactor;

		if (!mbUseManualDistance)
		{
			distanceFromCenterY = std::lerp(mMaxDistanceY, mMinDistanceY, alpha);
		}

		handNextPos = Vector2(distanceFromCenterX, distanceFromCenterY);

		float newShoulderRot = 0.f;
		newShoulderRot = DegreeToRadian(angle);
		handNextPos.Rotate(angle);

		mHand->SetPositionXY(handNextPos);
		GetOwner()->SetRotationZ(newShoulderRot);
	}
}