#include "hjStage1Boss.h"
#include "hjSpriteRenderer.h"
#include "hjResources.h"
#include "hjAnimator.h"
#include "hjCollider2D.h"
#include "hjObject.h"
#include "hjTime.h"
#include "hjStage1BossHand.h"
#include "hjBoss1Laser.h"
#include "hjBoss1Sword.h"
#include "hjXmlParser.h"
#include <random>
#include "hjBoss1Bullet.h"
#include "hjBoss1BulletMuzzle.h"

namespace hj
{
#define Bellial_Horn_X_Size_Ratio 0.10714f
#define Bellial_Horn_Y_Size_Ratio 0.11718f

	Stage1Boss::Stage1Boss()
	{
		SetName(WIDE("Boss_Bellial"));

		// 1. Sprite Renderer 생성
		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = MTRL_FIND("MTRL_Monster_Boss_Bellial");
		std::shared_ptr<Mesh> mesh = MESH_FIND("Mesh_Rect");
		sr->SetMaterial(material);
		sr->SetMesh(mesh);

		//AddComponent<Collider2D>();

		// 2. Body Animator 생성 및 Animation 추가
		Animator* animator = AddComponent<Animator>();
		if (material && animator)
		{
			std::shared_ptr<Texture> texture = material->GetTexture(eTextureSlot::T0);
			if (texture)
			{
				CreateBodyAnimation();
			}
		}

		// 3. Damage 판정용 객체와 충돌체 생성
		mDamageBody = object::Instantiate<GameObject>(eLayerType::Monster);

		Vector2 bodySize = GetComponent<Animator>()->GetCurrentAnimation()->GetCurrentSpriteSize();
		bodySize.x *= 0.785f;
		mDamageBody->SetScaleXY(bodySize);
		float movedPos = GetScaleX() * Bellial_Horn_X_Size_Ratio;
		mDamageBody->AddPositionX(movedPos);
		mDamageBody->GetTransform()->SetParent(this->GetTransform());

		mDamageBody->GetTransform()->FixedUpdate();

		mDamageCollider = mDamageBody->AddComponent<Collider2D>();

		// 4. Left Hand, Right Hand 생성
		mLeftHand = object::Instantiate<Stage1BossHand>(eLayerType::MonsterHas, Vector3(80.f * -7.f, 80.f * -3.f, 1.f));
		mRightHand = object::Instantiate<Stage1BossHand>(eLayerType::MonsterHas, Vector3(80.f * 7.f, 0.f, 1.f));

		mLeftHand->ChangeHandType(Boss1HandType::Left);
		mRightHand->ChangeHandType(Boss1HandType::Right);

		mLeftHand->ChangeHandState(Boss1HandState::Attack);
		mRightHand->ChangeHandState(Boss1HandState::Idle);


		// 5. SkellBossBack 이미지 객체 생성 
		mBackground = object::Instantiate<GameObject>(eLayerType::MonsterHas, Vector3(GetScaleX() * 0.5f, 0.f, 1.f));
		// 6. Effect 객체 생성
		{
		//	(1) DieFX 전용 객체 생성
		//	(2) BackgroundParticle 이미지 객체 생성 혹은 파티클 생성
			mBackground = object::Instantiate<GameObject>(eLayerType::MonsterHas, Vector3(GetScaleX() * 0.5f, 0.f, 1.f));
		}
		// 7. Pattern 1(Summon Sword)에 필요한 것들 생성
		mBoss1Sword = object::Instantiate<Boss1Sword>(eLayerType::MonsterAttack, Vector3(GetScaleX()* 0.5f, 0.f, 0.f));
		mBoss1Sword->SetSpawnPosXY(Vector2(GetScaleX() * 0.5f, 0.f));

		mBoss1Sword->ChangeSwordState(Boss1SwordState::Spawn);
		// 8. Pattern 2(Fire Bullets)에 필요한 것들 생성
		// 9. Pattern 3(Shot Laser)에 필요한 것들 생성
		Boss1BulletMuzzle* mBoss1Muzzle = object::Instantiate<Boss1BulletMuzzle>(eLayerType::MonsterHas);
	}
	Stage1Boss::~Stage1Boss()
	{
	}
	void Stage1Boss::Initialize()
	{
		GameObject::Initialize();
	}

	void Stage1Boss::Update()
	{
		GameObject::Update();
		if (Boss1SwordState::End == mBoss1Sword->GetSwordState())
		{
			mBoss1Sword->ChangeSwordState(Boss1SwordState::Spawn);
		}
	}

	void Stage1Boss::FixedUpdate()
	{
		static float testTime = 0.f;
		static float testMaxTime = 5.f;

		AdjustColliderPosAndSize();

		if (testTime < testMaxTime)
		{
			testTime += Time::ActualDeltaTime();
			Animator* animator = GetComponent<Animator>();
			
			if (testTime > testMaxTime)
			{
				testTime = 0.f;
				
				if (animator->GetCurrentAnimation()->AnimationName()._Equal(WIDE("Bellial_Idle")))
				{
					animator->Play(WIDE("Bellial_AttackStart"), false);
				}
				else if (animator->GetCurrentAnimation()->AnimationName()._Equal(WIDE("Bellial_Attack")))
				{
					animator->Play(WIDE("Bellial_AttackEnd"), true);
				}
			}
			else
			{
				Animation* anim = animator->GetCurrentAnimation();
				std::wstring curAnimName = anim->AnimationName();
				if (curAnimName._Equal(WIDE("Bellial_AttackStart")))
				{
					if (anim->IsComplete())
					{
						testTime = 0.f;
						animator->Play(WIDE("Bellial_Attack"));
					}
				}
				else if (curAnimName._Equal(WIDE("Bellial_AttackEnd")))
				{
					if (anim->IsComplete())
					{
						testTime = 0.f;
						animator->Play(WIDE("Bellial_Idle"));
					}
				}
			}
		}

		GameObject::FixedUpdate();
	}

	void Stage1Boss::Render()
	{
		GameObject::Render();
	}

	void Stage1Boss::Damaged(float damage)
	{

	}

	void Stage1Boss::AdjustColliderPosAndSize()
	{
		if (!GetComponent<Animator>() || !GetComponent<Animator>()->GetCurrentAnimation())
		{
			return;
		}

		float canvasSizeY = GetComponent<Animator>()->GetCurrentAnimation()->GetCanvasSizeY();
		float spriteSizeY = GetComponent<Animator>()->GetCurrentAnimation()->GetCurrentSpriteSize().y;
		float offsetY = GetComponent<Animator>()->GetCurrentAnimation()->GetCurrentSpriteTrimOffset().y;

		Vector2 bodySize = GetComponent<Animator>()->GetCurrentAnimation()->GetCurrentSpriteTrimSize();
		float topPos = GetWorldCenterUp().y;
		float moveY = 0.f;
		if (bodySize.y != 630.f)
		{
			float noHornHeadSizeY = (bodySize.y - 77.5f) * 0.5f;
			Animator* animator = GetComponent<Animator>();
			if (animator->GetCurrentAnimation()->AnimationName()._Equal(WIDE("Bellial_Idle")))
			{
				moveY = topPos - offsetY - noHornHeadSizeY - (canvasSizeY - spriteSizeY);
			}
			else
			{
				float bodySizeOffset = ((bodySize.y - 77.5f) * 0.5f);
				moveY = topPos - offsetY - 77.5f - bodySizeOffset;
			}
		}
		else
		{
			moveY = -offsetY - 77.5f * 0.5f;
		}

		bodySize.x *= 0.785f;
		bodySize.y -= 77.5f;

		mDamageBody->SetScaleXY(bodySize);
		mDamageBody->SetPositionY(moveY);
	}

	bool Stage1Boss::IsAttackEnd()
	{

		return true;
	}

	void Stage1Boss::ChangeState(Boss1State nextState)
	{
		if (mBossState == nextState)
			return;

		switch (nextState)
		{
		case hj::Boss1State::StartReady:
			return;
		case hj::Boss1State::Idle:
			ChangeAnimation(WIDE("Bellial_Idle"), true);
			break;
		case hj::Boss1State::Attack:
			SelectAttackPattern();
			break;
		case hj::Boss1State::Dead:
			ProcessDead();
			break;
		case hj::Boss1State::End:
		default:
			break;
		}
	}

	void Stage1Boss::ChangeAnimation(const std::wstring& animWstr, bool loop)
	{
		Animator* animator = this->GetComponent<Animator>();

		if (!animator)
			return;

		Animation* animation = animator->FindAnimation(animWstr);

		if (!animation)
			return;

		animator->Play(animWstr, loop);
	}

	void Stage1Boss::SelectAttackPattern()
	{
		auto curTime = std::chrono::system_clock::now();
		auto duration = curTime.time_since_epoch();
		auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
		std::mt19937 randomSeed(static_cast<UINT>(millis));

		std::uniform_int_distribution<UINT> patternRange(static_cast<UINT>(Boss1AttackPattern::SwordAttack)
														, static_cast<UINT>(Boss1AttackPattern::End));

		mBossAttackPattern = static_cast<Boss1AttackPattern>(patternRange(randomSeed));
	}

	void Stage1Boss::ExecuteAttackPattern()
	{
		bool bIsExecute = IsAttackEnd();;
		if (Boss1State::Attack != mBossState || bIsExecute)
			return;

		bool bValidAttackPattern = true;

		switch (mBossAttackPattern)
		{
		case hj::Boss1AttackPattern::SwordAttack:
			PatternSwordAttack();
			break;
		case hj::Boss1AttackPattern::BulletAttack:
			PatternBulletAttack();
			break;
		case hj::Boss1AttackPattern::LaserPattern1:
			PatternLaserAttack1();
			break;
		case hj::Boss1AttackPattern::LaserPattern2:
			PatternLaserAttack2();
			break;
		case hj::Boss1AttackPattern::End:
		default:
			bValidAttackPattern = false;
			break;
		}

		mbAttackExecuted = bValidAttackPattern;
	}

	void Stage1Boss::ProcessDead()
	{

	}

	void Stage1Boss::PatternSwordAttack()
	{

	}

	void Stage1Boss::PatternBulletAttack()
	{

	}

	void Stage1Boss::PatternLaserAttack1()
	{

	}

	void Stage1Boss::PatternLaserAttack2()
	{

	}

	void Stage1Boss::CreateBodyAnimation()
	{
		SpriteRenderer* sr = GetComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = sr->GetMaterial();
		std::shared_ptr<Texture> texture = material->GetTexture(eTextureSlot::T0);
		Vector2 atlasTexSize = texture->GetTexSize();

		float duration = 0.f;
		duration = 1.f / 10.f;

		XmlParser* parser = new XmlParser;
		std::wstring path = WIDE("02_Object/02_Monster/01_Boss/Monster_Boss_1_Bellial.xml");
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

		CREATE_ANIM(animBellialIdle, frame, atlasTexSize, duration);
		while (parseResult)
		{
			parseResult = parser->FindElem(WIDE("sprite"));
			if (!parseResult)
			{
				parseResult = true;
				count = 6;
				parser->ResetMainPos();

				break;
			}

			targetSpriteNameWstr = WIDE("SkellBossIdle") + std::to_wstring(count);
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
					, trimmedOffset.x, trimmedOffset.y, animBellialIdle);

				++count;
			}
		}

		CREATE_SHEET(animBellialAttack);
		while (parseResult)
		{
			parseResult = parser->FindElem(WIDE("sprite"));
			if (!parseResult)
			{
				parseResult = true;
				count = 0;
				parser->ResetMainPos();

				break;
			}

			targetSpriteNameWstr = WIDE("SkellBossAttack0") + std::to_wstring(count);
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
					, trimmedOffset.x, trimmedOffset.y, animBellialAttack);

				++count;
			}
		}


		CREATE_SHEET(animBellialAttackStart);
		while (parseResult)
		{
			parseResult = parser->FindElem(WIDE("sprite"));
			if (!parseResult || (6 == count))
			{
				parseResult = true;
				count = 5;
				parser->ResetMainPos();

				break;
			}

			targetSpriteNameWstr = WIDE("SkellBossAttack0") + std::to_wstring(count);
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
					, trimmedOffset.x, trimmedOffset.y ,animBellialAttackStart);

				++count;
			}
		}

		CREATE_SHEET(animBellialAttackEnd);
		while (parseResult)
		{
			parseResult = parser->FindElem(WIDE("sprite"));
			if (!parseResult || (-1 == count))
			{
				parseResult = true;
				count = 0;
				break;
			}

			targetSpriteNameWstr = WIDE("SkellBossAttack0") + std::to_wstring(count);
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
					, trimmedOffset.x, trimmedOffset.y, animBellialAttackEnd);

				--count;
				parser->ResetMainPos();
			}
		}

		Animator* animator = GetComponent<Animator>();

		std::wstring idleAnimWstr = WIDE("Bellial_Idle");
		std::wstring attackAnimWstr = WIDE("Bellial_Attack");
		std::wstring attackStartAnimWstr = WIDE("Bellial_AttackStart");
		std::wstring attackEndAnimWstr = WIDE("Bellial_AttackEnd");

		animator->Create(idleAnimWstr, texture, animBellialIdle, canvasSize, false);
		animator->Create(attackAnimWstr, texture, animBellialAttack, canvasSize, false);
		animator->Create(attackStartAnimWstr, texture, animBellialAttackStart, canvasSize, false);
		animator->Create(attackEndAnimWstr, texture, animBellialAttackEnd, canvasSize, false);

		animator->Play(idleAnimWstr, true);

		AddPositionX(-canvasSize.x * Bellial_Horn_X_Size_Ratio);
		SetScaleXY(canvasSize);
		GetTransform()->FixedUpdate();

		delete parser;
	}

	void Stage1Boss::ProcessDamaged(float damage)
	{

	}

	void Stage1Boss::Dead()
	{

	}

	void Stage1Boss::CreateJaws()
	{

	}

}