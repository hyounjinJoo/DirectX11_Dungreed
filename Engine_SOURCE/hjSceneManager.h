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
		static void LoadScene(eSceneType type);
		static Scene* GetActiveScene() { return mActiveScene; }
	private:
		static std::vector<Scene*> mScenes;
		static Scene* mActiveScene;
	};
}