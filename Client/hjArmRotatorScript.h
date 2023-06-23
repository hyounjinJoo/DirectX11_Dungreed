#pragma once
#include <hjScript.h>
namespace hj
{
	enum class Axis
	{
		X,
		Y,
		Z,
		End,
	};

	class ArmRotatorScript :
		public Script
	{
	public:
		ArmRotatorScript();
		virtual ~ArmRotatorScript();

		void Initialize() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() override;

	public:
		void SetBody(class GameObject* body) { mBody = body; }
		void SetTarget(class Hand* target) { mHand = target; }

		void SetUsingMouseRotation(bool use) { mbUsingMouseRotation = use; }
		void SetUsingManualRotation(bool use) { mbUsingManualRotation = use; }
		void SetOffsetAngle(float degree) { mOffsetAngle = degree; }
		void SetUseManualDistance(bool use) { mbUseManualDistance = use; }
		void SetManualDistance(float dist) { mManualDistance = dist; }
		void SetMinDistance(const Vector2& vector) { mMinDistanceX = vector.x; mMinDistanceY = vector.y; }
		void SetMaxDistance(const Vector2& vector) { mMaxDistanceX = vector.x; mMaxDistanceY = vector.y; }
		void SetDistanceInfo(const Vector2& minDistance, const Vector2& maxDistance)
		{
			mMinDistanceX = minDistance.x; mMinDistanceY = minDistance.y;
			mMaxDistanceX = maxDistance.x; mMaxDistanceY = maxDistance.y;
		}
		void SetUseOriginAngle(bool use) { mbUseOriginAngle = use; }
		void SetNotAllowMinusHandPosX(bool notAllow) { mbNotAllowMinusHandPosX = notAllow; }

		void SetArmRotatorFactor(float offsetDegree, bool useManualDist, float manualDist, bool useOriginAngle, bool notAllowMinusHandPosX);

		void InverseArmAxis(Axis axis);
		void SetManualTargetPosition(const Vector2& position) { mManualTargetPosition = position; }

	public:
		float GetOffsetAngle() { return mOffsetAngle; }

	private:
		void RotateArm(const math::Vector2& targetPos);

	private:
		class GameObject* mBody;
		class Hand* mHand;
		bool mbUsingMouseRotation;
		bool mbUsingManualRotation;
		Vector2 mManualTargetPosition;
		bool mbInverseX;
		bool mbUseManualDistance;
		bool mbUseOriginAngle;
		bool mbNotAllowMinusHandPosX;

		float mManualDistance;
		float mOffsetAngle;
		float mMinDistanceX;
		float mMaxDistanceX;
		float mMinDistanceY;
		float mMaxDistanceY;
	};
}
