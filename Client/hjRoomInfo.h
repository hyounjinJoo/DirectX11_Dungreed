#pragma once
#include "hjEngine.h"
#include "hjMath.h"
#include "hjRoomNotPass.h"
#include "hjRoomPassThrough.h"

namespace hj
{
	class RoomInfo
	{
		enum class DoorType
		{
			L,
			LT,
			LR,
			LB,
			LTR,
			LTB,
			LRB,
			LTRB,
			T,
			TR,
			TRB,
			R,
			RB,
			B,
			END,
		};
		enum class RoomType
		{
			Start,
			Normal,
			Shop,
			Inn,
			BossEntrance,
			BossLobby,
			Boss1,
			Boss2,
			Boss3,
			Ending,
			END,
		};

	public:
		RoomInfo();
		~RoomInfo();

	private:
		class GameObject* mTileMapObj;

		std::wstring mTileMapFileName;
		UINT mLayerCount;
		math::Vector4 mMapLTRBLimit;
		math::Vector2 mMapSize;
		DoorType mDoorType;
		RoomType mRoomType;

		std::vector<class RoomNotPass*> mNotPassColliders;
		std::vector<class RoomPassThrough*> mPassThroughColliders;
		//std::vector<RoomDoor> mDoorColliders;
	};
}

