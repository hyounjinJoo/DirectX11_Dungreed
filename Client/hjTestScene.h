#pragma once
#include <hjScene.h>
#include "hjPaintShader.h"

namespace hj
{
    class TestScene :
        public Scene
	{
	public:
		TestScene();
		virtual ~TestScene();

		void Initialize() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() override;

		void OnEnter() override;
		void OnExit() override;
		class GameObject* GetFadeObject() { return mFadeObject; }

	private:
		class GameObject* mObj;
		GameObject* mFadeObject;
		std::shared_ptr<graphics::PaintShader> mPaintShader;
    };
}
