#pragma once

#include "hjEntity.h"
#include "hjLayer.h"

namespace hj
{
	using namespace hj::enums;
	class Scene : public Entity
	{
	public:
		Scene(eSceneType type);
		virtual ~Scene();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();
		virtual void Destroy();

		virtual void OnEnter();
		virtual void OnExit();

		eSceneType GetSceneType() { return mSceneType; }
		void AddGameObject(GameObject* gameObject, const eLayerType type);
		Layer& GetLayer(eLayerType type) { return mLayers[(UINT)type]; }
		std::vector<GameObject*> GetDontDestroyGameObjects();
		const std::vector<GameObject*>& GetGameObjects(const eLayerType type);

	private:
		std::vector<Layer> mLayers;
		eSceneType mSceneType;
	};
}
