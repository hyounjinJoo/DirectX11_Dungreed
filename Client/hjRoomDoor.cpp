#include "hjRoomDoor.h"
#include "hjCollider2D.h"
#include "hjFadeScript.h"
#include "hjTime.h"
#include "hjPlayer.h"
#include "hjDoorScript.h"

namespace hj
{
	RoomDoor::RoomDoor()
		: mExitDoor(nullptr)
		, mFadeScript(nullptr)
		, mDoorCollider(nullptr)
		, mbEntryStart(false)
		, mbFadeOutComplete(false)
		, mbExitStart(false)
		, mbFadeInComplete(false)
		, mDoorPlaced(DoorPlaced::None)
	{
		mDoorCollider = AddComponent<Collider2D>();
		AddComponent<DoorScript>();

		Vector2 scale = Vector2(80.f, 320.f);
		SetScaleXY(scale);
	}

	RoomDoor::~RoomDoor()
	{
		mFadeScript = nullptr;
		mDoorCollider = nullptr;
		mExitDoor = nullptr;
	}

	void RoomDoor::Initialize()
	{
		int a = 0;
	}

	void RoomDoor::Update()
	{
		GameObject::Update();

		if (DoorPlaced::None == mDoorPlaced)
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

		if (DoorPlaced::None == mDoorPlaced)
			return;
	}

	void RoomDoor::Render()
	{
		GameObject::Render();

		if (DoorPlaced::None == mDoorPlaced)
			return;
	}

	void RoomDoor::SetDoorPlaced(DoorPlaced place)
	{
		mDoorPlaced = place;
	}

	void RoomDoor::SetFadeScript(class FadeScript* fadeScript)
	{
		if (fadeScript)
			mFadeScript = fadeScript;
	}

	void RoomDoor::FadeOutStart(class Player* player)
	{
		if (!mFadeScript || !player || !mExitDoor)
			return;

		mbEntryStart = true;
		mbFadeOutComplete = false;

		//FadeOut Start
		mFadeScript->FadeOutStart();
		
		// DeltaTime Update Stop
		Time::StopDeltaTimeUpdate(true);

		// Move Process Start
		// 1. GetExitDoor's exit pos
		DoorPlaced exitDoorPlace = mExitDoor->GetDoorPlacedType();
		// 2. player pos setted verified exit pos

		Vector4 exitDoorLTRB = mExitDoor->GetWorldLTRB();
#define OFFSET 1.f
#define LEFT exitDoorLTRB.x
#define TOP exitDoorLTRB.y
#define RIGHT exitDoorLTRB.z
#define BOTTOM exitDoorLTRB.w

		Vector2 exitPos = Vector2::Zero;
		switch (exitDoorPlace)
		{
		case DoorPlaced::L:
			exitPos.x = RIGHT + OFFSET + player->GetWorldScaleX() * 0.5f;
			exitPos.y = player->GetWorldPositionY();
			break;
		case DoorPlaced::R:
			exitPos.x = LEFT - OFFSET - player->GetWorldScaleX() * 0.5f;
			exitPos.y = player->GetWorldPositionY();
			break;
		case DoorPlaced::T:
			exitPos.x = player->GetWorldPositionX();
			exitPos.y = BOTTOM - OFFSET - player->GetWorldScaleY() * 0.5f;
			break;
		case DoorPlaced::B:
			exitPos.x = player->GetWorldPositionX();
			exitPos.y = TOP + OFFSET + player->GetWorldScaleY() * 0.5f;
			break;
		case hj::DoorPlaced::None:
		default:
			break;
		}
		player->SetPositionXY(exitPos);
	}

	void RoomDoor::FadeInStart()
	{
		if (!mFadeScript)
			return;

		mbEntryStart = false;
		mbExitStart = true;
		mbFadeInComplete = false;
		mFadeScript->FadeInStart();

		Time::StopDeltaTimeUpdate(false);
	}

	void RoomDoor::CheckFadeOutComplete()
	{
		if (!mFadeScript)
			return;

		eFadeState fadeState = mFadeScript->GetFadeState();
		if (eFadeState::FadeOutEnd == fadeState)
		{
			mbFadeOutComplete = true;
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

}