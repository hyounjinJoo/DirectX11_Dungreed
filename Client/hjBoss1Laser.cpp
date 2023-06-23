#include "hjBoss1Laser.h"
#include "hjSpriteRenderer.h"
#include "hjResources.h"
#include "hjAnimator.h"
#include "hjObject.h"
#include "hjXmlParser.h"
#include "hjCollider2D.h"
#include "hjBoss1LaserCollider.h"
#include "hjAudioClip.h"
#include "hjAudioSource.h"

namespace hj
{
	Boss1Laser::Boss1Laser()
		: mLaserCollider(nullptr)
	{
		SetName(WIDE("Effect_Boss1_Laser"));

		int maxLaserPart = 9;

		Vector2 laserWholeScale = Vector2::Zero; 
		float laserWholeMidPosX = 0.f;
		for (int iter = 0; iter < maxLaserPart; ++iter)
		{
			Boss1LaserPart* laserPart = object::Instantiate<Boss1LaserPart>(eLayerType::MonsterAttack_ForeGround);
			laserPart->SetOwnerLaser(this);

#define LASER_HEAD_PART_INDEX 0
#define LASER_SECOND_BODY_PART_INDEX 1

			if (LASER_HEAD_PART_INDEX == iter)
			{
				laserPart->SetLaserType(Boss1LaserType::Head);
				laserPart->GetTransform()->SetParent(GetTransform());
			}
			else
			{
				if (LASER_SECOND_BODY_PART_INDEX == iter)
				{
					laserPart->SetLaserType(Boss1LaserType::SecondBody);
				}
				else
				{
					laserPart->SetLaserType(Boss1LaserType::Body);
				}
				laserPart->GetTransform()->SetParent(mLaserParts[static_cast<size_t>(iter) - static_cast<size_t>(1)]->GetTransform());
			}

			
			Vector2 spriteTrimmedScale = laserPart->GetLaserRealSize();
			laserWholeScale.x += spriteTrimmedScale.x;
			if (LASER_HEAD_PART_INDEX == iter)
			{
				laserWholeScale.y = spriteTrimmedScale.y * 0.8f;
				laserWholeMidPosX += spriteTrimmedScale.x;

				spriteTrimmedScale.x *= -0.6f;
				spriteTrimmedScale.y *= -0.2f;

				AddPositionXY(spriteTrimmedScale);
			}
			
			mLaserParts.push_back(laserPart);
		}

		if (0 < mLaserParts.size())
		{
			laserWholeMidPosX += laserWholeScale.x * 0.5f;

			mLaserCollider = object::Instantiate<Boss1LaserCollider>(eLayerType::MonsterAttack_ForeGround);
			mLaserCollider->Pause();
			mLaserCollider->SetPositionX(laserWholeMidPosX);
			mLaserCollider->SetScaleXY(laserWholeScale);

			mLaserCollider->SetOwnerObject(this);
			mLaserCollider->GetTransform()->SetParent(this->GetTransform());
			mLaserCollider->GetTransform()->FixedUpdate();
		}

		mAttackSoundObj = object::Instantiate<GameObject>(eLayerType::UI);
		std::shared_ptr<AudioClip> clip = Resources::Load<AudioClip>(WIDE("laser"), WIDE("laser.mp3"));
		AudioSource* audioSrc = mAttackSoundObj->AddComponent<AudioSource>();
		clip->SetLoop(false);
		audioSrc->SetClip(clip);
	}

	Boss1Laser::~Boss1Laser()
	{
		for (Boss1LaserPart* iter : mLaserParts)
		{
			iter = nullptr;
		}

		mLaserCollider = nullptr;

		if (mAttackSoundObj)
		{
			mAttackSoundObj->Death();
		}
	}

	void Boss1Laser::Initialize()
	{
		GameObject::Initialize();

		if (!mOwnerRoom)
			return;

		for (Boss1LaserPart* iter : mLaserParts)
		{
			if (iter)
			{
				iter->SetOwnerRoom(mOwnerRoom);
			}
		}
	}
	void Boss1Laser::Update()
	{
		GameObject::Update();
	}
	void Boss1Laser::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}
	void Boss1Laser::Render()
	{
		GameObject::Render();
	}

	void Boss1Laser::Death()
	{
		if (mLaserCollider)
		{
			mLaserCollider->Death();
		}
			
		for (auto laserPart : mLaserParts)
		{
			laserPart->Death();
		}
	}

	void Boss1Laser::ShotLaser()
	{
		for (Boss1LaserPart* part : mLaserParts)
		{
			part->Play(true);
		}

		mAttackSoundObj->GetComponent<AudioSource>()->Play();
		mLaserCollider->Activate();
	}

	void Boss1Laser::ResetLaser()
	{
		for (Boss1LaserPart* part : mLaserParts)
		{
			part->ResetLaser();
		}
	}

	bool Boss1Laser::IsLaserEnd()
	{
		bool result = true;
		if (!mLaserParts.empty())
		{
			result = mLaserParts[0]->IsEndLaser();
		}

		return result;
	}

	void Boss1Laser::SetLaserColliderPosZ(float posZ)
	{		
		if (mLaserCollider)
		{
			mLaserCollider->SetPositionZ(posZ);
		}		
	}

	void Boss1Laser::PauseLaserAnimation()
	{
		if (mLaserCollider)
		{
			mLaserCollider->Pause();
		}

		for (auto laserParts : mLaserParts)
		{
			laserParts->PauseAnimation();
		}
	}

	void Boss1Laser::DeactivateLaser()
	{
		if (mLaserCollider)
		{
			mLaserCollider->Pause();
		}

		for (auto laserParts : mLaserParts)
		{
			laserParts->Pause();
		}
	}

}

namespace hj
{
	Boss1LaserPart::Boss1LaserPart()
		: mLaserType(Boss1LaserType::End)
		, mOwnerLaser(nullptr)
		, mbPlaying(false)
		, mbIsEnd(false)
	{
		SetName(WIDE("Effect_Boss1_LaserParts"));

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

	Boss1LaserPart::~Boss1LaserPart()
	{
	}

	void Boss1LaserPart::Initialize()
	{
		GameObject::Initialize();
	}

	void Boss1LaserPart::Update()
	{
		if(mbPlaying)
		{
			GameObject::Update(); 
		}
	}

	void Boss1LaserPart::FixedUpdate()
	{
		if (mbPlaying)
		{
			Animator* animator = GetComponent<Animator>();

			float frameSizeX = 1.f;
			if (Boss1LaserType::SecondBody == mLaserType)
			{
				if (animator->GetCurrentAnimation())
				{
					int index = animator->GetCurrentAnimation()->GetCurrentFrameNumber();
					frameSizeX = animator->FindAnimation(WIDE("Effect_Bellial_Laser_Head"))->GetSpriteSize(static_cast<UINT>(index)).x;
				}
			}
			else
			{
				if (animator->GetCurrentAnimation())
				{
					frameSizeX = animator->GetCurrentAnimation()->GetCurrentSpriteSize().x;
				}
			}
			SetPositionX(frameSizeX);
			
			GameObject::FixedUpdate();
		}
	}

	void Boss1LaserPart::Render()
	{
		if (mbPlaying)
		{
			GameObject::Render();
		}
	}

	void Boss1LaserPart::CreateAnimation()
	{
		SpriteRenderer* sr = GetComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = sr->GetMaterial();
		std::shared_ptr<Texture> texture = material->GetTexture(eTextureSlot::T0);
		Vector2 atlasTexSize = texture->GetTexSize();

		float duration = 0.f;
		duration = 1.f / 24.f;

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
			CREATE_ANIM(animBellialLaserBody, frame, atlasTexSize, duration);
			while (parseResult)
			{
				parseResult = parser->FindElem(WIDE("sprite"));
				if (!parseResult)
				{
					parseResult = true;
					break;
				}

				targetSpriteNameWstr = WIDE("SkellBossLaserBody") + std::to_wstring(count);
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
						, trimmedOffset.x, trimmedOffset.y, animBellialLaserBody);

					++count;
				}
			}
						
			parser->ResetMainPos();
			count = 0;


			CREATE_SHEET(animBellialLaserHead);
			while (parseResult)
			{
				parseResult = parser->FindElem(WIDE("sprite"));
				if (!parseResult)
				{
					parseResult = true;
					break;
				}

				targetSpriteNameWstr = WIDE("SkellBossLaserHead") + std::to_wstring(count);
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
						, trimmedOffset.x, trimmedOffset.y, animBellialLaserHead);

					++count;
				}
			}

			Animator* animator = GetComponent<Animator>();

			std::wstring bodyAnimWstr = WIDE("Effect_Bellial_Laser_Body");
			std::wstring headAnimWstr = WIDE("Effect_Bellial_Laser_Head");

			animator->Create(bodyAnimWstr, texture, animBellialLaserBody, canvasSize, false);
			animator->Create(headAnimWstr, texture, animBellialLaserHead, canvasSize, false);
			animator->GetCompleteEvent(bodyAnimWstr) = std::bind(&Boss1LaserPart::Play, this, false);
			animator->GetCompleteEvent(headAnimWstr) = std::bind(&Boss1LaserPart::Play, this, false);


			animator->GetStartEvent(bodyAnimWstr) = std::bind(&Boss1LaserPart::OnDamageEnter, this);
			animator->GetEvent(bodyAnimWstr, 4) = std::bind(&Boss1LaserPart::OnDamageExit, this);

			SetScaleXY(canvasSize);
			GetTransform()->FixedUpdate();
		}

		delete parser;		
	}

	hj::math::Vector2 Boss1LaserPart::GetLaserRealSize()
	{
		Animator* animator = GetComponent<Animator>();
		if (!animator)
			return Vector2::Zero;


		std::wstring animWstr;
		switch (mLaserType)
		{
		case Boss1LaserType::Body:
		case Boss1LaserType::SecondBody:
			animWstr = WIDE("Effect_Bellial_Laser_Body");
			break;
		case Boss1LaserType::Head:
			animWstr = WIDE("Effect_Bellial_Laser_Head");
			break;
		case Boss1LaserType::End:
		default:
			return Vector2::Zero;
		}

		Animation* animation = animator->FindAnimation(animWstr);
		if (!animator)
			return Vector2::Zero;

		Vector2 laserRealSize = animation->GetSpriteTrimSize(0);

		return laserRealSize;
	}

	void Boss1LaserPart::SetLaserType(Boss1LaserType type)
	{
		mLaserType = type;
	}

	void Boss1LaserPart::Play(bool play)
	{
		mbPlaying = play;
		if (mbPlaying)
		{
			PlayAnimation();
		}
		else
		{
			mbIsEnd = true;
		}
	}

	void Boss1LaserPart::OnDamageEnter()
	{
		if (!mOwnerLaser)
		{
			return;
		}

		Boss1LaserCollider* laserCollider = mOwnerLaser->GetLaserCollider();
		if (!laserCollider)
			return;

		laserCollider->Activate();
	}

	void Boss1LaserPart::OnDamageExit()
	{
		if (!mOwnerLaser)
		{
			return;
		}
		
		Boss1LaserCollider* laserCollider = mOwnerLaser->GetLaserCollider();
		if (!laserCollider)
			return;
		
		laserCollider->Pause();
	}

	void Boss1LaserPart::SetOwnerLaser(Boss1Laser* laser)
	{
		Boss1Laser* verifiedLaser = dynamic_cast<Boss1Laser*>(laser);
		if (verifiedLaser)
		{
			mOwnerLaser = verifiedLaser;
		}
	}

	void Boss1LaserPart::PlayAnimation()
	{
		Animator* animator = GetComponent<Animator>();
		if (!animator)
		{
			return;
		}

		switch (mLaserType)
		{
		case hj::Boss1LaserType::Body:
			animator->Play(WIDE("Effect_Bellial_Laser_Body"), false);
			break;
		case hj::Boss1LaserType::SecondBody:
			animator->Play(WIDE("Effect_Bellial_Laser_Body"), false);
			break;
		case hj::Boss1LaserType::Head:
			animator->Play(WIDE("Effect_Bellial_Laser_Head"), false);
			break;
		case hj::Boss1LaserType::End:
		default:
			break;
		}
	}

}