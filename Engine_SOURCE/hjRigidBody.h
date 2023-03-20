#pragma once
#include "hjComponent.h"

using namespace hj::math;
namespace hj
{
	class RigidBody : public Component
	{
	public:
		RigidBody();
		virtual ~RigidBody();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void AddForce(Vector2 force);
		void SetMass(float mass) { mMass = mass; }
		void SetGround(bool isGround) { mbGround = isGround; }
		bool IsGround() { return mbGround; }
		Vector2 GetVelocity() { return mVelocity; }
		void SetVelocity(Vector2 velocity) { mVelocity = velocity; }

	private:
		// ���� �������� �̿��� �̵�
		float mMass;
		float mFriction;
		//float mMoveFriction;
		Vector2 mForce;
		Vector2 mVelocity;
		Vector2 mAccelation;

		// �߷� �̿��� ����
		Vector2 mGravity;
		bool mbGround;
		Vector2 mLimitVelocity;
	};
}