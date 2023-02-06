#include "hjMeshRenderer.h"
#include "hjGameObject.h"
#include "hjTransform.h"

namespace hj
{
	MeshRenderer::MeshRenderer()
		: Component(eComponentType::Mesh)
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

		mShader->Binds();
		mMesh->BindBuffer();

		mMesh->Render();
	}
}