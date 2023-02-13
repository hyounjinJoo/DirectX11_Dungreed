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

		void SetMesh(Mesh* mesh) { mMesh = mesh; }
		void SetMaterial(Material* material) { mMaterial = material; }

	private:
		Mesh* mMesh;
		Material* mMaterial;
	};
}