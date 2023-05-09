#include "hjRoomDoor.h"
#include "hjCollider2D.h"
#include "hjFadeScript.h"
#include "hjTime.h"

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
	}

	RoomDoor::~RoomDoor()
	{
	}

	void RoomDoor::Initialize()
	{
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

		Vector4 thisLTRB = GetWorldLTRB();
#define OFFSET 10.f
#define LEFT thisLTRB.x
#define TOP thisLTRB.y
#define RIGHT thisLTRB.z
#define BOTTOM thisLTRB.w

		switch (place)
		{
		case hj::DoorPlaced::L:
			mExitPos = Vector2(RIGHT + OFFSET, 0.f);
			break;
		case hj::DoorPlaced::T:
			mExitPos = Vector2(0.f, BOTTOM + OFFSET);
			break;
		case hj::DoorPlaced::R:
			mExitPos = Vector2(LEFT - OFFSET, 0.f);
			break;
		case hj::DoorPlaced::B:
			mExitPos = Vector2(0.f, TOP - OFFSET);
			break;
		case hj::DoorPlaced::None:
			mExitPos = Vector2::Zero;
			break;
		default:
			break;
		}
	}

	Vector2 RoomDoor::GetExitDoorsExitPos()
	{
		Vector2 result = Vector2::Zero;
		if (mExitDoor)
		{
			result = mExitDoor->GetExitDoorsExitPos();
		}

		return result;
	}

	void RoomDoor::FadeOutStart()
	{
		mbEntryStart = true;
		mbFadeOutComplete = false;

		//FadeOut Start
		mFadeScript->FadeOutStart();
		
		// DeltaTime Update Stop
		Time::StopDeltaTimeUpdate(true);

		// Move Process Start
		// 1. GetExitDoor's exit pos
		// 2. verify exit pos
		//    - if exit pos's x, y component are 0, then return
		// 3. GetPlayer 
		// 4. player pos setted verified exit pos
	}

	void RoomDoor::FadeInStart()
	{
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