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
		const std::vector<GameObject*>& GetGameObejcts() { return mGameObjects; }

	private:
		std::vector<GameObject*> mGameObjects;
	};

	typedef const std::vector<GameObject*>& GameObjects;
}