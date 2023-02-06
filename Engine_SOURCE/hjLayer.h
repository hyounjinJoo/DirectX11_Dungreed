#pragma once

#include "hjGameObject.h"

namespace hj
{
	class Layer : public Entity
	{
	public:
		Layer();
		virtual ~Layer();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

		void AddGameObject(GameObject* gameObject);

	private:
		std::vector<GameObject*> mGameObjects;
	};
}