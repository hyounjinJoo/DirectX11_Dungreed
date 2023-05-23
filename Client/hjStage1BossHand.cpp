#include "hjStage1BossHand.h"
#include "hjStage1Boss.h"
#include "hjSpriteRenderer.h"
#include "hjAnimation.h"
#include "hjAnimator.h"
#include "hjResources.h"
#include "hjXmlParser.h"

namespace hj
{
	Stage1BossHand::Stage1BossHand()
		: mOwner(nullptr)
		, mHandType(Boss1HandType::End)
		, mHandState(Boss1HandState::End)
	{
		SetName(WIDE("Boss_Bellial_Hand"));

		// 1. Sprite Renderer 积己
		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = MTRL_FIND("MTRL_Monster_Boss_Bellial");
		std::shared_ptr<Mesh> mesh = MESH_FIND("Mesh_Rect");
		sr->SetMaterial(material);
		sr->SetMesh(mesh);

		// 2. Body Animator 积己 棺 Animation 眠啊
		Animator* animator = AddComponent<Animator>();
		if (material && animator)
		{
			std::shared_ptr<Texture> texture = material->GetTexture(eTextureSlot::T0);
			if (texture)
			{
				CreateAnimation();
			}
		}
	}

	Stage1BossHand::~Stage1BossHand()
	{
		if (mOwner)
			mOwner = nullptr;
	}
	
	void Stage1BossHand::Initialize()
	{
		GameObject::Initialize();
	}
	
	void Stage1BossHand::Update()
	{
		GameObject::Update();
	}
	
	void Stage1BossHand::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}
	
	void Stage1BossHand::Render()
	{
		GameObject::Render();
	}
	
	void Stage1BossHand::SetOwner(Stage1Boss* owner)
	{
		if (!owner)
		{
			return;
		}

		if (!dynamic_cast<Stage1Boss*>(owner))
		{
			return;
		}

		mOwner = owner;
	}

	void Stage1BossHand::ChangeHandState(Boss1HandState nextState)
	{
		if (mHandState == nextState)
			return;

		Animator* animator = GetComponent<Animator>();
		if (!animator)
			return;

		switch (nextState)
		{
		case hj::Boss1HandState::Idle:
			animator->Play(WIDE("Bellial_HandIdle"));
			break;
		case hj::Boss1HandState::Attack:
			animator->Play(WIDE("Bellial_HandAttack"), false);
			break;
		case hj::Boss1HandState::End:
		default:
			break;
		}
	}

	void Stage1BossHand::ChangeHandType(Boss1HandType handType)
	{
		if (Boss1HandType::End == handType)
			return;

		mHandType = handType;
		switch (handType)
		{
		case hj::Boss1HandType::Left:
			SetName(WIDE("Boss_Bellial_Hand_L"));
			SetRotationY(DegreeToRadian(0.f));
			break;
		case hj::Boss1HandType::Right:
			SetName(WIDE("Boss_Bellial_Hand_R"));
			SetRotationY(XM_PI);
			break;
		case hj::Boss1HandType::End:
		default:
			break;
		}
	}

	void Stage1BossHand::CreateAnimation()
	{
		SpriteRenderer* sr = GetComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = sr->GetMaterial();
		std::shared_ptr<Texture> texture = material->GetTexture(eTextureSlot::T0);
		Vector2 atlasTexSize = texture->GetTexSize();

		float duration = 0.f;
		duration = 1.f / 15.f;

		XmlParser* parser = new XmlParser;
		std::wstring path = WIDE("02_Object/02_Monster/Monster_Boss_1_Bellial.xml");
		bool parseResult = parser->LoadFile(path);

		if (!parseResult)
			return;

		parseResult = parser->FindElem(WIDE("TextureAtlas"));
		parseResult = parser->IntoElem();

		std::wstring targetSpriteNameWstr;
		int count = 0;

		Vector2 leftTop = Vector2::Zero;
		Vector2 size = Vector2::One;
		Vector2 offset = Vector2::Zero;
		Vector2 trimmedSize = Vector2::Zero;
		Vector2 trimmedOffset = Vector2::Zero;

		size_t checkStringParseValue = std::wstring::npos;
		CREATE_ANIM(animBellialHandIdle, frame, atlasTexSize, duration);
		while (parseResult)
		{
			parseResult = parser->FindElem(WIDE("sprite"));
			if (!parseResult)
			{
				parseResult = true;
				parser->ResetMainPos();

				break;
			}

			targetSpriteNameWstr = WIDE("SkellBossLeftHandIdle") + std::to_wstring(count);
			checkStringParseValue = parser->CheckAttributeIncludeWstr(WIDE("n"), targetSpriteNameWstr);
			if (std::wstring::npos != checkStringParseValue)
			{
				leftTop.x = static_cast<float>(parser->GetIntAttribute(WIDE("x")));
				leftTop.y = static_cast<float>(parser->GetIntAttribute(WIDE("y")));
				size.x = static_cast<float>(parser->GetIntAttribute(WIDE("w")));
				size.y = static_cast<float>(parser->GetIntAttribute(WIDE("h")));
				offset.x = static_cast<float>(parser->GetFloatAttribute(WIDE("pX")));
				offset.y = static_cast<float>(parser->GetFloatAttribute(WIDE("pY")));
				trimmedSize.x = static_cast<float>(parser->GetFloatAttribute(WIDE("tW")));
				trimmedSize.y = static_cast<float>(parser->GetFloatAttribute(WIDE("tH")));
				trimmedOffset.x = static_cast<float>(parser->GetFloatAttribute(WIDE("tOX")));
				trimmedOffset.y = static_cast<float>(parser->GetFloatAttribute(WIDE("tOY")));

				FRAME_ADD_OFFSET_TRIM_OFFSET(frame, leftTop.x, leftTop.y, size.x, size.y
					, offset.x, offset.y, trimmedSize.x, trimmedSize.y
					, trimmedOffset.x, trimmedOffset.y, animBellialHandIdle);
			}
		}

		CREATE_SHEET(animBellialHandAttack);
		while (parseResult)
		{
			parseResult = parser->FindElem(WIDE("sprite"));
			if (!parseResult)
			{
				parseResult = true;
				parser->ResetMainPos();

				break;
			}

			targetSpriteNameWstr = WIDE("SkellBossLeftHandAttack");
			checkStringParseValue = parser->CheckAttributeIncludeWstr(WIDE("n"), targetSpriteNameWstr);
			if (std::wstring::npos != checkStringParseValue)
			{
				leftTop.x = static_cast<float>(parser->GetIntAttribute(WIDE("x")));
				leftTop.y = static_cast<float>(parser->GetIntAttribute(WIDE("y")));
				size.x = static_cast<float>(parser->GetIntAttribute(WIDE("w")));
				size.y = static_cast<float>(parser->GetIntAttribute(WIDE("h")));
				offset.x = static_cast<float>(parser->GetFloatAttribute(WIDE("pX")));
				offset.y = static_cast<float>(parser->GetFloatAttribute(WIDE("pY")));
				trimmedSize.x = static_cast<float>(parser->GetFloatAttribute(WIDE("tW")));
				trimmedSize.y = static_cast<float>(parser->GetFloatAttribute(WIDE("tH")));
				trimmedOffset.x = static_cast<float>(parser->GetFloatAttribute(WIDE("tOX")));
				trimmedOffset.y = static_cast<float>(parser->GetFloatAttribute(WIDE("tOY")));

				FRAME_ADD_OFFSET_TRIM_OFFSET(frame, leftTop.x, leftTop.y, size.x, size.y
					, offset.x, offset.y, trimmedSize.x, trimmedSize.y
					, trimmedOffset.x, trimmedOffset.y, animBellialHandIdle);

				++count;
			}
		}

		Animator* animator = GetComponent<Animator>();

		std::wstring idleAnimWstr = WIDE("Bellial_HandIdle");
		std::wstring attackAnimWstr = WIDE("Bellial_HandAttack");

		animator->Create(idleAnimWstr, texture, animBellialHandIdle, canvasSize, false);
		animator->Create(attackAnimWstr, texture, animBellialHandAttack, canvasSize, false);

		animator->Play(idleAnimWstr, true);

		SetScaleXY(canvasSize);
		GetTransform()->FixedUpdate();

		delete parser;
	}
}