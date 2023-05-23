#include "hjBoss1Laser.h"
#include "hjSpriteRenderer.h"
#include "hjResources.h"
#include "hjAnimator.h"
#include "hjObject.h"
#include "hjXmlParser.h"

namespace hj
{
	Boss1Laser::Boss1Laser()
		: mLaserDir(Boss1LaserDir::Left)
	{
		SetName(WIDE("Effect_Boss1_Laser"));

		int maxLaserPart = 9;
		for (int iter = 0; iter < maxLaserPart; ++iter)
		{
			Boss1LaserPart* laserPart = object::Instantiate<Boss1LaserPart>(eLayerType::MonsterAttack);
			if (0 == iter)
			{
				laserPart->SetLaserType(Boss1LaserType::Head);
				laserPart->GetTransform()->SetParent(GetTransform());
			}
			else
			{
				if (1 == iter)
				{
					laserPart->SetLaserType(Boss1LaserType::SecondBody);
				}
				else
				{
					laserPart->SetLaserType(Boss1LaserType::Body);
				}
				laserPart->GetTransform()->SetParent(mLaserParts[iter - 1]->GetTransform());
			}

			mLaserParts.push_back(laserPart);
		}
	}

	Boss1Laser::~Boss1Laser()
	{
	}

	void Boss1Laser::Initialize()
	{
		GameObject::Initialize();
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

	void Boss1Laser::SetLaserDir(Boss1LaserDir dir)
	{
		switch (dir)
		{
		case hj::Boss1LaserDir::Left:
			SetRotationY(0.f);
			GetTransform()->FixedUpdate();
			break;
		case hj::Boss1LaserDir::Right:
			SetRotationY(XM_PI);
			GetTransform()->FixedUpdate();
			break;
		case hj::Boss1LaserDir::End:
		default:
			break;
		}

		mLaserDir = dir;
	}

	void Boss1Laser::ShotLaser(Boss1LaserDir dir)
	{
		SetLaserDir(dir);

		for (Boss1LaserPart* part : mLaserParts)
		{
			part->Play(true);
		}
	}

}

namespace hj
{
	Boss1LaserPart::Boss1LaserPart()
		: mLaserType(Boss1LaserType::End)
	{
		SetName(WIDE("Effect_Boss1_LaserParts"));

		// 1. Sprite Renderer 积己
		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = MTRL_FIND("MTRL_Monster_Boss_Bellial");
		std::shared_ptr<Mesh> mesh = MESH_FIND("Mesh_Rect");
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
		if (!mbPlaying)
			mbNeedPlay = true;

		if (mbNeedPlay)
		{
			PlayAnimation();
		}

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
		duration = 1.f / 6.f;

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
		Vector2 originSize = Vector2::Zero;

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

					FRAME_ADD_OFFSET(frame, leftTop.x, leftTop.y, size.x, size.y, offset.x, offset.y, animBellialLaserBody);

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

					FRAME_ADD_OFFSET(frame, leftTop.x, leftTop.y, size.x, size.y, offset.x, offset.y, animBellialLaserHead);

					++count;
				}
			}

			Animator* animator = GetComponent<Animator>();

			std::wstring bodyAnimWstr = WIDE("Effect_Bellial_Laser_Body");
			std::wstring headAnimWstr = WIDE("Effect_Bellial_Laser_Head");

			animator->Create(bodyAnimWstr, texture, animBellialLaserBody, canvasSize, false);
			animator->Create(headAnimWstr, texture, animBellialLaserHead, canvasSize, false);
			animator->GetCompleteEvent(bodyAnimWstr) = std::bind(&Boss1LaserPart::PlayEnd, this);
			animator->GetCompleteEvent(headAnimWstr) = std::bind(&Boss1LaserPart::PlayEnd, this);

			SetScaleXY(canvasSize);
			GetTransform()->FixedUpdate();
		}

		delete parser;		
	}

	void Boss1LaserPart::SetLaserType(Boss1LaserType type)
	{
		mLaserType = type;
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

		mbNeedPlay = false;
		mbPlaying = true;
	}

}