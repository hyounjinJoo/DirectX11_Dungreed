#pragma once
#include "hjResource.h"
#include "hjMath.h"
#include "hjTileMap.h"

namespace hj
{
	using namespace hj::math;

	struct tileLayer
	{
		std::vector<TileData> tileData;
	};

	class Map :
		public Resource
	{
	public:
		Map();
		virtual ~Map();

#pragma region Overrided Function
		virtual HRESULT Load(const std::wstring& path) override;
#pragma endregion

#pragma region Member Function
		UINT GetLayerCount() { return mLayerCount; }
		const tileLayer* GetTileMapLayer(UINT Index);
		const Vector2& GetTileSize() const { return mTileSize; }
		const Vector2& GetTileCount() const { return mTileCount; }
		const Vector2& GetTileMapSize() const { return mMapSize; }
#pragma endregion

#pragma region Member Variables
	private:
		std::vector<tileLayer*>   mTileLayer;
		Vector2 mTileSetSize;
		UINT mLayerCount;
		Vector2 mTileCount;
		Vector2 mTileSize;
		Vector2 mMapSize;
#pragma endregion
	};
}