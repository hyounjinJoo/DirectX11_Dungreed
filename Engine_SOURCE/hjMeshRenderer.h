#pragma once

#include "hjComponent.h"
#include "hjMesh.h"
#include "hjMaterial.h"

using namespace hj::graphics;
namespace hj
{
	class MeshRenderer : public Component
	{
	public:
		MeshRenderer();
		virtual ~MeshRenderer();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void SetMesh(std::shared_ptr<Mesh> mesh) { mMesh = mesh; }
		void SetMaterial(std::shared_ptr<Material> material) { mMaterial = material; }

	private:
		std::shared_ptr<Mesh> mMesh;
		std::shared_ptr<Material> mMaterial;
	};
}