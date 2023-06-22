#include "hjStage1BossRoom.h"
#include "hjMap.h"
#include "hjResources.h"
#include "hjCameraScript.h"
#include "hjObject.h"
#include "hjRoomNotPass.h"
#include "hjRoomPassThrough.h"
#include "hjRoomDoor.h"
#include "hjFadeObject.h"
#include "hjStage1Boss.h"
#include "hjActor.h"
#include "hjBoss1Sword.h"
#include "hjBossSpawner.h"

namespace hj
{
	Stage1BossRoom::Stage1BossRoom()
		: RoomBase()
		, mBoss(nullptr)
		, mbBossSpawned(false)
	{
		std::shared_ptr<Map> mapData = Resources::Find<Map>(WIDE("MAP_01_BossRoom"));
		if (mapData)
		{
			UINT layerCount = mapData->GetLayerCount();
			Vector2 mapSize = mapData->GetTileMapSize();
			Vector2 mapPos = GetPositionXY();
			mMapLTRBlimit.x = mapPos.x - mapSize.x * 0.5f;
			mMapLTRBlimit.y = mapPos.y + mapSize.y * 0.5f;
			mMapLTRBlimit.z = mapPos.x + mapSize.x * 0.5f;
			mMapLTRBlimit.w = mapPos.y - mapSize.y * 0.5f;

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

			RoomNotPass* floor = object::Instantiate<RoomNotPass>(eLayerType::ForeGround
				, Vector3(0.f, -760.f, 0.f));
			floor->SetScale(Vector3(1760.f, 240.f, 1.f));

			mGameObjects.push_back(floor);

			RoomNotPass* notPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround
				, Vector3(880.f, 280.f, 0.f));
			notPass->SetScale(Vector3(320.f, 1200.f, 1.f));

			mGameObjects.push_back(notPass);

			notPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround
				, Vector3(-880.f, 280.f, 0.f));
			notPass->SetScale(Vector3(320.f, 1200.f, 1.f));

			mGameObjects.push_back(notPass);

			notPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround
				, Vector3(0.f, 800.f, 0.f));
			notPass->SetScale(Vector3(1760.f, 160.f, 1.f));

			mGameObjects.push_back(notPass);

			RoomPassThrough* passThrough = object::Instantiate<RoomPassThrough>(eLayerType::ForeGround
				, Vector3(-1.f, -508.f, 0.f));
			passThrough->SetScale(Vector3(636.f, 56.f, 1.f));

			mGameObjects.push_back(passThrough);

			passThrough = object::Instantiate<RoomPassThrough>(eLayerType::ForeGround
				, Vector3(-441.f, -267.f, 0.f));
			passThrough->SetScale(Vector3(236.f, 56.f, 1.f));

			mGameObjects.push_back(passThrough);

			passThrough = object::Instantiate<RoomPassThrough>(eLayerType::ForeGround
				, Vector3(440.f, -267.f, 0.f));
			passThrough->SetScale(Vector3(236.f, 56.f, 1.f));

			mGameObjects.push_back(passThrough);

			passThrough = object::Instantiate<RoomPassThrough>(eLayerType::ForeGround
				, Vector3(-521.f, -27.f, 0.f));
			passThrough->SetScale(Vector3(236.f, 56.f, 1.f));

			mGameObjects.push_back(passThrough);

			passThrough = object::Instantiate<RoomPassThrough>(eLayerType::ForeGround
				, Vector3(518.f, -27.f, 0.f));
			passThrough->SetScale(Vector3(236.f, 56.f, 1.f));

			mGameObjects.push_back(passThrough);

			passThrough = object::Instantiate<RoomPassThrough>(eLayerType::ForeGround
				, Vector3(-521.f, 213.f, 0.f));
			passThrough->SetScale(Vector3(236.f, 56.f, 1.f));

			mGameObjects.push_back(passThrough);

			passThrough = object::Instantiate<RoomPassThrough>(eLayerType::ForeGround
				, Vector3(518.f, 213.f, 0.f));
			passThrough->SetScale(Vector3(236.f, 56.f, 1.f));

			mGameObjects.push_back(passThrough);

			passThrough = object::Instantiate<RoomPassThrough>(eLayerType::ForeGround
				, Vector3(-1.f, 372.f, 0.f));
			passThrough->SetScale(Vector3(314.f, 56.f, 1.f));

			mGameObjects.push_back(passThrough);

			RoomDoor* DoorL = object::Instantiate<RoomDoor>(eLayerType::ForeGround
				, Vector3(-920.f, -480.f, 0.f));
			DoorL->GetTransform()->FixedUpdate();
			DoorL->SetDoorPlaced(DoorPlaced::L);
			DoorL->SetName(WIDE("LeftDoor"));

			mDoors[static_cast<UINT>(DoorPlaced::L)] = DoorL;
			mGameObjects.push_back(DoorL);

			RoomDoor* DoorR = object::Instantiate<RoomDoor>(eLayerType::ForeGround
				, Vector3(920.f, -480.f, 0.f));
			DoorR->GetTransform()->FixedUpdate();
			DoorR->SetDoorPlaced(DoorPlaced::R);
			DoorR->SetName(WIDE("RightDoor"));

			mDoors[static_cast<UINT>(DoorPlaced::R)] = DoorR;
			mGameObjects.push_back(DoorR);

			DoorL->SetExitDoor(DoorR);
			DoorR->SetExitDoor(DoorL);

			mBoss = object::InstantiateNoInitialize<Stage1Boss>(eLayerType::Monster);
			mBoss->SetOwnerRoom(this);

			mGameObjects.push_back(mBoss);

			BossSpawner* spawner = object::Instantiate<BossSpawner>(eLayerType::ForeGround
				, DoorL->GetPosition() + Vector3(280.f, 0.f, 0.f));
			spawner->SetScale(Vector3(160.f, 320.f, 1.f));
			spawner->GetTransform()->FixedUpdate();
			spawner->SetName(WIDE("Spawner-Boss1"));
			spawner->SetOwnerRoom(this);
			mGameObjects.push_back(spawner);
		}
	}

	Stage1BossRoom::~Stage1BossRoom()
	{
		mBoss = nullptr;
	}

	void Stage1BossRoom::Initialize()
	{
		RoomBase::Initialize();
	}
	void Stage1BossRoom::Update()
	{
		RoomBase::Update();
	}
	void Stage1BossRoom::FixedUpdate()
	{
		RoomBase::FixedUpdate();
	}
	void Stage1BossRoom::Render()
	{
		RoomBase::Render();
	}

	void Stage1BossRoom::Activate()
	{
		GameObject::Activate();

		LimitCameraSpace();

		size_t Size = mGameObjects.size();

		for (size_t iter = 0; iter < Size; ++iter)
		{
			eState objectState = mGameObjects[iter]->GetState();
			if (eState::NotActiveByRoom == objectState)
			{
				continue;
			}

			mGameObjects[iter]->Activate();
			//else if(eState::NotActiveByRoom == objectState)
			//{
			//}
			//else
			//{
			//	mGameObjects[iter]->Pause();
			//}
		}

		Size = mDoors.size();

		for (size_t iter = 0; iter < Size; ++iter)
		{
			if (mDoors[iter])
				mDoors[iter]->Activate();
		}
	}

	void Stage1BossRoom::AddObjectsPosXY(const Vector2& pos)
	{
		size_t Size = mGameObjects.size();

		for (size_t iter = 0; iter < Size; ++iter)
		{
			mGameObjects[iter]->AddPositionXY(pos);
		}
	}

	void Stage1BossRoom::SpawnBoss()
	{
		if (mBoss && !mbBossSpawned)
		{
			mBoss->Initialize();
		}
	}

}
