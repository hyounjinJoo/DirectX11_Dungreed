#include "hjSceneManager.h"

namespace hj
{
	std::vector<Scene*> SceneManager::mScenes = {};
	Scene* SceneManager::mActiveScene = nullptr;
	

	void SceneManager::InitialResize()
	{
		mScenes.resize(static_cast<UINT>(eSceneType::End));
	}

	void SceneManager::Initialize()
	{
		mActiveScene->Initialize();
		//for (Scene* scene : mScenes)
		//{
		//	if(scene)
		//		scene->Initialize();
		//}
	}

	void SceneManager::Update()
	{
		mActiveScene->Update();
	}

	void SceneManager::FixedUpdate()
	{
		mActiveScene->FixedUpdate();
	}

	void SceneManager::Render()
	{
		mActiveScene->Render();
	}

	void SceneManager::Destroy()
	{
		mActiveScene->Destroy();
	}

	void SceneManager::Release()
	{
		if (mActiveScene)
			mActiveScene->OnExit();

		mActiveScene = nullptr;

		for (Scene* scene : mScenes)
		{
			delete scene;
			scene = nullptr;
		}
	}

	void SceneManager::CreateScene(eSceneType type, Scene* scene)
	{
		if (mScenes.size() >= static_cast<UINT>(eSceneType::End) && mScenes[static_cast<UINT>(type)])
		{
			mScenes.push_back(scene);
		}

		mScenes[static_cast<UINT>(type)] = scene;
	}

	void SceneManager::LoadScene(eSceneType type)
	{
		if (mActiveScene)
			mActiveScene->OnExit();

		// 씬이 변경될 때, dontDestroy 오브젝트는 다음씬으로 같이 넘겨줘야 한다.
		std::vector<GameObject*> gameObjects;
		
		if(mActiveScene)
			gameObjects = mActiveScene->GetDontDestroyGameObjects();
		
		mActiveScene = mScenes[(UINT)type];

		if (gameObjects.empty() == false)
		{
			for (GameObject* object : gameObjects)
			{
				eLayerType type = object->GetLayerType();
				mActiveScene->AddGameObject(object, type);
			}
		}
		mActiveScene->OnEnter();
	}
}