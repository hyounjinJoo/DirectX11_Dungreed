#include "hjStage1StartTR2.h"
#include "hjMap.h"
#include "hjResourceManager.h"
#include "hjCameraScript.h"
#include "hjObject.h"
#include "hjRoomNotPass.h"
#include "hjRoomPassThrough.h"
#include "hjRoomDoor.h"
#include "hjScene.h"
#include "hjSceneManager.h"

namespace hj
{
	Stage1StartTR2::Stage1StartTR2()
	{
		std::shared_ptr<Map> mapData = ResourceManager::Find<Map>(WIDE("MAP_01_S_TR2"));
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
				std::shared_ptr<Material> material = MTRL_FIND_STR("MTRL_Map_Tile");
				tilemap->SetMaterial(material);
				tilemap->SetMesh(MESH_FIND_STR("Mesh_Rect"));
				tilemap->SetAtlasTex(material->GetTexture(eTextureSlot::T0));
				tilemap->SetTileSize(mapData->GetTileSize());
				tilemap->SetTileMapCount(mapData->GetTileCount());

				const tileLayer* layer = mapData->GetTileMapLayer(layerIdx);
				tilemap->SetAllTileData(layer->tileData);

				mGameObjects.push_back(obj);
			}

			float tileSize = 80.f;


			RoomNotPass* notPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround
				, Vector3(tileSize * -4.5f, MapT - (tileSize * 2.f), 0.f));
			notPass->SetScale(Vector3(tileSize * 11.f, tileSize * 4.f, 1.f));
			mGameObjects.push_back(notPass);

			notPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround
				, Vector3(tileSize * 3.f, MapT - tileSize, 0.f));
			notPass->SetScale(Vector3(tileSize * 4.f, tileSize * 2.f, 1.f));
			mGameObjects.push_back(notPass);

			notPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround
				, Vector3(MapR - (tileSize * 2.5f), MapT - (tileSize * 2.f), 0.f));
			notPass->SetScale(Vector3(tileSize * 5.f, tileSize * 4.f, 1.f));
			mGameObjects.push_back(notPass);

			notPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround
				, Vector3(MapR - tileSize, tileSize * 1.5f, 0.f));
			notPass->SetScale(Vector3(tileSize * 2.f, tileSize, 1.f));
			mGameObjects.push_back(notPass);

			notPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround
				, Vector3(MapR - tileSize, MapB + tileSize * 2.5f, 0.f));
			notPass->SetScale(Vector3(tileSize * 2.f, tileSize, 1.f));
			mGameObjects.push_back(notPass);

			notPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround
				, Vector3(0.f, MapB + tileSize, 0.f));
			notPass->SetScale(Vector3(tileSize * 20.f, tileSize * 2.f, 1.f));
			mGameObjects.push_back(notPass);

			notPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround
				, Vector3(MapL + tileSize, 0.f, 0.f));
			notPass->SetScale(Vector3(tileSize * 2.f, tileSize * 12.f, 1.f));
			mGameObjects.push_back(notPass);

			float platformSize = tileSize * 0.33f;
			RoomPassThrough* passThrough = object::Instantiate<RoomPassThrough>(eLayerType::ForeGround
				, Vector3(tileSize * 3.f, MapB + (tileSize * 5.f) - (platformSize * 0.5f), 0.f));
			passThrough->SetScale(Vector3(tileSize * 4.f, platformSize, 1.f));
			mGameObjects.push_back(passThrough);

			passThrough = object::Instantiate<RoomPassThrough>(eLayerType::ForeGround
				, Vector3(tileSize * 3.f, MapB + (tileSize * 7.f) - (platformSize * 0.5f), 0.f));
			passThrough->SetScale(Vector3(tileSize * 4.f, platformSize, 1.f));
			mGameObjects.push_back(passThrough);

			RoomDoor* DoorT = object::Instantiate<RoomDoor>(eLayerType::ForeGround
				, Vector3((tileSize * 3.f), MapT - (tileSize * 2.f), 0.f));
			DoorT->GetTransform()->FixedUpdate();
			DoorT->SetDoorPlaced(DoorPlaced::T);
			DoorT->SetName(WIDE("TopDoor"));
			mDoors[static_cast<UINT>(DoorPlaced::T)] = DoorT;
			mGameObjects.push_back(DoorT);

			RoomDoor* DoorR = object::Instantiate<RoomDoor>(eLayerType::ForeGround
				, Vector3(MapR, MapB + (tileSize * 5.f), 0.f));
			DoorR->GetTransform()->FixedUpdate();
			DoorR->SetDoorPlaced(DoorPlaced::R);
			DoorR->SetName(WIDE("RightDoor"));
			mDoors[static_cast<UINT>(DoorPlaced::R)] = DoorR;
			mGameObjects.push_back(DoorR);
		}
	}

	Stage1StartTR2::~Stage1StartTR2()
	{
	}

	void Stage1StartTR2::Initialize()
	{
		RoomBase::Initialize();
	}
	void Stage1StartTR2::Update()
	{
		RoomBase::Update();
	}
	void Stage1StartTR2::FixedUpdate()
	{
		RoomBase::FixedUpdate();
	}
	void Stage1StartTR2::Render()
	{
		RoomBase::Render();
	}
}