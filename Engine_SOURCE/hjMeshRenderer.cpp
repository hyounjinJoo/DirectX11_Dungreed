#include "hjMeshRenderer.h"
#include "hjGameObject.h"
#include "hjTransform.h"

namespace hj
{
	MeshRenderer::MeshRenderer()
		: BaseRenderer(eComponentType::MeshRenderer)
	{
	}
	MeshRenderer::~MeshRenderer()
	{
	}
	void MeshRenderer::Initialize()
	{
	}
	void MeshRenderer::Update()
	{
	}
	void MeshRenderer::FixedUpdate()
	{
	}
	void MeshRenderer::Render()
	{
		GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

		GetMaterial()->Bind();
		GetMesh()->BindBuffer();

		GetMesh()->Render();

		GetMaterial()->Clear();
	}
}