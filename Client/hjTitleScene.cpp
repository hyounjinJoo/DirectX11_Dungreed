#include "hjTitleScene.h"
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

extern hj::Application application;

namespace hj
{
	TitleScene::TitleScene()
		: Scene(eSceneType::Title)
	{
	}

	TitleScene::~TitleScene()
	{
	}

	void TitleScene::Initialize()
	{
#pragma region Transform Initial Vector
		Vector3 pos = Vector3::Zero;
		Vector3 rot = Vector3::Zero;
		Vector3 scale = Vector3::One;
#pragma endregion

#pragma region Setting
	#pragma region Fade Object
			//scale = Vector3(1600.f, 900.f, 1.f);
			//
			//GameObject* fadeObj = object::Instantiate<GameObject>(eLayerType::UI
			//													, pos
			//													, rot
			//													, scale);
			//		
			//fadeObj->AddComponent(new FadeScript());
			//
			//MeshRenderer* fadeMR = new MeshRenderer();
			//fadeMR->SetMesh(Resources::Find<Mesh>(L"RectMesh"));
			//fadeMR->SetMaterial(Resources::Find<Material>(L"FadeMaterial"));
			//fadeObj->AddComponent(fadeMR);
	#pragma endregion
	#pragma region Main Camera
			pos = Vector3::Zero;
			rot = Vector3::Zero;
			scale = Vector3::One;

			GameObject* cameraObj = object::Instantiate<GameObject>(eLayerType::Camera, pos);

			Camera* cameraComp = new Camera();
			cameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
			//cameraComp->RegisterCameraInRenderer();
			cameraComp->TurnLayerMask(eLayerType::UI, false);
			cameraObj->AddComponent(cameraComp);
			mainCamera = cameraComp;

			//cameraObj->AddComponent(new CameraScript());
			object::DontDestroyOnLoad(cameraObj);
	#pragma endregion
	#pragma region UI Camera
			pos = Vector3::Zero;
			rot = Vector3::Zero;
			scale = Vector3::One;

			GameObject* cameraUIObj = object::Instantiate<GameObject>(eLayerType::Camera, pos);

			Camera* cameraUIComp = new Camera();
			cameraUIComp->SetProjectionType(Camera::eProjectionType::Orthographic);
			cameraUIComp->DisableLayerMask();
			cameraUIComp->TurnLayerMask(eLayerType::UI, true);
			cameraUIObj->AddComponent(cameraUIComp);
	#pragma endregion
#pragma endregion
#pragma region Objects
	#pragma region Test Object(Transform / MeshRenderer / PlayerScript)
			pos = Vector3(-400.f, -200.f, 0.f);
			rot = Vector3::Zero;
			GameObject* obj = object::Instantiate<GameObject>(eLayerType::Monster, pos, rot, scale);
			obj->SetName(L"Test Obj");
	
			MeshRenderer* mr = new MeshRenderer();
			std::shared_ptr<Material> material = MTRL_FIND("MTRL_Sprite");
			std::shared_ptr<Mesh> mesh = MESH_FIND("Mesh_Rect");
			mr->SetMaterial(material);
			mr->SetMesh(mesh);
	
			Vector2 texSize = material->GetTexture()->GetTexSize();
			obj->GetComponent<Transform>()->SetScale(Vector3(texSize.x, texSize.y, 1.f));
	
			obj->AddComponent(mr);
	
			Collider2D* collider = obj->AddComponent<Collider2D>();
			collider->SetType(eColliderType::Rect);
			
			object::DontDestroyOnLoad(obj);
	
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
	#pragma region HPBar Base
			pos = Vector3::Zero;
			rot = Vector3::Zero;
			scale = Vector3(74.f * 4.f, 16.f * 4.f, 1.f);
			pos = originLT + Vector3(scale.x / 2.f + 20.f, scale.y / -2.f - 20.f, 1.f);
	
			GameObject* hpBarBase = object::Instantiate<GameObject>(eLayerType::UI, pos, rot, scale);
			hpBarBase->SetName(L"HPBarBase");
	
			SpriteRenderer* hpBarBaseSR = new SpriteRenderer();
			hpBarBase->AddComponent(hpBarBaseSR);
	
			std::shared_ptr<Mesh> hpBarBaseMesh = MESH_FIND("Mesh_Rect");
			std::shared_ptr<Material> hpBarBaseMaterial = MTRL_FIND("MTRL_UI");
			hpBarBaseSR->SetMesh(hpBarBaseMesh);
			hpBarBaseSR->SetMaterial(hpBarBaseMaterial);
	#pragma endregion
#pragma endregion

		CollisionManager::CollisionLayerCheck(eLayerType::Player, eLayerType::Monster, true);
		Scene::Initialize();
	}

	void TitleScene::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(eSceneType::Dungeon);
		}

		Scene::Update();
	}

	void TitleScene::FixedUpdate()
	{
		Scene::FixedUpdate();
	}

	void TitleScene::Render()
	{
		Scene::Render();
	}

	void TitleScene::OnEnter()
	{
		Scene::OnEnter();
	}

	void TitleScene::OnExit()
	{
		Scene::OnExit();
	}
}