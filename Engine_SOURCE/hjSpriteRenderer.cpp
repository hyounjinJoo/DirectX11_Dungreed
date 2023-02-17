#include "hjSpriteRenderer.h"
#include "hjGameObject.h"
#include "hjTransform.h"

namespace hj
{
	SpriteRenderer::SpriteRenderer()
		: BaseRenderer(eComponentType::SpriteRenderer)
	{
	}

	SpriteRenderer::~SpriteRenderer()
	{
	}
	
	void SpriteRenderer::Initialize()
	{
	}
	
	void SpriteRenderer::Update()
	{
	}
	
	void SpriteRenderer::FixedUpdate()
	{
	}
	
	void SpriteRenderer::Render()
	{
		GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

		GetMaterial()->Bind();
		GetMesh()->BindBuffer();

		GetMesh()->Render();

		GetMaterial()->Clear();
	}
}