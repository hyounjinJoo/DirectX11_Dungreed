#include "hjSceneManager.h"
#include "hjTransform.h"
#include "hjMeshRenderer.h"

namespace hj
{
	void SceneManager::Initialize()
	{
		//mPlayScene = new Scene();
		//mPlayScene->Initialize();

		//GameObject* obj = new GameObject();
		//Transform* tr = new Transform();
		//obj->AddComponent(tr);

		//MeshRenderer* meshRenderer = new MeshRenderer();
		//obj->AddComponent(meshRenderer);

		//mPlayScene->AddGameObject(obj, eLayerType::None);
	}

	void SceneManager::Update()
	{
		mPlayScene->Update();
	}

	void SceneManager::FixedUpdate()
	{
		mPlayScene->FixedUpdate();
	}

	void SceneManager::Render()
	{
		mPlayScene->Render();
	}
}