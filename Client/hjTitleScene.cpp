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
#include "hjTitleBirdLeader.h"
#include "hjTitleBirdFollower.h"
#include "hjMainLogo.h"
#include "hjTitleGameStart.h"
#include "hjTitleOption.h"
#include "hjTitleExit.h"
#include "hjAudioListener.h"
#include "hjAudioClip.h"
#include "hjAudioSource.h"

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

		{
			GameObject* directionalLight = object::Instantiate<GameObject>(eLayerType::Player, Vector3(0.f, 0.f, -100.f));
			Light* lightComp = directionalLight->AddComponent<Light>();
			lightComp->SetType(eLightType::Directional);
			lightComp->SetDiffuse(Vector4(1.f, 1.f, 1.f, 1.f));
		}
		{
			if (nullptr != renderer::mainCamera)
			{
				Camera* mainCamera = renderer::mainCamera;
				mainCamera->GetOwner()->SetPositionXY(Vector2(0.f, 0.f));
				mainCamera->GetOwner()->AddComponent<AudioListener>();
			}
		}
	#pragma region Layer Object
		#pragma region Background
			std::shared_ptr<Material> material = MTRL_FIND_STR("MTRL_ColorRect");
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
			material = MTRL_FIND_STR("MTRL_Title_Layer_Cloud");
			std::shared_ptr<Texture> texture = material->GetTexture(eTextureSlot::T0);
			Vector2 texSize = texture->GetTexSize();
			scale = Vector3(texSize.x, texSize.y, 1.f);
			float ratio = height / texSize.y;
			scale *= ratio * 2.f;
			pos.y = -450.f;
			pos.z = 3.f;
			
			layerObj = object::Instantiate<LayerObject>(eLayerType::BackGround, pos, rot, scale);
			layerObj->GetComponent<SpriteRenderer>()->SetMaterial(material);
			layerObj->SetName(WIDE("Title_Layer_BackCloud"));
			layerObj->SetMove(true);
			layerObj->SetMoveSpeed(0.004f);
			layerObj->SetStartUV(Vector2(0.f, 0.f));
			layerObj->SetEndUV(Vector2(1.f, 0.5f));

		#pragma endregion
		#pragma region FrondCloud
			texture = material->GetTexture(eTextureSlot::T0);
			pos.z = 1.f;
			pos.y = 450.f;
			
			layerObj = object::Instantiate<LayerObject>(eLayerType::ForeGround, pos, rot, scale);
			layerObj->GetComponent<SpriteRenderer>()->SetMaterial(material);
			layerObj->SetName(WIDE("Title_Layer_FrontCloud"));
			layerObj->SetMove(true);
			layerObj->SetMoveSpeed(0.025f);
			layerObj->SetStartUV(Vector2(0.f, 0.5f));
			layerObj->SetEndUV(Vector2(1.f, 1.f));
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
	#pragma region Bird
			TitleBirdLeader* Bird = nullptr;
#define MAX_LEADER_BIRD 4
#define MAX_FOLLOWER_BIRD 4
#define DIST 5.f
#define DELAY_START_TIME 10.f
#define LOOP_INTERVAL (MAX_LEADER_BIRD * 3.f)

		#pragma region Normal Bird
			int followingRow = static_cast<int>(std::floorf(MAX_FOLLOWER_BIRD * 0.5f)) + 1;
			Bird = object::Instantiate<TitleBirdLeader>(eLayerType::MidGround);
			Bird->SetReservedDelayMoveStart(0.f);
			Bird->SetLoopInterval(LOOP_INTERVAL);
			const Vector2 birdScale = Bird->GetScaleXY();

			for (int iter = 1; iter < MAX_LEADER_BIRD; ++iter)
			{
				Bird = object::Instantiate<TitleBirdLeader>(eLayerType::MidGround);
				Bird->SetReservedDelayMoveStart(DELAY_START_TIME * iter);
				Bird->SetLoopInterval(LOOP_INTERVAL);
				Bird->AddEndPosX(followingRow * birdScale.x);
			}
		#pragma endregion
		#pragma region Triangle Squadron Birds
			float distForBirdY = birdScale.y - 5.f;;
			TitleBirdFollower* followerBird = nullptr;
			
			for (int iter = 0; iter < MAX_FOLLOWER_BIRD; ++iter)
			{
				followerBird = object::Instantiate<TitleBirdFollower>(eLayerType::MidGround);
				followerBird->SetLeader(Bird);

				static int factorTwice = 0;
				static int factorSign = 0;

				if (iter % 2 == 0)
					factorTwice = 1;
				else
					factorTwice = 2;

				if (static_cast<float>(iter) < std::floorf(MAX_FOLLOWER_BIRD * 0.5f))
					factorSign = 1;
				else
					factorSign = -1;

					followerBird->SetDistanceFromLeader(Vector2(-(birdScale.x + DIST * iter) * factorTwice, distForBirdY * factorTwice * factorSign - (DIST * iter)));
			}
		#pragma endregion
	#pragma endregion
#pragma region UI
			MainLogo* mainLogo = object::Instantiate<MainLogo>(eLayerType::UI);
			TitleGameStart* startButton = object::Instantiate<TitleGameStart>(eLayerType::UI);
			TitleExit* exitButton = object::Instantiate<TitleExit>(eLayerType::UI);
#pragma endregion
#pragma region Sound
			mTitleSoundObj = object::Instantiate<GameObject>(eLayerType::UI);
			std::shared_ptr<AudioClip> clip = Resources::Load<AudioClip>(WIDE("Title"), WIDE("title.mp3"));
			AudioSource* audioSrc = mTitleSoundObj->AddComponent<AudioSource>();
			clip->SetLoop(true);
			audioSrc->SetClip(clip);
			audioSrc->Play();
#pragma endregion
		Scene::Initialize();
	}

	void TitleScene::Update()
	{
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

		Initialize();
	}

	void TitleScene::OnExit()
	{
		mTitleSoundObj->GetComponent<AudioSource>()->Stop();

		Scene::OnExit();
	}
}