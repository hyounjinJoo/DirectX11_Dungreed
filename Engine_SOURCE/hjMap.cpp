#include "hjMap.h"
#include "hjXmlParser.h"
#include "hjTexture.h"
#include "hjResources.h"

namespace hj
{
	Map::Map()
		: Resource(eResourceType::MapData)
	{
	}
	Map::~Map()
	{
		for (auto layer : mTileLayer) {
			delete layer;
		}
		mTileLayer.clear();
	}
	HRESULT Map::Load(const std::wstring& path)
	{
		XmlParser* parser = new XmlParser;
		bool parseResult = parser->LoadFile(path);
		if (!parseResult)
			return E_FAIL;

		parseResult = parser->FindElem(WIDE("map"));
		mTileCount.x = static_cast<float>(parser->GetIntAttribute("width"));
		mTileCount.y = static_cast<float>(parser->GetIntAttribute("height"));
		mTileSize.x = static_cast<float>(parser->GetIntAttribute("tilewidth"));
		mTileSize.y = static_cast<float>(parser->GetIntAttribute("tileheight"));
		mLayerCount = static_cast<UINT>(parser->GetIntAttribute("nextlayerid"));
		mMapSize.x = mTileCount.x * mTileSize.x;
		mMapSize.y = mTileCount.y * mTileSize.y;
		parser->IntoElem();

		parseResult = parser->FindElem(WIDE("tileset"));
		std::shared_ptr<Texture> texture = TEX_FIND("MapTile");
		int startIdx = parser->GetIntAttribute("firstgid");
		if (!texture)
			return E_FAIL;
		mTileSetSize = texture->GetTexSize();

		Vector2 sliceUV = mTileSize / mTileSetSize;

		for (UINT layerIdx = 0; layerIdx < mLayerCount; ++layerIdx)
		{
			tileLayer* layer = new tileLayer;
			parseResult = parser->FindElem(WIDE("layer"));
			parser->IntoElem();
			parseResult = parser->FindElem(WIDE("data"));
			parser->IntoElem(); 
			
			while (parser->FindElem("tile"))
			{
				TileData data = {};
				if (parser->HasAttribute("gid"))
				{
					int imgIdx = parser->GetIntAttribute("gid") - startIdx;
					data.mImgIdx = imgIdx;

					UINT mColCount = (UINT)mTileSetSize.x / (UINT)mTileSize.x;
					UINT mRowCount = (UINT)mTileSetSize.y / (UINT)mTileSize.y;

					int row = imgIdx / mColCount;
					int col = imgIdx % mColCount;

					data.mStartUV = Vector2(sliceUV.x * col, sliceUV.y * row);
				}
				layer->tileData.push_back(data);
			}
			mTileLayer.push_back(layer);

			parser->OutOfElem();
			parser->OutOfElem();
		}		
		
		delete parser;
		return NOERROR;
	}

	const tileLayer* Map::GetTileMapLayer(UINT Index)
	{
		if (Index >= mLayerCount)
			return nullptr;
		
		return mTileLayer[Index];
	}

}