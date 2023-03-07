#pragma once
#include "hjScene.h"

namespace hj
{
	class DungeonScene :
		public Scene
	{
	public:
		DungeonScene();
		virtual ~DungeonScene();

		void Initialize() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() override;
		
		void OnEnter() override;
		void OnExit() override;

	private:
	};
}
