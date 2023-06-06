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
#include "hjPlayer.h"

namespace hj
{
#define Bellial_Horn_X_Size_Ratio 0.10714f
#define Bellial_Horn_Y_Size_Ratio 0.11718f
#define Bellial_Max_HP 200.f

	Stage1Boss::Stage1Boss()
		: mMaximumHP(Bellial_Max_HP)
		, mCurrentHP(Bellial_Max_HP)
		, mBossState(Boss1State::StartReady)
		, mBossAttackPattern(Boss1AttackPattern::End)
		, mLeftHand(nullptr)
		, mRightHand(nullptr)
		, mLaser(nullptr)
		, mBodyAnimator(nullptr)
		, mbAttackExecuted(false)
		, mAuraManager(nullptr)
	{
		SetName(WIDE("Boss_Bellial"));
		
		SetPositionY(-80.f);

		// 1. Sprite Renderer ����
		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = MTRL_FIND_STR("MTRL_Monster_Boss_Bellial");
		std::shared_ptr<Mesh> mesh = MESH_FIND_STR("Mesh_Rect");
		sr->SetMaterial(material);
		sr->SetMesh(mesh);

		//AddComponent<Collider2D>();

		// 2. Body Animator ���� �� Animation �߰�
		Animator* animator = AddComponent<Animator>();
		if (material && animator)
		{
			std::shared_ptr<Texture> texture = material->GetTexture(eTextureSlot::T0);
			if (texture)
			{
				CreateBodyAnimation();
			}
		}

		// 3. Damage ������ ��ü�� �浹ü ����
		mDamageBody = object::Instantiate<GameObject>(eLayerType::Monster);

		Vector2 bodySize = GetComponent<Animator>()->GetCurrentAnimation()->GetCurrentSpriteSize();
		bodySize.x *= 0.785f;
		mDamageBody->SetScaleXY(bodySize);
		float movedPos = GetScaleX() * Bellial_Horn_X_Size_Ratio;
		mDamageBody->AddPositionX(movedPos);
		mDamageBody->GetTransform()->SetParent(this->GetTransform());

		mDamageBody->GetTransform()->FixedUpdate();

		mDamageCollider = mDamageBody->AddComponent<Collider2D>();

		// 4. Left Hand, Right Hand ����
		mLeftHand = object::Instantiate<Stage1BossHand>(eLayerType::MonsterHas, Vector3(80.f * -7.f, 80.f * -3.f, 0.1f));
		mRightHand = object::Instantiate<Stage1BossHand>(eLayerType::MonsterHas, Vector3(80.f * 7.f, 0.f, 0.1f));

		mLeftHand->SetInitialHandPosY(mLeftHand->GetPositionY());
		mRightHand->SetInitialHandPosY(mRightHand->GetPositionY());

		mLeftHand->ChangeHandType(Boss1HandType::Left);
		mRightHand->ChangeHandType(Boss1HandType::Right);

		mLeftHand->ChangeHandState(Boss1HandState::Idle);
		mRightHand->ChangeHandState(Boss1HandState::Idle);


		// 5. SkellBossBack �̹��� ��ü ���� 
		mBackground = object::Instantiate<GameObject>(eLayerType::MonsterHas, Vector3(GetScaleX() * 0.5f, 0.f, 1.f));
		// 6. Effect ��ü ����
		{
		//	(1) DieFX ���� ��ü ����
		//	(2) BackgroundParticle �̹��� ��ü ���� Ȥ�� ��ƼŬ ����
			mBackground = object::Instantiate<GameObject>(eLayerType::MonsterHas, Vector3(GetScaleX() * 0.5f, 0.f, 1.f));
		}
		// 7. Pattern 1(Summon Sword)�� �ʿ��� �͵� ����
		CreateSwords();
		
		// 8. Pattern 2(Fire Bullets)�� �ʿ��� �͵� ����
		mBulletMuzzle = object::Instantiate<Boss1BulletMuzzle>(eLayerType::MonsterHas);
		mBulletMuzzle->SetPositionXY(Vector2(movedPos * 0.5f, -80.f));
		mBulletMuzzle->GetTransform()->SetParent(this->GetTransform());
		// 9. Pattern 3(Shot Laser)�� �ʿ��� �͵� ����
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
	}

	void Stage1Boss::FixedUpdate()
	{
		static float testTime = 0.f;
		static float testMaxTime = 5.f;

		AdjustColliderPosAndSize();

		if (Boss1State::AttackEnd == mBossState)
		{
			if (mCurIdleTime < mIdleLimitTime)
			{
				mCurIdleTime += Time::FixedDeltaTime();
			}
			else
			{
				mCurIdleTime = 0.f;
				ChangeBoss1State(Boss1State::Attack);
			}
		}
		else if (Boss1State::Attack == mBossState)
		{
			ExecuteAttackPattern();
		}
		else if (Boss1State::StartReady == mBossState)
		{
			if (mTestSpawnBossTimer < mTestSpawnBossTimeLimit)
			{
				mTestSpawnBossTimer += Time::FixedDeltaTime();
			}
			else
			{
				mTestSpawnBossTimer = 0.f;
				ChangeBoss1State(Boss1State::Attack);
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

	void Stage1Boss::ChangeBoss1State(Boss1State nextState)
	{
		if (mBossState == nextState)
			return;

		mBossState = nextState;

		switch (nextState)
		{
		case hj::Boss1State::StartReady:
			// ������ �׽�Ʈ�� ���ؼ� �ٷ� ���º���
			ChangeBoss1State(Boss1State::Attack);
			return;
		case hj::Boss1State::Idle:
			ChangeAnimation(WIDE("Bellial_Idle"), true);
			break;
		case hj::Boss1State::Attack:
			SelectAttackPattern();
			break;
		case hj::Boss1State::AttackEnd:
			EndAttackPattern();
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
														, static_cast<UINT>(Boss1AttackPattern::End) - 1);

		mBossAttackPattern = static_cast<Boss1AttackPattern>(patternRange(randomSeed));
	}

	void Stage1Boss::EndAttackPattern()
	{
		mbAttackExecuted = true;
		mCurIdleTime = 0.f;
		ChangeAnimation(WIDE("Bellial_Idle"), true);

		switch (mBossAttackPattern)
		{
		case hj::Boss1AttackPattern::SwordAttack:
			break;
		case hj::Boss1AttackPattern::BulletAttack:
			break;
		case hj::Boss1AttackPattern::LaserPattern1:
			break;
		case hj::Boss1AttackPattern::LaserPattern2:
			break;
		case hj::Boss1AttackPattern::End:
		default:
			break;
		}
	}

	void Stage1Boss::ExecuteAttackPattern()
	{
		if (Boss1State::Attack != mBossState)
		{
			return;
		}

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
			break;
		}
	}

	void Stage1Boss::ProcessDead()
	{

	}

	void Stage1Boss::PatternSwordAttack()
	{
		if (mCurSwordSpawnTimer < mSwordSpawnInterval)
		{
			mCurSwordSpawnTimer += Time::FixedDeltaTime();
			return;
		}
		mCurSwordSpawnTimer = 0.f;
		static size_t nextSpawnSwordIndex = 0;

		mSwords[nextSpawnSwordIndex++]->ChangeSwordState(Boss1SwordState::Spawn);

		if (nextSpawnSwordIndex >= mSwords.size())
		{
			nextSpawnSwordIndex = 0; 
			ChangeBoss1State(Boss1State::AttackEnd);
		}

	}

	void Stage1Boss::PatternBulletAttack()
	{
		Animator* animator = GetComponent<Animator>();
		if (!animator)
		{
			ChangeBoss1State(Boss1State::AttackEnd);
			return;
		}

		Animation* animation = animator->GetCurrentAnimation();
		if (!animation)
		{
			ChangeBoss1State(Boss1State::AttackEnd);
			return;
		}

		std::wstring animName = animation->AnimationName();
		if (animName == WIDE("Bellial_Attack"))
		{
			if (mBulletMuzzle)
			{
				MuzzleState muzzleState = mBulletMuzzle->GetMuzzleState();

				if (MuzzleState::ShotEnd == muzzleState)
				{
					mBulletMuzzle->ChangeMuzzleState(MuzzleState::ReadyToShot);
					animator->Play(WIDE("Bellial_AttackEnd"), false);
				}
				else if (MuzzleState::ReadyToShot == muzzleState)
				{
					mBulletMuzzle->ChangeMuzzleState(MuzzleState::ShotStart);
				}
			}
		}
		else if (animName == WIDE("Bellial_Idle"))
		{
			animator->Play(WIDE("Bellial_AttackStart"), false);
		}
	}

#define NOTNEEDTOMOVEIDLE	false
#define NEEDTOMOVEIDLE		true
	void Stage1Boss::PatternLaserAttack1()
	{
		bool bIsShotEnd = ProcessLaserAttackAndCheckShotEnd(mLeftHand, NEEDTOMOVEIDLE);
		if (bIsShotEnd)
		{
			ChangeBoss1State(Boss1State::AttackEnd);
		}
	}

	void Stage1Boss::PatternLaserAttack2()
	{
		static int remainShotCount = 3;
		bool bIsShotEnd = false;
		switch (remainShotCount)
		{
		case 3:
		{
			bIsShotEnd = ProcessLaserAttackAndCheckShotEnd(mRightHand, NOTNEEDTOMOVEIDLE);
			if (bIsShotEnd)
			{
				remainShotCount -= 1;
			}

			break;
		}
		case 2:
		{
			bIsShotEnd = ProcessLaserAttackAndCheckShotEnd(mLeftHand, NOTNEEDTOMOVEIDLE);
			if (bIsShotEnd)
			{
				remainShotCount -= 1;
			}

			break;
		}
		case 1:
		{
			bIsShotEnd = ProcessLaserAttackAndCheckShotEnd(mRightHand, NEEDTOMOVEIDLE);
			if (bIsShotEnd)
			{
				mLeftHand->EndAttack(NEEDTOMOVEIDLE);
				remainShotCount -= 1;
			}

			break;
		}
		case 0:
		default:
		{
			remainShotCount = 3;
			ChangeBoss1State(Boss1State::AttackEnd);
		}
			break;
		}
	}

	bool Stage1Boss::ProcessLaserAttackAndCheckShotEnd(Stage1BossHand* HandToShot, bool bIsNeedToMoveIdle)
	{
		bool bIsEnd = false;
		if (!HandToShot)
		{
			bIsEnd = true;
			return bIsEnd;
		}

		if (Boss1HandState::Idle == HandToShot->GetHandState())
		{
			std::vector<GameObject*> playerLayerObjs = SceneManager::GetActiveScene()->GetGameObjects(eLayerType::Player);

			for (GameObject* iter : playerLayerObjs)
			{
				if (dynamic_cast<Player*>(iter))
				{
					HandToShot->AttackStart(iter->GetPositionY());
				}
			}
		}
		else if (HandToShot->IsAttackEnd())
		{
			bIsEnd = true;

			HandToShot->EndAttack(bIsNeedToMoveIdle);			
		}

		return bIsEnd;
	}

	void Stage1Boss::CreateSwords()
	{
#define TILESIZE 80.f
		Boss1Sword* boss1Sword = object::Instantiate<Boss1Sword>(eLayerType::MonsterAttack_ForeGround);
		Vector2 PosOffset = Vector2(TILESIZE * 2.f, 0.f);
		Vector2 PosOffsetY = Vector2(0.f, TILESIZE);

		int iter = -3;

		boss1Sword->SetSpawnPosXY(PosOffset * static_cast<float>(iter) + PosOffsetY);
		boss1Sword->SetPositionXY(PosOffset * static_cast<float>(iter) + PosOffsetY);
		boss1Sword->ChangeSwordState(Boss1SwordState::End);
		mSwords.push_back(boss1Sword);

		
		++iter;
		int maxSwordOffset = 4;
		for (; iter < maxSwordOffset; ++iter)
		{
			boss1Sword = object::Clone<Boss1Sword>(boss1Sword);
		
			boss1Sword->SetSpawnPosXY(PosOffset * static_cast<float>(iter) + PosOffsetY);
			boss1Sword->SetPositionXY(PosOffset * static_cast<float>(iter) + PosOffsetY);
			boss1Sword->ChangeSwordState(Boss1SwordState::End);
			mSwords.push_back(boss1Sword);
		
			// ��ġ�� ������ ���� -3, -2, -1, 1, 2, 3�� �ǵ��� ó��
			if (-1 == iter)
			{
				++iter;
			}
		}
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

		animator->GetCompleteEvent(attackStartAnimWstr) = std::bind(&Stage1Boss::ChangeAnimation, this, WIDE("Bellial_Attack"), true);
		animator->GetCompleteEvent(attackEndAnimWstr) = std::bind(&Stage1Boss::ChangeBoss1State, this, Boss1State::AttackEnd);
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