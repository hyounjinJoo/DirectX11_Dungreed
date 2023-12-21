#include "hjBoss1BackgroundEffect.h"
#include "hjSpriteRenderer.h"
#include "hjResourceManager.h"
#include "hjAnimator.h"
#include "hjXmlParser.h"

namespace hj
{
	Boss1BackgroundEffect::Boss1BackgroundEffect()
	{
		SetName(WIDE("Effect_Boss1_BackgroundFX"));

		// 1. Sprite Renderer 积己
		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = MTRL_FIND_STR("MTRL_Monster_Boss_Bellial");
		std::shared_ptr<Mesh> mesh = MESH_FIND_STR("Mesh_Rect");
		sr->SetMaterial(material);
		sr->SetMesh(mesh);

		// 2. Animator 积己 棺 Animation 眠啊
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

	Boss1BackgroundEffect::~Boss1BackgroundEffect()
	{
	}

	void Boss1BackgroundEffect::CreateAnimation()
	{
		SpriteRenderer* sr = GetComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = sr->GetMaterial();
		std::shared_ptr<Texture> texture = material->GetTexture(eTextureSlot::T0);
		Vector2 atlasTexSize = texture->GetTexSize();

		float duration = 0.f;
		duration = 1.f / 15.f;

		XmlParser* parser = new XmlParser;
		std::wstring path = WIDE("02_Object/02_Monster/01_Boss/Monster_Boss_1_Bellial.xml");
		bool parseResult = parser->LoadFile(path);

		if (!parseResult)
		{
			delete parser;
			return;
		}

		parseResult = parser->FindElem(WIDE("TextureAtlas"));
		parseResult = parser->IntoElem();

		std::wstring targetSpriteNameWstr;
		int count = 0;

		Vector2 leftTop = Vector2::Zero;
		Vector2 size = Vector2::One;
		Vector2 offset = Vector2::Zero;
		Vector2 originSize = Vector2::Zero;
		Vector2 trimmedSize = Vector2::One;
		Vector2 trimmedOffset = Vector2::Zero;

		size_t checkStringParseValue = std::wstring::npos;

		{
			CREATE_ANIM(animBellialBackground, frame, atlasTexSize, duration);
			while (parseResult)
			{
				parseResult = parser->FindElem(WIDE("sprite"));
				if (!parseResult)
				{
					parseResult = true;
					break;
				}

				targetSpriteNameWstr = WIDE("SkellBossBack") + std::to_wstring(count);
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
						, trimmedOffset.x, trimmedOffset.y, animBellialBackground);

					++count;
				}
			}

			Animator* animator = GetComponent<Animator>();

			std::wstring bossBackgroundAnimWstr = WIDE("Effect_Bellial_Background");

			animator->Create(bossBackgroundAnimWstr, texture, animBellialBackground, canvasSize, false);

			SetScaleXY(canvasSize);
			GetTransform()->FixedUpdate();

			animator->Play(bossBackgroundAnimWstr);
		}

		delete parser;
	}
}