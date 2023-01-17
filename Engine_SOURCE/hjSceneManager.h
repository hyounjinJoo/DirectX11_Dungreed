#pragma once
#include "hjScene.h"

namespace hj
{
	class SceneManager
	{
	public:
		static void Initialize();
		static void Update();
		static void FixedUpdate();
		static void Render();

	private:
		static Scene* mPlayScene;
	};
}