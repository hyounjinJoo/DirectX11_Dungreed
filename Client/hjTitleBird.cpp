#include "hjTitleBird.h"
#include "hjSpriteRenderer.h"
#include "hjResources.h"
#include "hjMaterial.h"
#include "hjTransform.h"
#include "hjApplication.h"
#include "hjTime.h"
#include "hjAnimator.h"
#include "hjAnimation.h"
#include "hjCollider2D.h"

extern hj::Application application;
namespace hj
{
	TitleBird::TitleBird()
		: GameObject()
	{
		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = MTRL_FIND("MTRL_Title_Object_Bird");
		std::shared_ptr<Mesh> mesh = MESH_FIND("Mesh_Rect");
		sr->SetMaterial(material);
		sr->SetMesh(mesh);

		Animator* animator = AddComponent<Animator>();
		if (animator)
		{
			CREATE_ANIM(birdIdleAnimSheet, birdIdleFrame, material->GetTexture()->GetTexSize(), 0.0625f);
			FRAME_ADD(birdIdleFrame, 240.f, 0.f, 48.f, 32.f,birdIdleAnimSheet);
			FRAME_ADD(birdIdleFrame, 48.f, 0.f, 48.f, 28.f,birdIdleAnimSheet);
			FRAME_ADD(birdIdleFrame, 96.f, 0.f, 48.f, 28.f,birdIdleAnimSheet);
			FRAME_ADD(birdIdleFrame, 288.f, 0.f, 48.f, 32.f,birdIdleAnimSheet);
			FRAME_ADD(birdIdleFrame, 336.f, 0.f, 48.f, 32.f,birdIdleAnimSheet);
			FRAME_ADD(birdIdleFrame, 144.f, 0.f, 48.f, 28.f,birdIdleAnimSheet);
			FRAME_ADD(birdIdleFrame, 0.f, 0.f, 48.f, 24.f,birdIdleAnimSheet);
			FRAME_ADD(birdIdleFrame, 192.f, 0.f, 48.f, 28.f,birdIdleAnimSheet);

			AUTO_OFFSET_CALC(birdIdleAnimSheet);

			bool isCreate = animator->Create(WIDE("Bird_Idle"), material->GetTexture(), birdIdleAnimSheet, canvasSize, false);

			if(isCreate)
			{
				animator->Play(L"Bird_Idle");
			}
		}

		Transform* tr = static_cast<Transform*>(mComponents[(UINT)eComponentType::Transform]);
		tr->SetScale(Vector3(48.f, 32.f, 1.f));

		AddComponent<Collider2D>();
	}
	
	TitleBird::~TitleBird()
	{
	}
	
	void TitleBird::Initialize()
	{
		GameObject::Initialize();
	}

	void TitleBird::Update()
	{
		GameObject::Update();
	}

	void TitleBird::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void TitleBird::Render()
	{
		GameObject::Render();
	}
}