#include "hjStage1StartL.h"
#include "hjMap.h"
#include "hjResources.h"
#include "hjObject.h"
#include "hjRoomNotPass.h"
#include "hjRoomPassThrough.h"
#include "hjRoomDoor.h"
#include "hjScene.h"
#include "hjSceneManager.h"

namespace hj
{
	Stage1StartL::Stage1StartL()
	{
		std::shared_ptr<Map> mapData = Resources::Find<Map>(WIDE("MAP_01_S_L"));
		if (mapData)
		{
			UINT layerCount = mapData->GetLayerCount();
			Vector2 mapSize = mapData->GetTileMapSize();
			Vector2 mapPos = GetPositionXY();
			mMapLTRBlimit.x = mapPos.x - mapSize.x * 0.5f;
			mMapLTRBlimit.y = mapPos.y + mapSize.y * 0.5f;
			mMapLTRBlimit.z = mapPos.x + mapSize.x * 0.5f;
			mMapLTRBlimit.w = mapPos.y - mapSize.y * 0.5f;

#define MapL mMapLTRBlimit.x
#define MapT mMapLTRBlimit.y
#define MapR mMapLTRBlimit.z
#define MapB mMapLTRBlimit.w

			for (UINT layerIdx = 0; layerIdx < layerCount; ++layerIdx)
			{
				GameObject* obj = object::Instantiate<GameObject>(eLayerType::BackGround);
				obj->SetName(L"TileMap");
				Transform* tr = obj->GetComponent<Transform>();
				tr->SetPositionZ(5.f - (float)layerIdx);
				tr->SetScale(Vector3(mapData->GetTileMapSize(), 1.f));

				TileMap* tilemap = obj->AddComponent<TileMap>();
				std::shared_ptr<Material> material = MTRL_FIND("MTRL_Map_Tile");
				tilemap->SetMaterial(material);
				tilemap->SetMesh(MESH_FIND("Mesh_Rect"));
				tilemap->SetAtlasTex(material->GetTexture(eTextureSlot::T0));
				tilemap->SetTileSize(mapData->GetTileSize());
				tilemap->SetTileMapCount(mapData->GetTileCount());

				const tileLayer* layer = mapData->GetTileMapLayer(layerIdx);
				tilemap->SetAllTileData(layer->tileData);

				mGameObjects.push_back(obj);
			}

			float tileSize = 80.f;

			RoomNotPass* notPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround
				, Vector3(0.f, MapT - (tileSize * 1.5f), 0.f));
			notPass->SetScale(Vector3(tileSize * 27.f, tileSize * 3, 1.f));
			mGameObjects.push_back(notPass);

			notPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround
				, Vector3(MapR - (tileSize * 0.5f), 0.f, 0.f));
			notPass->SetScale(Vector3(tileSize, tileSize * 12.f, 1.f));
			mGameObjects.push_back(notPass);

			notPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround
				, Vector3(0.f, MapB + tileSize, 0.f));
			notPass->SetScale(Vector3(tileSize * 27.f, tileSize * 2.f, 1.f));
			mGameObjects.push_back(notPass);

			notPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround
				, Vector3(MapL + tileSize, MapT - (tileSize * 4.5f), 0.f));
			notPass->SetScale(Vector3(tileSize * 2.f, tileSize * 3.f, 1.f));
			mGameObjects.push_back(notPass);

			RoomDoor* DoorL = object::Instantiate<RoomDoor>(eLayerType::ForeGround
				, Vector3(MapL, MapB + (tileSize * 4.f), 0.f));
			DoorL->GetTransform()->FixedUpdate();
			DoorL->SetDoorPlaced(DoorPlaced::L);
			DoorL->SetName(WIDE("LeftDoor"));

			mDoors[static_cast<UINT>(DoorPlaced::L)] = DoorL;
			mGameObjects.push_back(DoorL);
		}
	}

	Stage1StartL::~Stage1StartL()
	{
	}

	void Stage1StartL::Initialize()
	{
		RoomBase::Initialize();
	}
	void Stage1StartL::Update()
	{
		RoomBase::Update();
	}
	void Stage1StartL::FixedUpdate()
	{
		RoomBase::FixedUpdate();
	}
	void Stage1StartL::Render()
	{
		RoomBase::Render();
	}
}