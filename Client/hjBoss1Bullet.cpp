#include "hjBoss1Bullet.h"
#include "hjSpriteRenderer.h"
#include "hjXmlParser.h"
#include "hjAnimation.h"
#include "hjAnimator.h"
#include "hjResources.h"
#include "hjTime.h"
#include "hjCollider2D.h"
#include "hjBoss1BulletColliderScript.h"
#include "hjScript.h"

namespace hj
{
	Boss1Bullet::Boss1Bullet()
		: mMoveDir(Vector2::Zero)
		, mMoveSpeed(600.f)
		, mActiveTime(2.5f)
		, mActiveTimer(0.f)
		, mOwnerRoom(nullptr)
		, mbIsBulletState(true)
	{
		SetName(WIDE("Effect_Boss1_Bullet"));

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

		Collider2D* circleCollider = AddComponent<Collider2D>();
		circleCollider->SetColliderType(eColliderType::Circle);
		//Vector2 colliderSize = animator->GetCurrentAnimation()->GetSpriteSize(1) / GetScaleXY();
		if (animator && animator->GetCurrentAnimation())
		{
			circleCollider->SetSize(animator->GetCurrentAnimation()->GetSpriteSize(0));
		}

		Boss1BulletColliderScript* script = AddComponent<Boss1BulletColliderScript>();
		script->SetCollider(circleCollider);

		Pause();
	}

	Boss1Bullet::Boss1Bullet(const Boss1Bullet& bullet)
		: GameObject(bullet)
		, mMoveDir(Vector2::Zero)
		, mMoveSpeed(bullet.mMoveSpeed)
		, mActiveTime(bullet.mActiveTime)
		, mActiveTimer(bullet.mActiveTimer)
	{
		Boss1BulletColliderScript* script = this->GetScript<Boss1BulletColliderScript>();
		script->SetCollider(GetComponent<Collider2D>());
		script->SetOwner(this);
	}

	Boss1Bullet::~Boss1Bullet()
	{
	}
	void Boss1Bullet::Initialize()
	{
		GameObject::Initialize();
	}
	void Boss1Bullet::Update()
	{
		GameObject::Update();
	}
	void Boss1Bullet::FixedUpdate()
	{
		if (eState::Active != GetState())
			return;

		mActiveTimer += Time::FixedDeltaTime();

		if (mActiveTimer > mActiveTime)
		{
			mActiveTimer = 0.f;
			Pause();
			return;
		}

		AddPositionXY(mMoveDir * mMoveSpeed * Time::FixedDeltaTime());

		GameObject::FixedUpdate();
	}
	void Boss1Bullet::Render()
	{
		GameObject::Render();
	}

	GameObject* Boss1Bullet::Clone() const
	{
		return new Boss1Bullet(*this);
	}

	void Boss1Bullet::CreateAnimation()
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
			CREATE_ANIM(animBellialBullet, frame, atlasTexSize, duration);
			while (parseResult)
			{
				parseResult = parser->FindElem(WIDE("sprite"));
				if (!parseResult)
				{
					parseResult = true;
					break;
				}

				targetSpriteNameWstr = WIDE("BossBullet00") + std::to_wstring(count);
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
						, trimmedOffset.x, trimmedOffset.y, animBellialBullet);

					++count;
				}
			}

			parser->ResetMainPos();
			count = 0;


			CREATE_SHEET(animBellialBulletFX);
			while (parseResult)
			{
				parseResult = parser->FindElem(WIDE("sprite"));
				if (!parseResult)
				{
					parseResult = true;
					break;
				}

				targetSpriteNameWstr = WIDE("BossBulletFX00") + std::to_wstring(count);
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
						, trimmedOffset.x, trimmedOffset.y, animBellialBulletFX);

					++count;
				}
			}

			Animator* animator = GetComponent<Animator>();

			std::wstring bulletAnimWstr = WIDE("Effect_Bellial_Bullet");
			std::wstring bulletFXAnimWstr = WIDE("Effect_Bellial_Bullet_FX");

			animator->Create(bulletAnimWstr, texture, animBellialBullet, canvasSize, false);
			animator->Create(bulletFXAnimWstr, texture, animBellialBulletFX, canvasSize, false);

			//animator->GetStartEvent(bulletAnimWstr) = std::bind(&Boss1LaserPart::OnDamageEnter, this);
			//animator->GetCompleteEvent(bulletFXAnimWstr) = std::bind(&Boss1LaserPart::OnDamageExit, this);

			SetScaleXY(canvasSize);
			GetTransform()->FixedUpdate();

			animator->Play(bulletAnimWstr);
		}

		delete parser;
	}
}