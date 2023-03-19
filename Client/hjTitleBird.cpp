#include "hjTitleBird.h"
#include "hjSpriteRenderer.h"
#include "hjResources.h"
#include "hjMaterial.h"
#include "hjTransform.h"
#include "hjApplication.h"
#include "hjTime.h"
#include "hjAnimator.h"
#include "hjAnimation.h"

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
			std::vector<Animation::Sprite> frames;
			Animation::Sprite test = {};

			test.atlasSize = material->GetTexture()->GetTexSize();
			test.duration = 0.0625f;
			test.leftTop = Vector2(0.f, 0.f);
			test.offset = Vector2::Zero;
			test.size = Vector2(48.f, 32.f);

			frames.push_back(test);

			test.leftTop.x = 48.f; frames.push_back(test);
			test.leftTop.x = 96.f; frames.push_back(test);
			test.leftTop.x = 144.f; frames.push_back(test);
			test.leftTop.x = 192.f;	frames.push_back(test);
			test.leftTop.x = 240.f;	frames.push_back(test);
			test.leftTop.x = 288.f;	frames.push_back(test);
			test.leftTop.x = 336.f;	frames.push_back(test);

			bool isCreate = animator->Create(L"Bird_Idle", material->GetTexture(), frames, test.size, false);

			if(isCreate)
			{
				animator->Play(L"Bird_Idle");
			}
		}

		Transform* tr = static_cast<Transform*>(mComponents[(UINT)eComponentType::Transform]);
		tr->SetScale(Vector3(48.f, 32.f, 1.f));
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