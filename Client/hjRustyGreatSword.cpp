#include "hjRustyGreatSword.h"
#include "hjCollider2D.h"
#include "hjTime.h"
#include "hjSkeletonHand.h"
#include "hjAnimator.h"
#include "hjAttackScript.h"

namespace hj::object::item::weapon
{
	RustyGreatSword::RustyGreatSword()
		: Actor()
		, Weapon(eWeaponType::Melee, eItemClass::Common)
		, mAttackScript(nullptr)
	{
		SetName(WIDE("WEAPON_MONSTER_01_³ì½¼ ´ë°Ë"));

		mItemName = std::string("³ì½¼ ´ë°Ë");
		mItemDescription = std::string("½ºÄÌ·¹ÅæÀÌ »ç¿ëÇÏ´Â ³ì½¼ ´ë°Ë");
		mWeaponInfo.mMinAttack = 10;
		mWeaponInfo.mMaxAttack = 12;
		mWeaponInfo.mAttackPerSec = 1.5f;
		mWeaponInfo.mbUseManualDistance = true;
		mWeaponInfo.mManualDistance = 0.f;
		mWeaponInfo.mFirstOffsetAngle = 0.f;
		mWeaponInfo.mSecondOffsetAngle = 0.f;
		mWeaponInfo.mFirstMinDistance = Vector2(30.f, 0.f);
		mWeaponInfo.mFirstMaxDistance = Vector2(30.f, 0.f);

		mWeaponInfo.mSecondMinDistance = Vector2::Zero;
		mWeaponInfo.mSecondMaxDistance = Vector2::Zero;
		mWeaponInfo.mbIsNeedToRotX = false;
		mWeaponInfo.mbUseOriginAngle = false;
		mWeaponInfo.mbNotAllowMinusHandPosX = true;

		SetNameAndCreateSpriteRenderer(GetName(),
			WIDE("MTRL_Stage1"), WIDE("Mesh_Rect"));

		// 2. Animator »ý¼º ¹× Animation Ãß°¡
		Animator* animator = AddComponent<Animator>();
		if (animator)
		{
			CreateAnimation();
		}

		AddComponent<Collider2D>();
		mAttackScript = AddComponent<AttackScript>();
		mAttackScript->SetDamageRange(mWeaponInfo.mMinAttack, mWeaponInfo.mMaxAttack);
	}

	RustyGreatSword::~RustyGreatSword()
	{
		if (mAttackScript)
		{
			mAttackScript = nullptr;
		}
	}

	void RustyGreatSword::Initialize()
	{
		Actor::Initialize();
	}

	void RustyGreatSword::Update()
	{
		if (mAttackCoolTimer > 0.f)
		{
			mAttackCoolTimer -= Time::FixedDeltaTime();

			if (mAttackCoolTimer < 0.f)
				mAttackCoolTimer = 0.f;
		}

		Actor::Update();
	}

	void RustyGreatSword::FixedUpdate()
	{
		Actor::FixedUpdate();
	}

	void RustyGreatSword::Render()
	{
		Actor::Render();
	}

	void RustyGreatSword::Attack()
	{
		if (mAttackCoolTimer > 0.f)
		{
			return;
		}

		mAttackCoolTimer = mWeaponInfo.mAttackPerSec;

		if (GetComponent<Animator>())
			GetComponent<Animator>()->Play(WIDE("RustyGreatSwordAttack"), false);
	}

	void RustyGreatSword::CreateAnimation()
	{
		CheckHasMaterialAndTexture(eTextureSlot::T0);

		std::wstring path = WIDE("02_Object/02_Monster/00_Normal/Stage01.xml");
		std::wstring searchWstr = WIDE("RustyGreatSword0");
		std::wstring animWstr = WIDE("RustyGreatSwordIdle");
		float frameDuration = 1.f;

		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD_OFFSET, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration);

		searchWstr = WIDE("RustyGreatSword");
		animWstr = WIDE("RustyGreatSwordAttack");
		frameDuration = 1.f / 16.f;
		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD_OFFSET, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration, true, 1);

		Actor::InsertAnimationToAnimator(eTextureSlot::T0);

		Animator* animator = GetComponent<Animator>();
		if (animator)
		{
			animator->Play(WIDE("RustyGreatSwordIdle"));

			animator->GetEvent(WIDE("RustyGreatSwordAttack"), 5) = std::bind(&RustyGreatSword::DamageOn, this);
			animator->GetEvent(WIDE("RustyGreatSwordAttack"), 8) = std::bind(&RustyGreatSword::DamageOff, this);
		}

	}
	void RustyGreatSword::DamageOn()
	{
		if (mAttackScript)
		{
			mAttackScript->SetDamageOn(true);
		}
	}
	void RustyGreatSword::DamageOff()
	{
		if (mAttackScript)
		{
			mAttackScript->ClearDamagedObjects();
			mAttackScript->SetDamageOn(false);
			Animator* animator = GetComponent<Animator>();

			if (animator)
				animator->Play(WIDE("RustyGreatSwordIdle"));
		}
	}
}