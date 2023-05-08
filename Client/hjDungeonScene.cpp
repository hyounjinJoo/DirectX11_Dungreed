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
		//auto iter = mRooms.begin();
		//auto iterEnd = mRooms.end();
		//for (; iter != iterEnd;)
		//{
		//	if (*iter)
		//	{
		//		iter = mRooms.erase(iter);
		//		continue;
		//	}			
		//
		//	++iter;
		//}
	}

	void DungeonScene::Initialize()
	{
		Scene::Initialize();
	}

	void DungeonScene::Update()
	{
		Scene::Update();
		if (Input::GetKeyDown(eKeyCode::N))
		{
			SceneManager::NeedToLoad(eSceneType::Title);
		}
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

		Initialize();
	}

	void DungeonScene::OnExit()
	{
		Scene::OnExit();
	}
}