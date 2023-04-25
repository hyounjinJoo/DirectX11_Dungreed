#pragma once
#include "hjBaseRenderer.h"

namespace hj
{
	using namespace hj::math;

	struct TileData
	{
		int					mImgIdx;
		Vector2	mStartUV;
		int					mPadding;

		TileData()
			: mImgIdx(-1)
			, mPadding(0)
		{}
	};

	class TileMap :
		public BaseRenderer
	{
	public:
		TileMap();
		virtual ~TileMap();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;
		
		void UpdateData();


		void SetAtlasTex(std::shared_ptr<Texture> atlasTexture)
		{
			mAtlasTexture = atlasTexture;
			Vector2 atlasSize = mAtlasTexture->GetTexSize();
			mSliceUV = mSlicePixel / atlasSize;
		}

		void SetTileSize(Vector2 pixelSize)
		{
			mSlicePixel = pixelSize;
			if (nullptr != mAtlasTexture)
			{
				Vector2 atlasSize = mAtlasTexture->GetTexSize();
				mSliceUV = mSlicePixel / atlasSize;
			}
		}

		void SetTileMapCount(const Vector2& count) { SetTileMapCount((UINT)count.x, (UINT)count.y); }
		void SetTileMapCount(UINT countX, UINT countY);
		void SetTileData(int tileIdx, int imgIdx);
		void ClearTileData();

		void SetAllTileData(int imgIdx);
		void SetAllTileData(const std::vector<TileData>& data);

	private:
		std::shared_ptr<Texture>       mAtlasTexture;
		Vector2              mSlicePixel;
		Vector2              mSliceUV;

		UINT                mRowCount;
		UINT                mColCount;

		UINT                mTileCountX;
		UINT                mTileCountY;
		std::vector<TileData>   mTileData;
		StructuredBuffer*   mBuffer;

		bool                mbBufferUpdated;
	};
}
