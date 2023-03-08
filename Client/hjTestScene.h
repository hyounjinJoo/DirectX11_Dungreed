#pragma once
#include <hjScene.h>

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

	private:
    };
}
