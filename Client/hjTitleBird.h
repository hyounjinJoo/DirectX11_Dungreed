#pragma once
#include "hjGameObject.h"

namespace hj
{
	class TitleBird :
		public GameObject
	{
	public:
		TitleBird();
		virtual ~TitleBird();


		void Initialize() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() override;

	private:
	};
}