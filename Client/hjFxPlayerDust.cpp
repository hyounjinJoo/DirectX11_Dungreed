#include "hjFxPlayerDust.h"
#include "hjPlayer.h"
#include "hjSpriteRenderer.h"
#include "hjResourceManager.h"
#include "hjMaterial.h"
#include "hjTexture.h"
#include "hjAnimator.h"
#include "hjRigidBody.h"
#include "hjXmlParser.h"


namespace hj
{
	FxPlayerDust::FxPlayerDust()
		: mOwner(nullptr)
		, mAnimator(nullptr)
		, mbActivateEffect(false)
		, mOffsetPos(Vector2::Zero)
	{
		SetName(WIDE("플레이어 더스트"));

		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = MTRL_FIND_STR("MTRL_FX");
		std::shared_ptr<Mesh> mesh = MESH_FIND_STR("Mesh_Rect");
		sr->SetMaterial(material);
		sr->SetMesh(mesh);
		
		mAnimator = AddComponent<Animator>();
		if (mAnimator)
		{
			CreateAnimation();
		}
	}
	FxPlayerDust::~FxPlayerDust()
	{
	}
	void FxPlayerDust::Initialize()
	{
		GameObject::Initialize();
	}

	void FxPlayerDust::Update()
	{
		GameObject::Update(); 
		if (!mbActivateEffect)
		{
			if (mOwner)
			{
				SetPositionXY(mOwner->GetWorldCenterBottom() + mOffsetPos);
				eMoveDir moveDir = mOwner->GetComponent<RigidBody>()->GetMoveDir();
				bool isFlip = moveDir == eMoveDir::Left ? true : false;
				if (isFlip)
				{
					SetRotationY(PI);
				}
				else
				{
					SetRotationY(0.f);
				}
			}
		}
		else
		{
			if (mAnimator->GetCurrentAnimation()->IsComplete())
				mbActivateEffect = false;
		}
	}

	void FxPlayerDust::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void FxPlayerDust::Render()
	{
		if(mbActivateEffect)
			GameObject::Render();
	}

	void FxPlayerDust::SetOwner(class Player* owner)
	{
		mOwner = owner;
	}

	void FxPlayerDust::ActivateEffect()
	{
		if (mbActivateEffect)
			return;

		mbActivateEffect = true;
		mAnimator->Play(L"FX_PlayerDust", false);
	}

	void FxPlayerDust::ChangeAnimationDuration(float wholePlayTime)
	{
		Animation* dustAnim = mAnimator->FindAnimation(L"FX_PlayerDust");
		if (!dustAnim)
			return;

		dustAnim->ChangePlayDuration(wholePlayTime / 6.f);
	}

	void FxPlayerDust::CreateAnimation()
	{
		SpriteRenderer* sr = GetComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = sr->GetMaterial();
		std::shared_ptr<Texture> texture = material->GetTexture(eTextureSlot::T0);
		Vector2 atlasTexSize = texture->GetTexSize();

		float duration = 0.f;
		duration = 0.25f / 6.f;

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
		int count = 1;

		Vector2 leftTop = Vector2::Zero;
		Vector2 size = Vector2::One;
		Vector2 offset = Vector2::Zero;
		Vector2 originSize = Vector2::Zero;
		Vector2 trimmedSize = Vector2::One;
		Vector2 trimmedOffset = Vector2::Zero;

		size_t checkStringParseValue = std::wstring::npos;

		{
			CREATE_ANIM(animPlayerDust, frame, atlasTexSize, duration);
			while (parseResult)
			{
				parseResult = parser->FindElem(WIDE("sprite"));
				if (!parseResult)
				{
					parseResult = true;
					break;
				}

				targetSpriteNameWstr = WIDE("Dust") + std::to_wstring(count);
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
						, trimmedOffset.x, trimmedOffset.y, animPlayerDust);

					++count;
				}
			}

			Animator* animator = GetComponent<Animator>();

			std::wstring dustAnimWstr = WIDE("FX_PlayerDust");
			AUTO_OFFSET_CALC(animPlayerDust);
			mAnimator->Create(dustAnimWstr, texture, animPlayerDust, canvasSize, false);
						
			SetScaleXY(canvasSize);
			GetTransform()->FixedUpdate();

			mOffsetPos.y = canvasSize.y * 0.5f;
		}

		delete parser;
	}
}