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
		, mMinDistanceX(20.f)
		, mMaxDistanceX(0.f)
		, mMinDistanceY(0.f)
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

		float handWorldZ = mGrappedObject->GetWorldPositionZ();
		float handLocalZ = mGrappedObject->GetPositionZ();
		float weaponWorldPosZ = 0.f;
		if (dynamic_cast<PlayerHand*>(mGrappedObject))
		{
			Transform* weaponTR = dynamic_cast<PlayerHand*>(mGrappedObject)->GetWeaponTR();
			weaponWorldPosZ = weaponTR->GetWorldPositionZ();

			bool isBodyFlip = dynamic_cast<Player*>(mBody)->IsFlip();

			if (isBodyFlip)
			{
				if (0.95f <= handWorldZ && handWorldZ <= 2.05f)
				{
					mGrappedObject->SetPositionZ(-1.f);
					weaponTR->SetPositionZ(-1.f);
				}
				else
				{
					mGrappedObject->SetPositionZ(-2.f);
					weaponTR->SetPositionZ(1.f);
				}
			}
			else
			{
				if (0.95f <= handWorldZ && handWorldZ <= 2.05f)
				{
					mGrappedObject->SetPositionZ(1.f);
					weaponTR->SetPositionZ(1.f);
				}
				else
				{
					mGrappedObject->SetPositionZ(2.f);
					weaponTR->SetPositionZ(-1.f);
				}
			}
		}
	}

	void ArmRotatorScript::Render()
	{
		Script::Render();
	}

	void ArmRotatorScript::InverseArmAxis(Axis axis)
	{
		Vector3 ownerRot = GetOwner()->GetRotation();
		switch (axis)
		{
		case hj::Axis::X:
			ownerRot.x = ownerRot.x + PI;
			if (ownerRot.x >= 2 * PI && ownerRot.x < 0.f)
			{
				ownerRot.x = 0.f;
			}

			mbInverseX = !mbInverseX;
			if (dynamic_cast<PlayerHand*>(mGrappedObject))
			{
				dynamic_cast<PlayerHand*>(mGrappedObject)->InverseHandPosZ(mbInverseX);
			}
			if (mbInverseX)
			{
				mOffsetAngle = 45.f;
			}
			else
			{
				mOffsetAngle = 20.f;
			}
			break;
		case hj::Axis::Y:
			ownerRot.y = ownerRot.y + PI;
			break;
		case hj::Axis::Z:
			ownerRot.z = ownerRot.z + PI;
			break;
		case hj::Axis::End:
		default:
			return;
		}
		GetOwner()->SetRotation(ownerRot);
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
#define EQUIANGULAR_ANGLE 120.f
#define RIGHT_ANGLE 90.f
#define ANGLE_CORRECTION 60.f

		if (mbInverseX)
		{
			if (dir.x > 0.f)
			{
				angle = (cross.z < 0.f) ? -angle - mOffsetAngle : angle - mOffsetAngle;
			}
			else
			{
				angle = (cross.z < 0.f) ? angle - STRAIGHT_ANGLE - mOffsetAngle : -angle + STRAIGHT_ANGLE - mOffsetAngle;
			}
		}
		else
		{
			if (dir.x > 0.f)
			{
				angle = (cross.z < 0.f) ? -angle + mOffsetAngle : angle + mOffsetAngle;
			}
			else
			{
				angle = (cross.z < 0.f) ? angle - STRAIGHT_ANGLE + mOffsetAngle : -angle + STRAIGHT_ANGLE + mOffsetAngle;
			}
		}

		float distanceFromCenterX = std::lerp(mMinDistanceX, mMaxDistanceX, (angle + ANGLE_CORRECTION) / STRAIGHT_ANGLE);
		float distanceFromCenterY = std::lerp(mMinDistanceY, mMaxDistanceY, (angle + ANGLE_CORRECTION) / STRAIGHT_ANGLE);
		
		mGrappedObject->SetPositionXY(Vector2(distanceFromCenterX, distanceFromCenterY));
		GetOwner()->SetRotationZ(DegreeToRadian(angle));
	}
}