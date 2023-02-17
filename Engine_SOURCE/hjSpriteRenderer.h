#pragma once
#include "hjBaseRenderer.h"
#include "hjMesh.h"
#include "hjMaterial.h"

namespace hj
{
	class SpriteRenderer : public BaseRenderer
	{
	public:
		SpriteRenderer();
		virtual ~SpriteRenderer();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;
	};
}
