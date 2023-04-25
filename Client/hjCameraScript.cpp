#include "hjCameraScript.h"
#include "hjTransform.h"
#include "hjGameObject.h"
#include "hjInput.h"
#include "hjTime.h"
#include "hjSceneManager.h"
#include "hjPlayer.h"
#include <random>

namespace hj
{
	CameraScript::CameraScript()
		: Script()
		, mbMoveLimit(true)
		, mbFollowPlayer(true)
		, mLimitLTRB(Vector4(-1600.f, 900.f, 1600.f, -900.f))
		, mRectHalfScale(Vector2(800.f, 450.f))
		, mShakeDuration(0.1f)
		, mShakeTimer(0.f)
		, mDicreaseShakeFactor(1.f)
		, mShakeMoveLimit(Vector2(10.f, 10.f))
		, mOriginPos(Vector2::Zero)
	{
	}

	CameraScript::~CameraScript()
	{
	}

	void CameraScript::Initialize()
	{
	}

	void CameraScript::Update()
	{	
		UpdateCameraPos();
		UpdateShakedCameraPos();

		if (Input::GetKeyDown(eKeyCode::N_0))
		{
			mbFollowPlayer = !mbFollowPlayer;
			mbMoveLimit = !mbMoveLimit;
		}
		if (!mbFollowPlayer)
		{
			if (Input::GetKeyPressed(eKeyCode::U))
			{
				GetOwner()->AddPositionY(300.f * Time::ActualDeltaTime());
			}
			if (Input::GetKeyPressed(eKeyCode::J))
			{
				GetOwner()->AddPositionY(-300.f * Time::ActualDeltaTime());
			}
			if (Input::GetKeyPressed(eKeyCode::H))
			{
				GetOwner()->AddPositionX(-300.f * Time::ActualDeltaTime());
			}
			if (Input::GetKeyPressed(eKeyCode::K))
			{
				GetOwner()->AddPositionX(300.f * Time::ActualDeltaTime());
			}
		}
	}

	void CameraScript::Render()
	{
		Script::Render();
	}

	GameObject* CameraScript::GetPlayer()
	{
		std::vector<GameObject*> objects = SceneManager::GetActiveScene()->GetGameObjects(eLayerType::Player);
		for (auto iter : objects)
		{
			if (dynamic_cast<Player*>(iter))
			{
				return iter;
			}
		}

		return nullptr;
	}

	void CameraScript::UpdateCameraPos()
	{
		GameObject* player = GetPlayer();
		if (!player)
			return;

		Vector2 nextCameraPos = GetOwner()->GetPositionXY();
		
		if (mbFollowPlayer)
			nextCameraPos = player->GetPositionXY();

		if (mbMoveLimit)
		{
			// 카메라의 현 위치 + 반절크기만큼 더 확장한 것이 좌,상,우,하의 제한을 넘어가는지 확인하면 된다.
			Vector2 cameraPos = nextCameraPos;
			Vector2 correctionValue = Vector2::Zero;

#define CAMERA_LIMIT_L mLimitLTRB.x
#define CAMERA_LIMIT_T mLimitLTRB.y
#define CAMERA_LIMIT_R mLimitLTRB.z
#define CAMERA_LIMIT_B mLimitLTRB.w

			if (cameraPos.x - mRectHalfScale.x < CAMERA_LIMIT_L)
			{
				correctionValue.x = CAMERA_LIMIT_L - (cameraPos.x - mRectHalfScale.x);
			}

			if (cameraPos.y + mRectHalfScale.y > CAMERA_LIMIT_T)
			{
				correctionValue.y = -((cameraPos.y + mRectHalfScale.y) - CAMERA_LIMIT_T);
			}

			if (cameraPos.x + mRectHalfScale.x > CAMERA_LIMIT_R)
			{
				correctionValue.x = -((cameraPos.x + mRectHalfScale.x) - CAMERA_LIMIT_R);
			}

			if (cameraPos.y - mRectHalfScale.y < CAMERA_LIMIT_B)
			{
				correctionValue.y = CAMERA_LIMIT_B - (cameraPos.y - mRectHalfScale.y);
			}

			nextCameraPos += correctionValue;
		}


		GetOwner()->SetPositionXY(nextCameraPos);
		mOriginPos = nextCameraPos;
	}

	void CameraScript::UpdateShakedCameraPos()
	{
		if (mShakeTimer <= 0.f)
			return;

		mShakeTimer -= Time::ActualDeltaTime() * mDicreaseShakeFactor;

		Vector2 shakeMove = CalcRandomShakeValue();
		Vector2 shakedPos = mOriginPos + shakeMove;
		GetOwner()->SetPositionXY(shakedPos);

		if (mShakeTimer <= 0.f)
		{
			mShakeTimer = 0.f;
			GetOwner()->SetPositionXY(mOriginPos);
		}
	}

	void CameraScript::Shake()
	{
		mShakeTimer = mShakeDuration;
	}

	Vector2 CameraScript::CalcRandomShakeValue()
	{
		auto curTime = std::chrono::system_clock::now();
		auto duration = curTime.time_since_epoch();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
		std::mt19937 randomSeed(static_cast<UINT>(millis));

		std::uniform_real_distribution<float> valueRangeX(-mShakeMoveLimit.x, mShakeMoveLimit.x + 0.1f);
		std::uniform_real_distribution<float> valueRangeY(-mShakeMoveLimit.y, mShakeMoveLimit.y + 0.1f);

		Vector2 randomShakeValue = Vector2::Zero;
		randomShakeValue = Vector2(valueRangeX(randomSeed), valueRangeY(randomSeed));

		return randomShakeValue;
	}
}