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
		, mVelocity(Vector2::Zero)
		, mForce(Vector2::Zero)
		, mAccelation(Vector2::Zero)
	{
		mLimitVelocity.x = 600.f;
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

		float fixedDelta = Time::FixedDeltaTime();
		// �ӵ��� ���ӵ��� ���Ѵ�
		if (mbHorizonAccelMove)
			mAccelation *= fixedDelta;
		else
			mAccelation.y *= fixedDelta;


		mVelocity += mAccelation;

		if (mbGround)
		{
			// ������ ������
			Vector2 gravity = mGravity;
			gravity.Normalize();
			float dot = mVelocity.Dot(gravity);
			mVelocity -= gravity * dot;
		}
		else
		{
			// ���߿� ���� ��
			// ��� ���
			mVelocity -= mGravity * fixedDelta;
		}

		// �ִ� �ӵ� ����
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

		//������ ���� ( ����� ���� ����, �ӵ��� 0 �� �ƴ� ��)
		if(mbOnFriction)
		{
			if (!(mVelocity == Vector2::Zero) && mbGround)
			{
				// �ӵ��� �ݴ� �������� �������� ����
				Vector2 friction = -mVelocity;
				friction.Normalize();
				friction = friction * mFriction * mMass * fixedDelta;

				// ���������� ���� �ӵ� ���ҷ��� ���� �ӵ����� �� ū ���
				if (mVelocity.Length() < friction.Length())
				{
					// �ӵ��� 0 �� �����.
					mVelocity = Vector2(0.f, 0.f);
				}
				else
				{
					// �ӵ����� ���������� ���� �ݴ�������� �ӵ��� �����Ѵ�.
					mVelocity += friction;
				}
			}
		}

		// �ӵ��� �°� ��ü�� �̵���Ų��.
		Vector2 pos = GetOwner()->GetPositionXY();
		Vector2 curFrameDeltaMove = mVelocity * fixedDelta;

		if (mForce.y != 0.f)
		{
			int a = 0;
		}
		pos = pos + curFrameDeltaMove;
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