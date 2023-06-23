#include "hjTestScene.h"
#include "hjTransform.h"
#include "hjMeshRenderer.h"
#include "hjRenderer.h"
#include "hjResources.h"
#include "hjTexture.h"
#include "hjPlayerScript.h"
#include "hjCamera.h"
#include "hjCameraScript.h"
#include "hjSpriteRenderer.h"
#include "hjApplication.h"
#include "hjFadeScript.h"
#include "hjObject.h"
#include "hjInput.h"
#include "hjSceneManager.h"
#include "hjCollider2D.h"
#include "hjTestPlayer.h"
#include "hjTestMonster.h"
#include "hjCollisionManager.h"
#include "hjAnimator.h"
#include "hjPlayer.h"
#include "hjTime.h"
#include "hjLight.h"
#include "hjPaintShader.h"
#include "hjTileMap.h"
#include "hjXmlParser.h"
#include "hjMap.h"
#include "hjRoomRectFloor.h"
#include "hjRoomNotPass.h"
#include "hjParticleSystem.h"
#include "hjRoomPassThrough.h"
#include "hjRoomDoor.h"
#include "hjFadeObject.h"
#include "hjStage1BossRoom.h"
#include "hjStage1StartL.h"
#include "hjStage1StartLTR.h"
#include "hjStage1StartLR.h"
#include "hjStage1StartLB.h"
#include "hjStage1StartLTB.h"
#include "hjStage1StartLRB.h"
#include "hjStage1StartLTRB.h"
#include "hjStage1StartT.h"
#include "hjStage1StartTR.h"
#include "hjStage1StartTR2.h"
#include "hjStage1StartTB.h"
#include "hjStage1StartTRB.h"
#include "hjStage1StartR.h"
#include "hjStage1StartRB.h"
#include "hjStage1StartLT.h"
#include "hjStage1Boss.h"
#include "hjAudioClip.h"
#include "hjAudioSource.h"

extern hj::Application application;
namespace hj
{

	TestScene::TestScene()
		: Scene(eSceneType::Test)
		, mObj(nullptr)
		, mFadeObject(nullptr)
	{
	}

	TestScene::~TestScene()
	{
		mObj = nullptr;
		mFadeObject = nullptr;
	}

	void TestScene::Initialize()
	{
#pragma region Transform Initial Vector
		Vector3 pos = Vector3::Zero;
		Vector3 rot = Vector3::Zero;
		Vector3 scale = Vector3::One;
#pragma endregion
#pragma region Compute Shader Test
		{
			//paint shader
			//mPaintShader = Resources::Find<PaintShader>(L"Shader_Paint");
			//Paint Tex and Noise Tex
			//std::shared_ptr<Texture> paintTex = Resources::Find<Texture>(L"PaintTexture");
			//std::shared_ptr<Texture> noiseTex = Resources::Find<Texture>(L"NoiseTex01");
			//paintTex->SetTexSize(Vector2(1024.f, 1024.f));
			//mPaintShader->SetTarget(paintTex);
			//mPaintShader->SetNoiseTexture(noiseTex);
		}
#pragma endregion
#pragma region Light Test
		{
			{
				GameObject* directionalLight = object::Instantiate<GameObject>(eLayerType::Player, Vector3(0.f, 0.f, -100.f));
				directionalLight->SetName(WIDE("Dir Light"));
				Light* lightComp = directionalLight->AddComponent<Light>();
				lightComp->SetType(eLightType::Directional);
				lightComp->SetDiffuse(Vector4(1.f, 1.f, 1.f, 1.f));
			}
			{
				//GameObject* pointLight = object::Instantiate<GameObject>(eLayerType::Player, Vector3(0.f, 0.f, 0.f));
				//Light* lightComp = pointLight->AddComponent<Light>();
				//lightComp->SetType(eLightType::Point);
				//lightComp->SetRadius(500.f);
				//lightComp->SetDiffuse(Vector4(1.f, 0.f, 1.f, 1.f));
			}
		}
#pragma endregion
#pragma region Setting
	#pragma region Fade Object
		mFadeObject = object::Instantiate<FadeObject>(eLayerType::UI);

	#pragma endregion
#pragma endregion
#pragma region Objects
#pragma region Light Object
		//pos = Vector3::Zero;
		//rot = Vector3::Zero;
		//scale = Vector3(500.f, 500.f, 1.f);
		//GameObject* light = object::Instantiate<GameObject>(eLayerType::Player, pos, rot, scale);
		//
		//SpriteRenderer* sr = new SpriteRenderer();
		//light->AddComponent(sr);
		//
		//std::shared_ptr<Mesh> mesh = Resources::Find<Mesh>(L"RectMesh");
		//std::shared_ptr<Material> lightMaterial = Resources::Find<Material>(L"RectMaterial");
		//
		//sr->SetMaterial(lightMaterial);
		//sr->SetMesh(mesh);
		//
		//mActiveScene->AddGameObject(light, eLayerType::Player);
#pragma endregion
#pragma region Test Object(Transform / MeshRenderer / PlayerScript)
		//pos = Vector3(100.f, 100.f, 0.f);
		//rot = Vector3::Zero;
		////GameObject* obj = object::Instantiate<GameObject>(eLayerType::Monster, pos, rot, scale);
		//mObj = object::Instantiate<GameObject>(eLayerType::Monster, pos, rot, scale);
		//
		//mObj->SetName(L"Test Obj");
		//
		//MeshRenderer* mr = new MeshRenderer();
		//std::shared_ptr<Material> material = MTRL_FIND("MTRL_Sprite");
		//std::shared_ptr<Mesh> mesh = MESH_FIND("Mesh_Rect");
		//mr->SetMaterial(material);
		//mr->SetMesh(mesh);
		//
		//Vector2 texSize = material->GetTexture()->GetTexSize();
		//mObj->GetComponent<Transform>()->SetScale(Vector3(texSize.x, texSize.y, 1.f));
		////Vector2 texSize = Vector2(500.f, 500.f);
		//mObj->SetScale(Vector3(texSize.x, texSize.y, 1.f));
		//mObj->AddComponent(mr);
		//
		//Collider2D* collider = mObj->AddComponent<Collider2D>();
		//collider->SetType(eColliderType::Rect);

#pragma endregion
#pragma region Collision Test
#pragma region Collision Object - 1
		pos = Vector3(0.f, 0.f, 0.f);
		/*TestPlayer* testPlayer = object::Instantiate<TestPlayer>(eLayerType::Player, pos, rot, scale);
		testPlayer->SetName(L"Test Player for Collision");

		SpriteRenderer* sr = testPlayer->AddComponent<SpriteRenderer>();
		material = MTRL_FIND("MTRL_Char_Adventurer");
		sr->SetMaterial(material);
		sr->SetMesh(mesh);

		texSize = material->GetTexture()->GetTexSize();
		texSize *= 4.f;
		testPlayer->GetComponent<Transform>()->SetScale(Vector3(texSize.x, texSize.y, 1.f));

		collider = testPlayer->AddComponent<Collider2D>();
		collider->SetType(eColliderType::Rect);

		PlayerScript* playerScript = new PlayerScript();
		testPlayer->AddComponent(playerScript);*/
		SpriteRenderer* sr = nullptr;
		
		Player* player = object::Instantiate<Player>(eLayerType::Player, pos);
		player->SetName(L"Player");
		//Vector2 testScale = player->GetScaleXY();
		//player->SetScaleXY(testScale * 1.25f);
#pragma endregion
#pragma region Collision Object - 2
		//pos = Vector3(100.f, 100.f, 1.f);
		//TestMonster* testMonster = object::Instantiate<TestMonster>(eLayerType::Monster, pos, rot, scale);
		//testMonster->SetName(L"Test Monster for Collision");
		//
		//sr = testMonster->AddComponent<SpriteRenderer>();
		//material = MTRL_FIND("MTRL_Weapon_Legendary_DemonSword");
		//sr->SetMaterial(material);
		//sr->SetMesh(mesh);
		//
		//texSize = material->GetTexture()->GetTexSize();
		//texSize *= 4.f;
		//testMonster->GetComponent<Transform>()->SetScale(Vector3(texSize.x, texSize.y, 1.f));
		//testMonster->GetTransform()->SetParent(player->GetTransform());
		////testMonster->GetTransform()->SetInheritParentPosition(true);
		//
		//collider = testMonster->AddComponent<Collider2D>();
		//collider->SetType(eColliderType::Circle);
		//collider->SetSize(texSize);
#pragma endregion
		//mObj->GetTransform()->SetParent(testMonster->GetTransform());
#pragma endregion
#pragma region Test Transform Inherit Object
		//pos = Vector3(0.5f, 0.5f, 0.f);
		//rot = Vector3::Zero;
		//scale = Vector3::One;
		//
		//Transform* parentTR = obj->GetComponent<Transform>();
		//obj = object::Instantiate<GameObject>(eLayerType::Player, parentTR, pos, rot, scale);
		//obj->SetName(L"Test Obj - Transform Inherit");
		//
		//Transform* childTR = player->GetTransform();
		//childTR->SetInheritParentPosition(true);
		////childTR->SetInheritParentTransform(true);
		//
		//mr = new MeshRenderer();
		//obj->AddComponent(mr);
		//
		//material = MTRL_FIND("MTRL_Sprite");
		//mr->SetMaterial(material);
		//mr->SetMesh(mesh);
#pragma endregion
#pragma endregion
#pragma region UI
#pragma region Get Window Size
		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);

		float width = static_cast<float>(winRect.right - winRect.left);
		float height = static_cast<float>(winRect.bottom - winRect.top);
		Vector3 originLT = Vector3(0.f - width / 2.f, height / 2.f, 0.f);
#pragma endregion
#pragma endregion
		{
			RoomBase* startRoom = object::Instantiate<Stage1StartR>(eLayerType::ForeGround);
			startRoom->SettingFadeObject();
			startRoom->SettingDoorOwner();
			startRoom->Pause();

			RoomBase* roomLT = object::Instantiate<Stage1StartLT>(eLayerType::ForeGround);
			roomLT->SettingFadeObject();
			roomLT->SettingDoorOwner();
			roomLT->Pause();

			startRoom->ConnectDoor(DoorPlaced::R, roomLT->GetDoor(DoorPlaced::L));
			roomLT->ConnectDoor(DoorPlaced::L, startRoom->GetDoor(DoorPlaced::R));

			RoomBase* roomTB = object::Instantiate<Stage1StartTB>(eLayerType::ForeGround);
			roomTB->SettingFadeObject();
			roomTB->SettingDoorOwner();
			roomTB->Pause();

			roomLT->ConnectDoor(DoorPlaced::T, roomTB->GetDoor(DoorPlaced::B));
			roomTB->ConnectDoor(DoorPlaced::B, roomLT->GetDoor(DoorPlaced::T));
			
			RoomBase* roomRB = object::Instantiate<Stage1StartRB>(eLayerType::ForeGround);
			roomRB->SettingFadeObject();
			roomRB->SettingDoorOwner();
			roomRB->Pause();

			roomTB->ConnectDoor(DoorPlaced::T, roomRB->GetDoor(DoorPlaced::B));
			roomRB->ConnectDoor(DoorPlaced::B, roomTB->GetDoor(DoorPlaced::T));

			RoomBase* roomLTRB = object::Instantiate<Stage1StartLTRB>(eLayerType::ForeGround);
			roomLTRB->SettingFadeObject();
			roomLTRB->SettingDoorOwner();
			roomLTRB->Pause();

			roomRB->ConnectDoor(DoorPlaced::R, roomLTRB->GetDoor(DoorPlaced::L));
			roomLTRB->ConnectDoor(DoorPlaced::L, roomRB->GetDoor(DoorPlaced::R));

			RoomBase* roomBoss1 = object::Instantiate<Stage1BossRoom>(eLayerType::ForeGround);
			roomBoss1->SettingFadeObject();
			roomBoss1->SettingDoorOwner();
			roomBoss1->Pause();

			roomLTRB->ConnectDoor(DoorPlaced::R, roomBoss1->GetDoor(DoorPlaced::L));
			roomBoss1->ConnectDoor(DoorPlaced::L, roomLTRB->GetDoor(DoorPlaced::R));

			startRoom->Activate();
		}

	
		CollisionManager::CollisionLayerCheck(eLayerType::Player, eLayerType::ForeGround, true);
		CollisionManager::CollisionLayerCheck(eLayerType::Player, eLayerType::Monster, true);
		
		CollisionManager::CollisionLayerCheck(eLayerType::Player, eLayerType::MonsterAttack_ForeGround, true);
		CollisionManager::CollisionLayerCheck(eLayerType::Player, eLayerType::MonsterAttack_NotForeGround, true);

		CollisionManager::CollisionLayerCheck(eLayerType::Monster, eLayerType::ForeGround, true);

		CollisionManager::CollisionLayerCheck(eLayerType::ForeGround, eLayerType::MonsterAttack_ForeGround, true);
		CollisionManager::CollisionLayerCheck(eLayerType::ForeGround, eLayerType::PlayerAttack_ForeGround, true);
		CollisionManager::CollisionLayerCheck(eLayerType::ForeGround, eLayerType::MonsterHas, true);

		CollisionManager::CollisionLayerCheck(eLayerType::PlayerAttack_ForeGround, eLayerType::Monster, true);
		CollisionManager::CollisionLayerCheck(eLayerType::PlayerAttack_NotForeGround, eLayerType::Monster, true);

#pragma region Sound
		mTestSoundObj = object::Instantiate<GameObject>(eLayerType::UI);
		std::shared_ptr<AudioClip> clip = Resources::Load<AudioClip>(WIDE("Field"), WIDE("1.JailField.mp3"));
		AudioSource* audioSrc = mTestSoundObj->AddComponent<AudioSource>();
		clip->SetLoop(true);
		audioSrc->SetClip(clip);
		audioSrc->Play();
#pragma endregion
		Scene::Initialize();
	}

	void TestScene::Update()
	{
		Scene::Update();
		static const bool bCameraTransformInit = (renderer::mainCamera != nullptr 
											&& renderer::mainCamera->GetOwner() != nullptr 
											&& renderer::mainCamera->GetOwner()->GetTransform() != nullptr 
											&& renderer::mainCamera->GetOwner()->GetTransform()->GetParent() != nullptr);


		if (!bCameraTransformInit)
		{
			std::vector<GameObject*> playerLayerObjs = GetGameObjects(eLayerType::Player);

			for (GameObject* iter : playerLayerObjs)
			{
				if (dynamic_cast<Player*>(iter))
				{
					renderer::mainCamera->GetOwner()->GetTransform()->SetParent(iter->GetTransform());
				}
			}
		}

		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::NeedToLoad(eSceneType::Title);
		}
		if (Input::GetKeyDown(eKeyCode::I))
		{
			Camera* camera = renderer::mainCamera;
			GameObject* cameraObj = camera->GetOwner();
			std::vector<Script*> scripts = cameraObj->GetScripts();

			for (auto iter : scripts)
			{
				if (dynamic_cast<CameraScript*>(iter))
				{
					CameraScript* script = static_cast<CameraScript*>(iter);
					script->Shake();
				}
			}
		}

		//if (Input::GetKeyPressed(eKeyCode::I))
		//{
		//	mObj->AddPositionY(100.f * Time::DeltaTime());
		//}
		//if (Input::GetKeyPressed(eKeyCode::K))
		//{
		//	mObj->AddPositionY(-100.f * Time::DeltaTime());
		//}
		//if (Input::GetKeyPressed(eKeyCode::J))
		//{
		//	mObj->AddPositionX(-100.f * Time::DeltaTime());
		//}
		//if (Input::GetKeyPressed(eKeyCode::L))
		//{
		//	mObj->AddPositionX(100.f * Time::DeltaTime());
		//}
		//if (Input::GetKeyPressed(eKeyCode::U))
		//{
		//	mObj->AddRotationZ(10.f * Time::DeltaTime());
		//}
		//if (Input::GetKeyPressed(eKeyCode::O))
		//{
		//	mObj->AddRotationZ(-10.f * Time::DeltaTime());
		//}
	}

	void TestScene::FixedUpdate()
	{
		Scene::FixedUpdate();
		//mPaintShader->OnExecute();
	}

	void TestScene::Render()
	{
		Scene::Render();
	}

	void TestScene::OnEnter()
	{
		Scene::OnEnter();

		Initialize();
	}

	void TestScene::OnExit()
	{
		mTestSoundObj->GetComponent<AudioSource>()->Stop();
		Scene::OnExit();
	}

	void TestScene::PlayBossBgm()
	{
		std::shared_ptr<AudioClip> clip = Resources::Load<AudioClip>(WIDE("Boss"), WIDE("1.JailBoss.mp3"));
		AudioSource* audioSrc = mTestSoundObj->GetComponent<AudioSource>();
		clip->SetLoop(true);
		audioSrc->Stop();
		audioSrc->SetClip(clip);
		audioSrc->Play();
	}

	void TestScene::PlayFieldBgm()
	{
		std::shared_ptr<AudioClip> clip = Resources::Load<AudioClip>(WIDE("Field"), WIDE("1.JailField.mp3"));
		AudioSource* audioSrc = mTestSoundObj->GetComponent<AudioSource>();
		clip->SetLoop(true);
		audioSrc->Stop();
		audioSrc->SetClip(clip);
		audioSrc->Play();
	}

	void TestScene::StopBgm()
	{
		AudioSource* audioSrc = mTestSoundObj->GetComponent<AudioSource>();
		audioSrc->Stop();
	}

}