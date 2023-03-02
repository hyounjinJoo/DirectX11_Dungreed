#include "hjSceneManager.h"
#include "hjTitleScene.h"
#include "hjDungeonScene.h"

namespace hj
{
	std::vector<Scene*> SceneManager::mScenes = {};
	Scene* SceneManager::mActiveScene = nullptr;

	void SceneManager::Initialize()
	{
		mScenes.resize((UINT)eSceneType::End);

		mScenes[(UINT)eSceneType::Title] = new TitleScene();
		mScenes[(UINT)eSceneType::Dungeon] = new DungeonScene();

		mActiveScene = mScenes[(UINT)eSceneType::Title];

		for (Scene* scene : mScenes)
		{
			if(scene)
				scene->Initialize();
		}
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

	void SceneManager::LoadScene(eSceneType type)
	{
		if (mActiveScene)
			mActiveScene->OnExit();

		mActiveScene = mScenes[(UINT)type];

		// ���� ����� ��, dontDestroy ������Ʈ�� ���������� ���� �Ѱ���� �Ѵ�.
		std::vector<GameObject*> gameObjects
			= mActiveScene->GetDontDestroyGameObjects();

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