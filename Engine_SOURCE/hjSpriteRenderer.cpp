#include "hjSpriteRenderer.h"
#include "hjGameObject.h"
#include "hjTransform.h"
#include "hjAnimator.h"

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

		GetMesh()->BindBuffer();
		GetMaterial()->Bind();

		Animator* animator = GetOwner()->GetComponent<Animator>();
		if (animator)
		{
			animator->Binds();
		}

		GetMesh()->Render();
		GetMaterial()->Clear();
		
		if (animator)
		{
			animator->Clear();
		}
	}
}