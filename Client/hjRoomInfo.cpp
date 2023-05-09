#include "hjRoomInfo.h"

namespace hj
{
	RoomInfo::RoomInfo()
		: mTileMapObj(nullptr)
		, mTileMapFileName()
		, mLayerCount(0)
		, mDoorType(DoorType::END)
		, mRoomType(RoomType::END)
	{
	}
	RoomInfo::~RoomInfo()
	{
		if (mTileMapObj)
			mTileMapObj = nullptr;

		size_t Size = mNotPassColliders.size();
		for (size_t iter = 0; iter < Size; ++iter)
		{
			mNotPassColliders[iter] = nullptr;
		}
		mNotPassColliders.clear();

		//Size = mPassThroughColliders.size();
		//for (size_t iter = 0; iter < Size; ++iter)
		//{
		//	mPassThroughColliders[iter] = nullptr;
		//}
		//mPassThroughColliders.clear();

		//Size = mDoorColliders.size();
		//for (size_t iter = 0; iter < Size; ++iter)
		//{
		//	mDoorColliders[iter] = nullptr;
		//}
		//mDoorColliders.clear();
	}
}