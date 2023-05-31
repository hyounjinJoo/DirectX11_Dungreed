#include "hjBoss1BulletMuzzle.h"
#include "hjTime.h"
#include "hjObject.h"
#include "hjBoss1Bullet.h"

namespace hj
{
	Boss1BulletMuzzle::Boss1BulletMuzzle()
		: mNextShotBulletIndex(0)
		, mShotDelayTime(0.03f)
		, mShotDelayTimer(0.f)
		, mMuzzleState(MuzzleState::End)
		, mRotateDir(1.f)
	{
		// Bullet �� 120�� ����
		Boss1Bullet* bullet = object::Instantiate<Boss1Bullet>(eLayerType::MonsterAttack);
		bullet->GetTransform()->SetParent(this->GetTransform());
		mBullets.push_back(bullet);
		for (int iter = 1; iter < 120; ++iter)
		{
			bullet = object::Clone<Boss1Bullet>(bullet);
			bullet->GetTransform()->SetParent(this->GetTransform());
			mBullets.push_back(bullet);
		}

		ChangeMuzzleState(MuzzleState::ShotStart);
	}
	
	Boss1BulletMuzzle::~Boss1BulletMuzzle()
	{
		for (GameObject* iter : mBullets)
		{
			iter = nullptr;
		}
	}

	void Boss1BulletMuzzle::Initialize()
	{
		GameObject::Initialize();
	}

	void Boss1BulletMuzzle::Update()
	{
		if (MuzzleState::ShotStart == mMuzzleState)
		{
			CheckNeedToShot();
		}
		else if (MuzzleState::ShotEnd == mMuzzleState)
		{
			CheckLastBulletPause();
		}
		GameObject::Update();
	}
	
	void Boss1BulletMuzzle::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void Boss1BulletMuzzle::ChangeMuzzleState(MuzzleState state)
	{
		switch(state)
		{
		case MuzzleState::ShotStart:
			mMuzzleState = MuzzleState::ShotStart;
			break;
		case MuzzleState::ShotEnd:
			mMuzzleState = MuzzleState::ShotEnd;
			break;
		case MuzzleState::End:
		default:
			break;
		}
	}

	void Boss1BulletMuzzle::CheckNeedToShot()
	{
		if (mShotDelayTimer < mShotDelayTime)
		{
			mShotDelayTimer += Time::ActualDeltaTime();

			if (mShotDelayTimer > mShotDelayTime)
			{
				mShotDelayTimer = 0.f;

				Shot();
			}
		}
	}

	void Boss1BulletMuzzle::CheckLastBulletPause()
	{
		if (eState::Paused == mBullets[mNextShotBulletIndex - 1]->GetState())
		{
			EndPattern();
		}
	}

	void Boss1BulletMuzzle::Shot()
	{
		static const Vector2 initialDirection = Vector2::Down;


		// �Ҹ� ��ġ ����
		mBullets[mNextShotBulletIndex]->SetPositionXY(this->GetPositionXY());
		// �Ҹ� �߻� ���� ����
		constexpr float initialDegree_90 = XM_PI * 0.5f;
		constexpr float angleRatio = 5.f;

		int axis = mNextShotBulletIndex % 4;
		float finalAxis = RadianToDegree(initialDegree_90) * axis;
		
		int directionAngle = mNextShotBulletIndex / 4;
		float eachAddedRot = static_cast<float>(directionAngle);

		Vector2 rotatedDirection = initialDirection.Rotate(finalAxis + eachAddedRot * angleRatio);
		
		// �Ҹ� �ʱ�ȭ �� Ȱ��ȭ
		mBullets[mNextShotBulletIndex]->SetMoveDirection(rotatedDirection);
		mBullets[mNextShotBulletIndex]->Activate();

		++mNextShotBulletIndex;

		static const size_t totalBullet = mBullets.size();
		if (totalBullet <= mNextShotBulletIndex)
		{
			ChangeMuzzleState(MuzzleState::ShotEnd);
		}
	}

	void Boss1BulletMuzzle::EndPattern()
	{
		mNextShotBulletIndex = 0;
		Pause();
	}

}