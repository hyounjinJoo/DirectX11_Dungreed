#include "hjRoomDoor.h"
#include "hjCollider2D.h"
#include "hjFadeScript.h"
#include "hjTime.h"
#include "hjPlayer.h"
#include "hjDoorScript.h"
#include "hjRoomBase.h"
#include "hjPlayerScript.h"

namespace hj
{
	RoomDoor::RoomDoor()
		: mExitDoor(nullptr)
		, mFadeScript(nullptr)
		, mDoorCollider(nullptr)
		, mOwnerRoom(nullptr)
		, mTargetPlayer(nullptr)
		, mDoorPlaced(DoorPlaced::End)
		, mbEntryStart(false)
		, mbExitStart(false)
		, mbFadeInComplete(false)
		, mbFadeOutComplete(false)
	{
		mDoorCollider = AddComponent<Collider2D>();
		AddComponent<DoorScript>();

		Vector2 scale = Vector2(40.f, 320.f);
		SetScaleXY(scale);
	}

	RoomDoor::~RoomDoor()
	{
		mFadeScript = nullptr;
		mDoorCollider = nullptr;
		mExitDoor = nullptr;
		mOwnerRoom = nullptr;
		mTargetPlayer = nullptr;
	}

	void RoomDoor::Initialize()
	{
		int a = 0;
	}

	void RoomDoor::Update()
	{
		GameObject::Update();

		if (DoorPlaced::End == mDoorPlaced)
			return;

		if (mbEntryStart)
		{
			if (!mbFadeOutComplete)
				CheckFadeOutComplete();

			if (mbFadeOutComplete)
			{
				FadeInStart();
			}
		}
		if (mbExitStart)
		{
			if (!mbFadeInComplete)
				CheckFadeInComplete();

			if (mbFadeInComplete)
			{
				ProcessFadeInEnd();
			}
		}
	}

	void RoomDoor::FixedUpdate()
	{
		GameObject::FixedUpdate();

		if (DoorPlaced::End == mDoorPlaced)
			return;
	}

	void RoomDoor::Render()
	{
		GameObject::Render();

		if (DoorPlaced::End == mDoorPlaced)
			return;
	}

	void RoomDoor::SetDoorPlaced(DoorPlaced place)
	{
		mDoorPlaced = place;

		if (DoorPlaced::T == place || DoorPlaced::B == place)
		{
			Vector2 scale = Vector2(320.f, 40.f);
			SetScaleXY(scale);
		}
	}

	void RoomDoor::SetFadeScript(class FadeScript* fadeScript)
	{
		if (fadeScript)
			mFadeScript = fadeScript;
	}

	void RoomDoor::FadeOutStart(class Player* player)
	{
		if (!mFadeScript || !player || !mExitDoor || !mOwnerRoom || (nullptr == mExitDoor->GetOwnerRoom()))
			return;

		mbEntryStart = true;
		mbFadeOutComplete = false;

		//FadeOut Start
		mFadeScript->FadeOutStart();
		
		// DeltaTime Update Stop
		Time::StopDeltaTimeUpdate(true);

		mTargetPlayer = player;
	}

	void RoomDoor::FadeInStart()
	{
		if (!mFadeScript)
			return;

		mbEntryStart = false;
		mbFadeInComplete = false;
		mExitDoor->SetExitStart(true);
		mFadeScript->FadeInStart();
		Time::StopDeltaTimeUpdate(false);

		mOwnerRoom->Pause();
		mExitDoor->GetOwnerRoom()->Activate();
	}

	void RoomDoor::CheckFadeOutComplete()
	{
		if (!mFadeScript || !mExitDoor || !mOwnerRoom || (nullptr == mExitDoor->GetOwnerRoom()) || !mTargetPlayer)
			return;

		eFadeState fadeState = mFadeScript->GetFadeState();
		if (eFadeState::FadeOutEnd == fadeState)
		{
			mbFadeOutComplete = true;
			// Move Process Start
			// 1. GetExitDoor's exit pos
			DoorPlaced exitDoorPlace = mExitDoor->GetDoorPlacedType();
			// 2. player pos setted verified exit pos

			Vector2 thisDoorCenterBottom = this->GetWorldCenterBottom();
			Vector2 playerCenterBottom = mTargetPlayer->GetWorldCenterBottom();

			Vector4 exitDoorLTRB = mExitDoor->GetWorldLTRB();
#define OFFSET 1.f
#define LEFT exitDoorLTRB.x
#define TOP exitDoorLTRB.y
#define RIGHT exitDoorLTRB.z
#define BOTTOM exitDoorLTRB.w

			Vector2 exitPos = Vector2::Zero;
			float enteringHeight = std::fabsf(thisDoorCenterBottom.y) - std::fabsf(playerCenterBottom.y);
			float enteringPos = thisDoorCenterBottom.x - playerCenterBottom.x;

			switch (exitDoorPlace)
			{
			case DoorPlaced::L:
				exitPos.x = RIGHT + OFFSET + mTargetPlayer->GetWorldScaleX() * 0.5f;
				exitPos.y = BOTTOM + enteringHeight + mTargetPlayer->GetWorldScaleY() * 0.5f;
				break;
			case DoorPlaced::R:
				exitPos.x = LEFT - OFFSET - mTargetPlayer->GetWorldScaleX() * 0.5f;
				exitPos.y = BOTTOM + enteringHeight + mTargetPlayer->GetWorldScaleY() * 0.5f;
				break;
			case DoorPlaced::T:
				exitPos.x = mExitDoor->GetWorldPositionX() - enteringPos;
				exitPos.y = BOTTOM - OFFSET - mTargetPlayer->GetWorldScaleY() * 0.5f;
				break;
			case DoorPlaced::B:
				exitPos.x = mExitDoor->GetWorldPositionX() - enteringPos;
				exitPos.y = TOP + OFFSET + mTargetPlayer->GetWorldScaleY() * 0.5f;
				break;
			case hj::DoorPlaced::End:
			default:
				break;
			}
			mTargetPlayer->SetPositionXY(exitPos);
			mTargetPlayer->GetTransform()->FixedUpdate();
			mTargetPlayer->GetPlayerScript()->DashTrailTransformFixedUpdate();
		}
	}

	void RoomDoor::CheckFadeInComplete()
	{
		if (!mFadeScript)
			return;

		eFadeState fadeState = mFadeScript->GetFadeState();
		if (eFadeState::FadeInEnd == fadeState)
		{
			mbFadeInComplete = true;
		}
	}

	void RoomDoor::ProcessFadeInEnd()
	{
		if (!mFadeScript)
			return;

		mbEntryStart = false;
		mbExitStart = false;

		mbFadeOutComplete = true;
		mbFadeInComplete = true;
	}

	void RoomDoor::SetOwnerRoom(class RoomBase* ownerRoom)
	{
		if (!ownerRoom)
			return;

		mOwnerRoom = ownerRoom;
	}

	RoomBase* RoomDoor::GetOwnerRoom()
	{
		RoomBase* ownerRoom = nullptr;

		if (mOwnerRoom)
		{
			ownerRoom = mOwnerRoom;
		}

		return ownerRoom;
	}

}