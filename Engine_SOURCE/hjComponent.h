#pragma once

#include "hjEntity.h"

namespace hj
{
	class GameObject;
	class Component
	{
	public:
		enum class eType
		{
			None,
			Transform,
			Camera,

			// Render
			Mesh,
			SkyBox,
			Decal,

			Script,
			End,
		};

		Component();
		virtual ~Component();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	private:
		const eType mType;
		GameObject* mOwner;
	};
}