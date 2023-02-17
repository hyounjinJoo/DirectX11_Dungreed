#pragma once

#include "hjEntity.h"
#include "hjLayer.h"

namespace hj
{
	using namespace hj::enums;
	class Scene : public Entity
	{
	public:
		Scene();
		virtual ~Scene();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

		void AddGameObject(GameObject* gameObject, const eLayerType type);
		Layer& GetLayer(eLayerType type) { return mLayers[(UINT)type]; }

	private:
		std::vector<Layer> mLayers;
	};
}
