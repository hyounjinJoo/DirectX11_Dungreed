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

		void SetBody(class GameObject* body) { mBody = body; }
		void SetTarget(class GameObject* target) { mGrappedObject = target; }

		void SetUsingMouseRotation(bool use) { mbUsingMouseRotation = use; }
		void SetOffsetAngle(bool degree) { mOffsetAngle = degree; }
	
	private:
		void RotateArm(const math::Vector2& targetPos);

	private:
		class GameObject* mBody;
		class GameObject* mGrappedObject;
		bool mbUsingMouseRotation;

		float mOffsetAngle;
		float mMinDistanceX;
		float mMaxDistanceX;
		float mMinDistanceY;
		float mMaxDistanceY;
	};
}
