#include "hjBoss1Hand.h"
#include "hjStage1Boss.h"
#include "hjSpriteRenderer.h"
#include "hjAnimation.h"
#include "hjAnimator.h"
#include "hjResourceManager.h"
#include "hjXmlParser.h"
#include "hjBoss1Laser.h"
#include "hjObject.h"
#include "hjTime.h"

namespace hj
{
	Boss1Hand::Boss1Hand()
		: mOwner(nullptr)
		, mHandType(Boss1HandType::End)
		, mHandState(Boss1HandState::End)
		, mbCanMove(false)
		, mbAttackStart(false)
	{
		SetName(WIDE("Boss_Bellial_Hand"));

		// 1. Sprite Renderer 积己
		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = MTRL_FIND_STR("MTRL_Monster_Boss_Bellial");
		std::shared_ptr<Mesh> mesh = MESH_FIND_STR("Mesh_Rect");
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

		// 3. Laser 积己 棺 包府
		{
			mLaser = object::InstantiateNoInitialize<Boss1Laser>(eLayerType::MonsterAttack_ForeGround);
			mLaser->GetTransform()->SetParent(this->GetTransform());
		}
	}

	Boss1Hand::~Boss1Hand()
	{
		if (mOwner)
			mOwner = nullptr;

		mLaser = nullptr;
	}
	
	void Boss1Hand::Initialize()
	{
		GameObject::Initialize();

		if (!mOwnerRoom)
			return;

		mLaser->SetOwnerRoom(mOwnerRoom);
		mLaser->Initialize();
	}
	
	void Boss1Hand::Update()
	{
		GameObject::Update();
	}
	
	void Boss1Hand::FixedUpdate()
	{
		if (mbCanMove && (mHandState == Boss1HandState::MoveToAttack ||
							mHandState == Boss1HandState::MoveToIdle))
		{
			MoveHand();
		}

		GameObject::FixedUpdate();
	}
	
	void Boss1Hand::Render()
	{
		GameObject::Render();
	}
	
	void Boss1Hand::Death()
	{
		mOwner = nullptr;

		mLaser->Death();

		Actor::Death();
	}

	void Boss1Hand::SetOwner(Stage1Boss* owner)
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

	void Boss1Hand::ChangeHandState(Boss1HandState nextState)
	{
		if (mHandState == nextState)
			return;

		Animator* animator = GetComponent<Animator>();
		if (!animator)
			return;

		mHandState = nextState;

		switch (nextState)
		{
		case hj::Boss1HandState::Idle:
			animator->Play(WIDE("Bellial_HandIdle"));
			mbCanMove = true;
			break;
		case hj::Boss1HandState::MoveToAttack:
			animator->Play(WIDE("Bellial_HandIdle"));
			break;
		case hj::Boss1HandState::Attack:
			animator->Play(WIDE("Bellial_HandAttack"), false);
			mbCanMove = false;
			mbAttackStart = true;
			break;
		case hj::Boss1HandState::MoveToIdle:
			animator->Play(WIDE("Bellial_HandIdle"));
			break;
		case hj::Boss1HandState::End:
			mbCanMove = false;
		default:
			break;
		}
	}

	void Boss1Hand::ChangeHandType(Boss1HandType handType)
	{
		if (Boss1HandType::End == handType)
			return;

		mHandType = handType;
		switch (handType)
		{
		case hj::Boss1HandType::Left:
			SetName(WIDE("Boss_Bellial_Hand_L"));
			SetRotationY(0.f);
			mLaser->SetPositionZ(-0.1f);
			mLaser->SetLaserColliderPosZ(-0.1f);
			break;
		case hj::Boss1HandType::Right:
			SetName(WIDE("Boss_Bellial_Hand_R"));
			SetRotationY(XM_PI);
			mLaser->SetPositionZ(0.1f);
			mLaser->SetLaserColliderPosZ(0.1f);
			break;
		case hj::Boss1HandType::End:
		default:
			break;
		}
	}

	void Boss1Hand::MoveHand()
	{
		float curPosY = GetPositionY();

		if (curPosY != mTargetMoveHandPosY)
		{
			mMoveTimer += Time::FixedDeltaTime();
			float moveAlpha = mMoveTimer / mMoveLimitTime;
			
			float calcedPosY = std::lerp(curPosY, mTargetMoveHandPosY, moveAlpha);

			SetPositionY(calcedPosY);			
		}
		if (curPosY == mTargetMoveHandPosY)
		{
			mMoveTimer = 0.f;
			switch (mHandState)
			{
			case hj::Boss1HandState::MoveToAttack:
				ChangeHandState(Boss1HandState::Attack);
				break;
			case hj::Boss1HandState::MoveToIdle:
				ChangeHandState(Boss1HandState::Idle);

				break;
			case hj::Boss1HandState::End:
			default:
				break;
			}
		}

	}

	void Boss1Hand::AttackStart(float posY)
	{
		SetMoveTargetHandPosY(posY);
		ChangeHandState(Boss1HandState::MoveToAttack);
	}

	void Boss1Hand::ShotLaser()
	{
		if (!mLaser)
		{
			return;
		}

		mLaser->ShotLaser();
		mbAttackStart = false;
	}

	void Boss1Hand::EndAttack(bool bNeedToMoveIdlePos)
	{
		mbCanMove = true;
		mLaser->ResetLaser();

		if(bNeedToMoveIdlePos)
		{
			mTargetMoveHandPosY = mInitialHandPosY;
			ChangeHandState(Boss1HandState::MoveToIdle);
		}
		else
		{
			ChangeHandState(Boss1HandState::Idle);
		}
	}

	bool Boss1Hand::IsAttackEnd()
	{
		bool result = false;
		
		if (mLaser)
		{
			result = mLaser->IsLaserEnd();
		}

		return result;
	}

	void Boss1Hand::PauseLaserAnimation()
	{
		if (mLaser)
		{
			mLaser->PauseLaserAnimation();
		}
	}

	void Boss1Hand::DeactivateLaser()
	{
		if (mLaser)
		{
			mLaser->DeactivateLaser();
			mLaser->Pause();
		}
	}

	void Boss1Hand::CreateAnimation()
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

			targetSpriteNameWstr = WIDE("SkellBossLeftHandIdle");
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
					, trimmedOffset.x, trimmedOffset.y, animBellialHandAttack);
			}
		}

		Animator* animator = GetComponent<Animator>();

		std::wstring idleAnimWstr = WIDE("Bellial_HandIdle");
		std::wstring attackAnimWstr = WIDE("Bellial_HandAttack");

		animator->Create(idleAnimWstr, texture, animBellialHandIdle, canvasSize, false);
		animator->Create(attackAnimWstr, texture, animBellialHandAttack, canvasSize, false);

		animator->Play(idleAnimWstr, true);

		animator->GetEvent(attackAnimWstr, 9) = std::bind(&Boss1Hand::ShotLaser, this);

		SetScaleXY(canvasSize);
		GetTransform()->FixedUpdate();

		delete parser;
	}
}