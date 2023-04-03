#include "hjRigidBody.h"
#include "hjTime.h"
#include "hjComponent.h"
#include "hjGameObject.h"

namespace hj
{
	RigidBody::RigidBody()
		: Component(eComponentType::RigidBody)
		, mMass(1.0f)
		, mFriction(1000.0f)
		, mbOnFriction(false)
		, mbHorizonAccelMove(false)
		, mMoveDir(eMoveDir::End)
		, mLimitMaxJump(320.f)
		, mJumpStartPosY(0.f)
	{
		mLimitVelocity.x = 400.f;
		mLimitVelocity.y = 10000.f;
		mbGround = false;
		mGravity = Vector2(0.0f, 3000.0f);
	}
	RigidBody::~RigidBody()
	{
	}
	void RigidBody::Initialize()
	{
	}

	void RigidBody::Update()
	{
		mAccelation = mForce / mMass;

		// 속도에 가속도를 더한다
		if (mbHorizonAccelMove)
			mAccelation *= Time::DeltaTime();
		else
			mAccelation.y *= Time::DeltaTime();


		mVelocity += mAccelation;

		if (mbGround)
		{
			// 땅위에 있을때
			Vector2 gravity = mGravity;
			gravity.Normalize();
			float dot = mVelocity.Dot(gravity);
			mVelocity -= gravity * dot;
		}
		else
		{
			// 공중에 있을 때
			// 잠시 대기
			mVelocity -= mGravity * Time::DeltaTime();
		}


		// 최대 속도 제한
		Vector2 gravity = mGravity;
		gravity.Normalize();
		float dot = mVelocity.Dot(gravity);
		gravity = gravity * dot;

		Vector2 sideVelocity = mVelocity - gravity;
		if (mLimitVelocity.y < gravity.Length())
		{
			gravity.Normalize();
			gravity *= mLimitVelocity.y;
		}

		if (mLimitVelocity.x < sideVelocity.Length())
		{
			sideVelocity.Normalize();
			sideVelocity *= mLimitVelocity.x;
		}
		mVelocity = gravity + sideVelocity;

		//마찰력 조건 ( 적용된 힘이 없고, 속도가 0 이 아닐 떄)
		if(mbOnFriction)
		{
			if (!(mVelocity == Vector2::Zero) && mbGround)
			{
				// 속도에 반대 방향으로 마찰력을 적용
				Vector2 friction = -mVelocity;
				friction.Normalize();
				friction = friction * mFriction * mMass * Time::DeltaTime();

				// 마찰력으로 인한 속도 감소량이 현재 속도보다 더 큰 경우
				if (mVelocity.Length() < friction.Length())
				{
					// 속도를 0 로 만든다.
					mVelocity = Vector2(0.f, 0.f);
				}
				else
				{
					// 속도에서 마찰력으로 인한 반대방향으로 속도를 차감한다.
					mVelocity += friction;
				}
			}
		}

		// 속도에 맞게 물체를 이동시킨다.
		Vector2 pos = GetOwner()->GetPositionXY();
		if (pos.y + mVelocity.y * Time::DeltaTime() >= mJumpStartPosY + mLimitMaxJump)
			mVelocity.y = 0.f;

		pos = pos + mVelocity * Time::DeltaTime();
		GetOwner()->SetPositionXY(pos);
		mForce = Vector2::Zero;
		
		if (mVelocity.x == 0.f)
			mMoveDir = eMoveDir::End;
		else if (mVelocity.x < 0.f)
			mMoveDir = eMoveDir::Left;
		else if(mVelocity.x > 0.f)
			mMoveDir = eMoveDir::Right;

		if (!mbHorizonAccelMove)
			mVelocity.x = 0.f;
	}

	void RigidBody::FixedUpdate()
	{
	}

	void RigidBody::Render()
	{
	}

	void RigidBody::AddForce(Vector2 force)
	{
		mForce += force;
	}
}