#pragma once
#include "hjScene.h"
#include "hjTitleBird.h"

namespace hj
{
	class TitleScene :
		public Scene
	{
	public:
		TitleScene();
		virtual ~TitleScene();

		void Initialize() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() override;

		void OnEnter() override;
		void OnExit() override;

	private:
		class GameObject* mTitleSoundObj;
	};
}