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
#include "hjLayerObject.h"

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
#pragma region Get Window Size
		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);

		float width = static_cast<float>(winRect.right - winRect.left);
		float height = static_cast<float>(winRect.bottom - winRect.top);
		Vector3 originLT = Vector3(0.f - width / 2.f, height / 2.f, 0.f);
#pragma endregion

#pragma region Setting
	#pragma region Layer Object
		#pragma region Background
			std::shared_ptr<Material> material = MTRL_FIND("MTRL_ColorRect");
			scale = Vector3(width, height, 1.f);
			int colorRed = 121;
			int colorGreen = 186;
			int colorBlue = 255;
			int colorAlpha = 255;
		
			material->SetData(eGPUParam::Int_1, &colorRed);
			material->SetData(eGPUParam::Int_2, &colorGreen);
			material->SetData(eGPUParam::Int_3, &colorBlue);
			material->SetData(eGPUParam::Int_4, &colorAlpha);
			pos.z = 5.f;
		
			LayerObject* layerObj = object::Instantiate<LayerObject>(eLayerType::BackGround, pos, rot, scale);
			layerObj->GetComponent<SpriteRenderer>()->SetMaterial(material);
			layerObj->SetName(WIDE("Title_BackGround"));
			layerObj->SetMove(false);
		#pragma endregion
		#pragma region BackCloud
			material = MTRL_FIND("MTRL_Title_Layer_BackCloud");
			std::shared_ptr<Texture> texture = material->GetTexture();
			Vector2 texSize = texture->GetTexSize();
			scale = Vector3(texSize.x, texSize.y, 1.f);
			pos.z = 3.f;
			
			layerObj = object::Instantiate<LayerObject>(eLayerType::BackGround, pos, rot, scale);
			layerObj->GetComponent<SpriteRenderer>()->SetMaterial(material);
			layerObj->SetName(WIDE("Title_BackCloud"));
			layerObj->SetMove(true);
			layerObj->SetMoveSpeed(0.004f);
		#pragma endregion
		#pragma region FrondCloud
			material = MTRL_FIND("MTRL_Title_Layer_FrontCloud");
			texture = material->GetTexture();
			texSize = texture->GetTexSize();
			scale = Vector3(texSize.x, texSize.y, 1.f);
			pos.z = 1.f;
		
			layerObj = object::Instantiate<LayerObject>(eLayerType::ForeGround, pos, rot, scale);
			layerObj->GetComponent<SpriteRenderer>()->SetMaterial(material);
			layerObj->SetName(WIDE("Title_Layer_FrontCloud"));
			layerObj->SetMove(true);
			layerObj->SetMoveSpeed(0.025f);
		#pragma endregion
	#pragma endregion

		//material = MTRL_FIND("MTRL_Title_Layer_Sky_Day");
		//texture = material->GetTexture();
		//texSize = texture->GetTexSize();
		//texSize *= layerRatio;
		//scale = Vector3(texSize.x, texSize.y, 1.f);
		//pos.z = 5.f;
		//layerObj = object::Instantiate<LayerObject>(eLayerType::BackGround, pos, rot, scale);
		//layerObj->GetComponent<SpriteRenderer>()->SetMaterial(material);
		//layerObj->SetName(WIDE("Title_Layer_Sky_Day"));

		//material = MTRL_FIND("MTRL_Title_Layer_Sky_Night");
		//texture = material->GetTexture();
		//texSize = texture->GetTexSize();
		//texSize *= 4.f;
		//layerObj = object::Instantiate<LayerObject>(eLayerType::BackGround, pos);
		//layerObj->GetComponent<SpriteRenderer>()->SetMaterial(material);
		//layerObj->SetName(WIDE("Title_BackCloud"));


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
			//GameObject* cameraObj = object::Instantiate<GameObject>(eLayerType::Camera, pos);
			//
			//Camera* cameraComp = new Camera();
			//cameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
			////cameraComp->RegisterCameraInRenderer();
			//cameraComp->TurnLayerMask(eLayerType::UI, false);
			//cameraObj->AddComponent(cameraComp);
			//mainCamera = cameraComp;
			//
			////cameraObj->AddComponent(new CameraScript());
			//object::DontDestroyOnLoad(cameraObj);
	#pragma endregion
	#pragma region UI Camera
			//pos = Vector3::Zero;
			//rot = Vector3::Zero;
			//scale = Vector3::One;
			//
			//GameObject* cameraUIObj = object::Instantiate<GameObject>(eLayerType::Camera, pos);
			//
			//Camera* cameraUIComp = new Camera();
			//cameraUIComp->SetProjectionType(Camera::eProjectionType::Orthographic);
			//cameraUIComp->DisableLayerMask();
			//cameraUIComp->TurnLayerMask(eLayerType::UI, true);
			//cameraUIObj->AddComponent(cameraUIComp);
	#pragma endregion
#pragma endregion
#pragma region Objects
	
#pragma endregion
#pragma region UI
	
#pragma endregion
		Scene::Initialize();
	}

	void TitleScene::Update()
	{
		Scene::Update();
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(eSceneType::Test);
		}
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

		Initialize();
	}

	void TitleScene::OnExit()
	{
		Scene::OnExit();
	}
}