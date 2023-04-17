#pragma once
#include "hjScene.h"

namespace hj
{
	class SceneManager
	{
	public:
		static void InitialResize();
		static void Initialize();
		static void Update();
		static void FixedUpdate();
		static void Render();
		static void Destroy();
		static void Release();

		static void CreateScene(eSceneType type, Scene* scene);
		static void LoadScene();
		static Scene* GetActiveScene() { return mActiveScene; }
		static void NeedToLoad(eSceneType type);
		static bool IsNeedToLoadScene();
	private:
		static std::vector<Scene*> mScenes;
		static Scene* mActiveScene;
		static Scene* mNextLoadScene;
	};
}