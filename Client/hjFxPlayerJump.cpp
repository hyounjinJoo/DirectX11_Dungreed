#include "hjFxPlayerJump.h"
#include "hjSpriteRenderer.h"
#include "hjResourceManager.h"
#include "hjMaterial.h"
#include "hjMesh.h"
#include "hjTexture.h"
#include "hjAnimation.h"
#include "hjAnimator.h"
#include "hjPlayer.h"
#include "hjXmlParser.h"

namespace hj
{
	FxPlayerJump::FxPlayerJump()
		: mOwner(nullptr)
		, mJumpEffect(JumpEffect::Single)
		, mbActivateEffect(false)
		, mOffsetPos(Vector2::Zero)
		, mAnimator(nullptr)
	{
		SetName(WIDE("플레이어 점프"));

		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = MTRL_FIND_STR("MTRL_FX");
		std::shared_ptr<Mesh> mesh = MESH_FIND_STR("Mesh_Rect");
		sr->SetMaterial(material);
		sr->SetMesh(mesh);

		mAnimator = AddComponent<Animator>();
		std::shared_ptr<Texture> texture = material->GetTexture(eTextureSlot::T0);
		Vector2 atlasTexSize = texture->GetTexSize();

		XmlParser* parser = new XmlParser;
		std::wstring path = WIDE("08_FX/08_FX.xml");
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
			float duration = 0.f;
			duration = 1.f / 14.f;

			CREATE_ANIM(animPlayerJump, frame, atlasTexSize, duration);
			while (parseResult)
			{
				parseResult = parser->FindElem(WIDE("sprite"));
				if (!parseResult)
				{
					parseResult = true;
					break;
				}

				targetSpriteNameWstr = WIDE("JumpFX/JumpFX") + std::to_wstring(count);
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
						, trimmedOffset.x, trimmedOffset.y, animPlayerJump);

					++count;
				}
			}
			parser->ResetMainPos();
			count = 0;

			CREATE_SHEET(animPlayerDoubleJump);
			while (parseResult)
			{
				parseResult = parser->FindElem(WIDE("sprite"));
				if (!parseResult)
				{
					parseResult = true;
					break;
				}

				targetSpriteNameWstr = WIDE("DubleJumpFX/DubleJumpFX") + std::to_wstring(count);
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
						, trimmedOffset.x, trimmedOffset.y, animPlayerDoubleJump);

					++count;
				}
			}

			AUTO_OFFSET_CALC_Y(animPlayerJump);
			AUTO_OFFSET_CALC_Y(animPlayerDoubleJump);

			mAnimator->Create(WIDE("FX_PlayerJump"), texture, animPlayerJump, canvasSize, false);
			mAnimator->Create(WIDE("FX_PlayerDoubleJump"), texture, animPlayerDoubleJump, canvasSize, false);
			SetScaleXY(canvasSize);

			mOffsetPos.y = canvasSize.y * 0.5f;
		}

		delete parser;
	}
	FxPlayerJump::~FxPlayerJump()
	{
	}
	void FxPlayerJump::Initialize()
	{
		GameObject::Initialize();
	}
	void FxPlayerJump::Update()
	{
		GameObject::Update();

		if (mAnimator->GetCurrentAnimation())
		{
			if (mAnimator->GetCurrentAnimation()->IsComplete())
				mbActivateEffect = false;
		}
	}

	void FxPlayerJump::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void FxPlayerJump::Render()
	{
		if (mbActivateEffect)
			GameObject::Render();
	}

	void FxPlayerJump::SetOwner(Player* owner)
	{
		mOwner = owner;
	}

	void FxPlayerJump::ActivateEffect(JumpEffect jumpEffect)
	{
		SetPositionXY(mOwner->GetWorldCenterBottom() + mOffsetPos);
		mbActivateEffect = true;
		switch (jumpEffect)
		{
		case hj::JumpEffect::Single:
			mAnimator->Play(L"FX_PlayerJump", false);
			break;
		case hj::JumpEffect::Double:
			mAnimator->Play(L"FX_PlayerDoubleJump", false);
			break;
		case hj::JumpEffect::End:
		default:
			break;
		}
	}
}