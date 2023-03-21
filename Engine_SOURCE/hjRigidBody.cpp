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
	{
		mLimitVelocity.x = 400.f;
		mLimitVelocity.y = 1000.f;
		mbGround = false;
		mGravity = Vector2(0.0f, 2000.0f);
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

		// �ӵ��� ���ӵ��� ���Ѵ�
		if (mbHorizonAccelMove)
			mAccelation *= Time::DeltaTime();
		else
			mAccelation.y *= Time::DeltaTime();

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
			mVelocity -= mGravity * Time::DeltaTime();
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
				friction = friction * mFriction * mMass * Time::DeltaTime();

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