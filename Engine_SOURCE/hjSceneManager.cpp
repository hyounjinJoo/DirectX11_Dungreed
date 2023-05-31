#include "pch.h"
#include "hjSceneManager.h"

namespace hj
{
	std::vector<Scene*> SceneManager::mScenes = {};
	Scene* SceneManager::mActiveScene = nullptr;
	Scene* SceneManager::mNextLoadScene = nullptr;
	

	void SceneManager::InitialResize()
	{
		mScenes.resize(static_cast<UINT>(eSceneType::End));
	}

	void SceneManager::Initialize()
	{
		//mActiveScene->Initialize();
		//mNextLoadScene = mScenes[(UINT)eSceneType::Test]; 
		mNextLoadScene = mScenes[(UINT)eSceneType::Title];
		LoadScene();
		mActiveScene->CreateDefaultCamera();
		//for (Scene* scene : mScenes)
		//{
		//	if(scene)
		//		scene->Initialize();
		//}
	}

	void SceneManager::Update()
	{
		if(mActiveScene)
			mActiveScene->Update();
	}

	void SceneManager::FixedUpdate()
	{
		if (mActiveScene)
			mActiveScene->FixedUpdate();
	}

	void SceneManager::Render()
	{
		if (mActiveScene)
			mActiveScene->Render();
	}

	void SceneManager::Destroy()
	{
		if (mActiveScene)
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

	void SceneManager::LoadScene()
	{
		if (mActiveScene)
			mActiveScene->OnExit();

		// 씬이 변경될 때, dontDestroy 오브젝트는 다음씬으로 같이 넘겨줘야 한다.
		std::vector<GameObject*> gameObjects;
		
		if (mActiveScene)
		{
			gameObjects = mActiveScene->GetDontDestroyGameObjects();
			mActiveScene->Destroy();
		}
		
		mActiveScene = mNextLoadScene;
		mNextLoadScene = nullptr;

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

	void SceneManager::NeedToLoad(eSceneType type)
	{
		if(mScenes[static_cast<UINT>(type)])
			mNextLoadScene = mScenes[static_cast<UINT>(type)];
	}

	bool SceneManager::IsNeedToLoadScene()
	{
		bool isNeedTo = false;

		if (mNextLoadScene)
		{
			isNeedTo = true;
		}

		return isNeedTo;
	}

}