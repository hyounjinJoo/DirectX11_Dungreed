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
#include "hjXmlParser.h"

extern hj::Application application;
namespace hj
{
	TitleBird::TitleBird()
		: GameObject()
	{
		SetName(WIDE("Title Bird Obj"));

		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = MTRL_FIND("MTRL_Title_Object_Bird");
		std::shared_ptr<Mesh> mesh = MESH_FIND("Mesh_Rect");
		sr->SetMaterial(material);
		sr->SetMesh(mesh);

		Animator* animator = AddComponent<Animator>();
		if (animator)
		{
			XmlParser* parser = new XmlParser;
			std::wstring path = WIDE("01_Scene/00_TitleScene/TitleScene_01.xml");
			bool parseResult = parser->LoadFile(path);

			if (parseResult)
			{
				CREATE_ANIM(birdIdleAnimSheet, birdIdleFrame, material->GetTexture(eTextureSlot::T0)->GetTexSize(), 0.0625f);
				parseResult = parser->FindElem(WIDE("TextureAtlas"));
				parseResult = parser->IntoElem();
				Vector2 startPos = Vector2::Zero;
				Vector2 size = Vector2::Zero;

				for (int i = 0; i < 8; ++i)
				{
					parser->FindElem(WIDE("sprite"));
					startPos.x = static_cast<float>(parser->GetIntAttribute(WIDE("x")));
					startPos.y = static_cast<float>(parser->GetIntAttribute(WIDE("y")));
					size.x = static_cast<float>(parser->GetIntAttribute(WIDE("w")));
					size.y = static_cast<float>(parser->GetIntAttribute(WIDE("h")));
					FRAME_ADD(birdIdleFrame, startPos.x, startPos.y, size.x, size.y, birdIdleAnimSheet);
				}

				AUTO_OFFSET_CALC(birdIdleAnimSheet);

				bool isCreate = animator->Create(WIDE("Bird_Idle"), material->GetTexture(eTextureSlot::T0), birdIdleAnimSheet, canvasSize, false);

				if (isCreate)
				{
					animator->Play(L"Bird_Idle");
				}
			}
			delete parser;
		}

		Transform* tr = static_cast<Transform*>(mComponents[(UINT)eComponentType::Transform]);
		tr->SetScale(Vector3(60.f, 40.f, 1.f));
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