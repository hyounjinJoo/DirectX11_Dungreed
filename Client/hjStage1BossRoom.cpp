#include "hjStage1BossRoom.h"
#include "hjMap.h"
#include "hjResources.h"
#include "hjCameraScript.h"
#include "hjObject.h"
#include "hjRoomNotPass.h"
#include "hjRoomPassThrough.h"
#include "hjRoomDoor.h"
#include "hjTestScene.h"
#include "hjFadeObject.h"

namespace hj
{
	Stage1BossRoom::Stage1BossRoom()
	{
		std::shared_ptr<Map> mapData = Resources::Find<Map>(WIDE("MAP_01_BossRoom"));
		if (mapData)
		{
			UINT layerCount = mapData->GetLayerCount();
			Vector4 mapLTRBlimit = Vector4::Zero;
			Vector2 mapSize = mapData->GetTileMapSize();
			Vector2 mapPos = GetPositionXY();
			mapLTRBlimit.x = mapPos.x - mapSize.x * 0.5f;
			mapLTRBlimit.y = mapPos.y + mapSize.y * 0.5f;
			mapLTRBlimit.z = mapPos.x + mapSize.x * 0.5f;
			mapLTRBlimit.w = mapPos.y - mapSize.y * 0.5f;

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
			}


			if (renderer::mainCamera)
			{
				std::vector<Script*> scripts = renderer::mainCamera->GetOwner()->GetScripts();
				for (auto script : scripts)
				{
					CameraScript* cameraScript = dynamic_cast<CameraScript*>(script);
					if (cameraScript)
					{
						cameraScript->SetLimitSpace(mapLTRBlimit);
					}
				}
			}

			RoomNotPass* floor = object::Instantiate<RoomNotPass>(eLayerType::ForeGround, mapPos + Vector3(0.f, -760.f, 0.f));
			floor->SetScale(Vector3(1760.f, 240.f, 1.f));
			RoomNotPass* notPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround, mapPos + Vector3(880.f, 280.f, 0.f));
			notPass->SetScale(Vector3(320.f, 1200.f, 1.f));

			notPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround, mapPos + Vector3(-880.f, 280.f, 0.f));
			notPass->SetScale(Vector3(320.f, 1200.f, 1.f));

			notPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround, mapPos + Vector3(0.f, 800.f, 0.f));
			notPass->SetScale(Vector3(1760.f, 160.f, 1.f));

			RoomPassThrough* passThrough = object::Instantiate<RoomPassThrough>(eLayerType::ForeGround, mapPos + Vector3(-1.f, -508.f, 0.f));
			passThrough->SetScale(Vector3(636.f, 56.f, 1.f));

			passThrough = object::Instantiate<RoomPassThrough>(eLayerType::ForeGround, mapPos + Vector3(-441.f, -267.f, 0.f));
			passThrough->SetScale(Vector3(236.f, 56.f, 1.f));

			passThrough = object::Instantiate<RoomPassThrough>(eLayerType::ForeGround, mapPos + Vector3(440.f, -267.f, 0.f));
			passThrough->SetScale(Vector3(236.f, 56.f, 1.f));

			passThrough = object::Instantiate<RoomPassThrough>(eLayerType::ForeGround, mapPos + Vector3(-521.f, -27.f, 0.f));
			passThrough->SetScale(Vector3(236.f, 56.f, 1.f));

			passThrough = object::Instantiate<RoomPassThrough>(eLayerType::ForeGround, mapPos + Vector3(518.f, -27.f, 0.f));
			passThrough->SetScale(Vector3(236.f, 56.f, 1.f));

			passThrough = object::Instantiate<RoomPassThrough>(eLayerType::ForeGround, mapPos + Vector3(-521.f, 213.f, 0.f));
			passThrough->SetScale(Vector3(236.f, 56.f, 1.f));

			passThrough = object::Instantiate<RoomPassThrough>(eLayerType::ForeGround, mapPos + Vector3(518.f, 213.f, 0.f));
			passThrough->SetScale(Vector3(236.f, 56.f, 1.f));

			passThrough = object::Instantiate<RoomPassThrough>(eLayerType::ForeGround, mapPos + Vector3(-1.f, 372.f, 0.f));
			passThrough->SetScale(Vector3(314.f, 56.f, 1.f));

			FadeObject* fadeObject = dynamic_cast<FadeObject*>(dynamic_cast<TestScene*>(SceneManager::GetActiveScene())->
				GetFadeObject());
			RoomDoor* testDoorL = object::Instantiate<RoomDoor>(eLayerType::ForeGround, Vector3(-840.f, -480.f, 0.f));
			testDoorL->GetTransform()->FixedUpdate();
			testDoorL->SetDoorPlaced(DoorPlaced::L);
			testDoorL->SetName(WIDE("LeftDoor"));
			testDoorL->SetFadeScript(fadeObject->GetFadeScript());

			RoomDoor* testDoorR = object::Instantiate<RoomDoor>(eLayerType::ForeGround, Vector3(840.f, -480.f, 0.f));
			testDoorR->GetTransform()->FixedUpdate();
			testDoorR->SetDoorPlaced(DoorPlaced::R);
			testDoorR->SetName(WIDE("RightDoor"));
			testDoorR->SetFadeScript(fadeObject->GetFadeScript());

			testDoorL->SetExitDoor(testDoorR);
			testDoorR->SetExitDoor(testDoorL);
		}
	}

	Stage1BossRoom::~Stage1BossRoom()
	{
	}

	void Stage1BossRoom::Initialize()
	{
		GameObject::Initialize();
	}
	void Stage1BossRoom::Update()
	{
		GameObject::Update();
	}
	void Stage1BossRoom::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}
	void Stage1BossRoom::Render()
	{
		GameObject::Render();
	}
}
