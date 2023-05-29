#pragma once

#include "hjBaseRenderer.h"
#include "hjMesh.h"
#include "hjMaterial.h"

using namespace hj::graphics;
namespace hj
{
	class MeshRenderer : public BaseRenderer
	{
	public:
		MeshRenderer();
		MeshRenderer(const MeshRenderer& meshRenderer);
		virtual ~MeshRenderer();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;
		virtual Component* Clone() const override;
	};
}