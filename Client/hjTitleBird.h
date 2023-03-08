#pragma once
#include "hjGameObject.h"

namespace hj
{
	class MainBird :
		public GameObject
	{
	public:
		MainBird();
		virtual ~MainBird();


		void Initialize() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() override;

	private:
	};
}