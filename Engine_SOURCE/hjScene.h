#pragma once

#include "hjEntity.h"
#include "hjLayer.h"

namespace hj
{
	class Scene : public Entity
	{
	public:
		Scene();
		virtual ~Scene();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	private:
		std::vector<Layer> mLayers;
	};
}
