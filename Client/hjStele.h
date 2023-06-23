#pragma once
#include "hjActor.h"

namespace hj
{
	enum class eSteleState
	{
		Closed,
		Idle,
		Opened,
		None,
	};

	enum class eDirType
	{
		Vertical,
		Horizon,
		End,
	};

	class RoomDoor;

	class Stele :
		public Actor
	{
	public:
		Stele();
		virtual ~Stele();

	public:
		void ChangeSteleState(eSteleState state);
		eSteleState GetSteleState() { return mSteleState; }
		void Closed();
		void Opened();

		void ChangeDirType(eDirType type);

	public:
		void SetDoor(RoomDoor* door)
		{
			if (door)
			{
				mDoor = door;
			}
		}

	private:
		void CreateAnimation();

	private:
		eSteleState mSteleState;
		eDirType mType;
		RoomDoor* mDoor;
		class RoomNotPass* mRoomNotPass;

		GameObject* mSoundObj;
	};
}