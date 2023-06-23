#include "hjStage1StartLTRB.h"
#include "hjMap.h"
#include "hjResources.h"
#include "hjCameraScript.h"
#include "hjObject.h"
#include "hjRoomNotPass.h"
#include "hjRoomPassThrough.h"
#include "hjRoomDoor.h"
#include "hjScene.h"
#include "hjSceneManager.h"
#include "hjMonsterSpawner.h"
#include "hjSkeletonBase.h"

namespace hj
{
	Stage1StartLTRB::Stage1StartLTRB()
	{
		std::shared_ptr<Map> mapData = Resources::Find<Map>(WIDE("MAP_01_S_LTRB"));
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
				, Vector3(MapR - (tileSize * 2.5f), MapB + tileSize, 0.f));
			notPass->SetScale(Vector3(tileSize * 5.f, tileSize * 2.f, 1.f));
			mGameObjects.push_back(notPass);

			notPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround
				, Vector3(tileSize * -4.5f, MapB + tileSize, 0.f));
			notPass->SetScale(Vector3(tileSize * 11.f, tileSize * 2.f, 1.f));
			mGameObjects.push_back(notPass);

			notPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround
				, Vector3(MapL + tileSize, tileSize, 0.f));
			notPass->SetScale(Vector3(tileSize * 2.f, tileSize * 2.f, 1.f));
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

			passThrough = object::Instantiate<RoomPassThrough>(eLayerType::ForeGround
				, Vector3((tileSize * 3.f), MapB + (tileSize * 2.f) - (platformSize * 0.5f), 0.f));
			passThrough->SetScale(Vector3(tileSize * 4.f, platformSize, 1.f));
			mGameObjects.push_back(passThrough);

			passThrough->GetTransform()->FixedUpdate();
			MonsterSpawner* spawner = object::Instantiate<MonsterSpawner>(eLayerType::ForeGround, passThrough->GetPosition() + Vector3(0.f, 100.f, 0.f));
			spawner->SetOwnerRoom(this);

			SkeletonBase* skelBase = object::Instantiate<SkeletonBase>(eLayerType::MonsterHas);
			spawner->SetSpawnMonster(skelBase);
			skelBase->SetType(eSkeletonType::GreatSword);

			RoomDoor* DoorL = object::Instantiate<RoomDoor>(eLayerType::ForeGround
				, Vector3(MapL, MapB + (tileSize * 4.f), 0.f));
			DoorL->GetTransform()->FixedUpdate();
			DoorL->SetDoorPlaced(DoorPlaced::L);
			DoorL->SetName(WIDE("LeftDoor"));

			mDoors[static_cast<UINT>(DoorPlaced::L)] = DoorL;
			mGameObjects.push_back(DoorL);

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

			RoomDoor* DoorB = object::Instantiate<RoomDoor>(eLayerType::ForeGround
				, Vector3((tileSize * 3.f), MapB, 0.f));
			DoorB->GetTransform()->FixedUpdate();
			DoorB->SetDoorPlaced(DoorPlaced::B);
			DoorB->SetName(WIDE("BottomDoor"));
			mDoors[static_cast<UINT>(DoorPlaced::B)] = DoorB;
			mGameObjects.push_back(DoorB);
		}
	}

	Stage1StartLTRB::~Stage1StartLTRB()
	{
	}

	void Stage1StartLTRB::Initialize()
	{
		RoomBase::Initialize();
	}
	void Stage1StartLTRB::Update()
	{
		RoomBase::Update();
	}
	void Stage1StartLTRB::FixedUpdate()
	{
		RoomBase::FixedUpdate();
	}
	void Stage1StartLTRB::Render()
	{
		RoomBase::Render();
	}
}