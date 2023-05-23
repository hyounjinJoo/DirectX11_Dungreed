#include "hjStage1StartLRB.h"
#include "hjMap.h"
#include "hjResources.h"
#include "hjCameraScript.h"
#include "hjObject.h"
#include "hjRoomNotPass.h"
#include "hjRoomPassThrough.h"
#include "hjRoomDoor.h"
#include "hjScene.h"
#include "hjSceneManager.h"

namespace hj
{
	Stage1StartLRB::Stage1StartLRB()
	{
		std::shared_ptr<Map> mapData = Resources::Find<Map>(WIDE("MAP_01_S_LRB"));
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
			notPass->SetScale(Vector3(tileSize * 30, tileSize * 3, 1.f));
			mGameObjects.push_back(notPass);

			notPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround
				, Vector3(MapR - tileSize, tileSize * 3.f, 0.f));
			notPass->SetScale(Vector3(tileSize * 2.f, tileSize * 6.f, 1.f));
			mGameObjects.push_back(notPass);

			notPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround
				, Vector3(MapR - (tileSize * 4.f), MapB + tileSize, 0.f));
			notPass->SetScale(Vector3(tileSize * 8.f, tileSize * 2.f, 1.f));
			mGameObjects.push_back(notPass);

			notPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround
				, Vector3(-tileSize * 6.f, MapB + tileSize, 0.f));
			notPass->SetScale(Vector3(tileSize * 18.f, tileSize * 2.f, 1.f));
			mGameObjects.push_back(notPass);

			notPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround
				, Vector3(MapL + tileSize, MapT - (tileSize * 4.5f), 0.f));
			notPass->SetScale(Vector3(tileSize * 2.f, tileSize * 3.f, 1.f));
			mGameObjects.push_back(notPass);

			float platformSize = tileSize * 0.33f;
			RoomPassThrough* passThrough = object::Instantiate<RoomPassThrough>(eLayerType::ForeGround
					, Vector3(tileSize * 5.f, MapB + (tileSize * 2.f) - (platformSize * 0.5f), 0.f));
			passThrough->SetScale(Vector3(tileSize * 4.f, platformSize, 1.f));
			mGameObjects.push_back(passThrough);

			RoomDoor* DoorL = object::Instantiate<RoomDoor>(eLayerType::ForeGround
				, Vector3(MapL, MapB + (tileSize * 4.f), 0.f));
			DoorL->GetTransform()->FixedUpdate();
			DoorL->SetDoorPlaced(DoorPlaced::L);
			DoorL->SetName(WIDE("LeftDoor"));

			mDoors[static_cast<UINT>(DoorPlaced::L)] = DoorL;
			mGameObjects.push_back(DoorL);

			RoomDoor* DoorR = object::Instantiate<RoomDoor>(eLayerType::ForeGround
				, Vector3(MapR, MapB + (tileSize * 4.f), 0.f));
			DoorR->GetTransform()->FixedUpdate();
			DoorR->SetDoorPlaced(DoorPlaced::R);
			DoorR->SetName(WIDE("RightDoor"));
			mDoors[static_cast<UINT>(DoorPlaced::R)] = DoorR;
			mGameObjects.push_back(DoorR);

			RoomDoor* DoorB = object::Instantiate<RoomDoor>(eLayerType::ForeGround
				, Vector3(tileSize * 5.f, MapB, 0.f));
			DoorB->GetTransform()->FixedUpdate();
			DoorB->SetDoorPlaced(DoorPlaced::B);
			DoorB->SetName(WIDE("BottomDoor"));
			mDoors[static_cast<UINT>(DoorPlaced::B)] = DoorB;
			mGameObjects.push_back(DoorB);
		}
	}

	Stage1StartLRB::~Stage1StartLRB()
	{
	}

	void Stage1StartLRB::Initialize()
	{
		RoomBase::Initialize();
	}
	void Stage1StartLRB::Update()
	{
		RoomBase::Update();
	}
	void Stage1StartLRB::FixedUpdate()
	{
		RoomBase::FixedUpdate();
	}
	void Stage1StartLRB::Render()
	{
		RoomBase::Render();
	}
}