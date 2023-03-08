#include "hjDungeonScene.h"
#include "hjSceneManager.h"
#include "hjGameObject.h"
#include "hjObject.h"
#include "hjCamera.h"
#include "hjCameraScript.h"
#include "hjInput.h"

namespace hj
{

	DungeonScene::DungeonScene()
		: Scene(eSceneType::Dungeon)
	{
	}

	DungeonScene::~DungeonScene()
	{
	}

	void DungeonScene::Initialize()
	{
		//Vector3 pos = Vector3::Zero;
		//Vector3 rot = Vector3::Zero;
		//Vector3 scale = Vector3::One;
		//
		//GameObject* cameraObj = object::Instantiate<GameObject>(eLayerType::Camera, this);
		//
		//Camera* cameraComp = cameraObj->AddComponent<Camera>();
		//cameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		//cameraComp->TurnLayerMask(eLayerType::UI, false);
		//
		//cameraObj->AddComponent(new CameraScript());

		Scene::Initialize();
	}

	void DungeonScene::Update()
	{
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::LoadScene(eSceneType::Title);
		}

		Scene::Update();
	}

	void DungeonScene::FixedUpdate()
	{
		Scene::FixedUpdate();
	}

	void DungeonScene::Render()
	{
		Scene::Render();
	}

	void DungeonScene::OnEnter()
	{
		Scene::OnEnter();
	}

	void DungeonScene::OnExit()
	{
		Scene::OnExit();
	}
}