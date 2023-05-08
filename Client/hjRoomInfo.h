#pragma once
#include "hjEngine.h"
#include "hjMath.h"
#include "hjRoomNotPass.h"

namespace hj
{
	class RoomInfo
	{
		enum class DoorType
		{
			None,
			Door2,
			Door3,
			Door4,
			End,
		};
		enum class RoomType
		{
			None,
			Start,
			Normal,
			Shop,
			Inn,
			BossLobby,
			Boss,
			End,
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

		std::vector<RoomNotPass*> mNotPassColliders;
		//std::vector<RoomPassThrough> mPassThroughColliders;
		//std::vector<RoomDoor> mDoorColliders;
	};
}

