#pragma once
#include <hjScript.h>
namespace hj
{
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

		void SetTarget(class GameObject* target) { mGrappedObject = target; }

		void SetUsingMouseRotation(bool use) { mbUsingMouseRotation = use; }
		void SetOffsetAngle(bool degree) { mOffsetAngle = degree; }

	private:
		void RotateArm(const Vector2& targetPos);

	private:
		class GameObject* mGrappedObject;
		bool mbUsingMouseRotation;

		float mOffsetAngle;
		float mMinDistance;
		float mMaxDistance;
	};
}
