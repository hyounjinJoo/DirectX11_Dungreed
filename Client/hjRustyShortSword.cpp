#include "hjRustyShortSword.h"
#include "hjCollider2D.h"
#include "hjTime.h"
#include "hjSkeletonHand.h"
#include "hjAnimator.h"
#include "hjAttackScript.h"

namespace hj::object::item::weapon
{
	RustyShortSword::RustyShortSword()
		: Actor()
		, Weapon(eWeaponType::Melee, eItemClass::Common)
		, mAttackScript(nullptr)
	{
		SetName(WIDE("WEAPON_MONSTER_01_³ì½¼ ¼ô¼Òµå"));

		mItemName = std::string("³ì½¼ ¼ô¼Òµå");
		mItemDescription = std::string("½ºÄÌ·¹ÅæÀÌ »ç¿ëÇÏ´Â ³ì½¼ ¼ô¼Òµå");
		mWeaponInfo.mMinAttack = 5;
		mWeaponInfo.mMaxAttack = 6;
		mWeaponInfo.mAttackPerSec = 1.f;
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

	RustyShortSword::~RustyShortSword()
	{
		if (mAttackScript)
		{
			mAttackScript = nullptr;
		}
	}
	
	void RustyShortSword::Initialize()
	{
		Actor::Initialize();
	}
	
	void RustyShortSword::Update()
	{
		if (mAttackCoolTimer > 0.f)
		{
			mAttackCoolTimer -= Time::FixedDeltaTime();

			if (mAttackCoolTimer < 0.f)
				mAttackCoolTimer = 0.f;
		}

		Actor::Update();
	}
	
	void RustyShortSword::FixedUpdate()
	{
		Actor::FixedUpdate();
	}
	
	void RustyShortSword::Render()
	{
		Actor::Render();
	}
	
	void RustyShortSword::Attack()
	{
		if (mAttackCoolTimer > 0.f)
		{
			return;
		}

		mAttackCoolTimer = mWeaponInfo.mAttackPerSec;
		
		if(GetComponent<Animator>())
			GetComponent<Animator>()->Play(WIDE("RustyShortSwordAttack"), false);
	}
	
	void RustyShortSword::CreateAnimation()
	{
		CheckHasMaterialAndTexture(eTextureSlot::T0);

		std::wstring path = WIDE("02_Object/02_Monster/00_Normal/Stage01.xml");
		std::wstring searchWstr = WIDE("RustyShortSword0");
		std::wstring animWstr = WIDE("RustyShortSwordIdle");
		float frameDuration = 1.f;

		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD_OFFSET, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration);

		searchWstr = WIDE("RustyShortSword");
		animWstr = WIDE("RustyShortSwordAttack");
		frameDuration = 1.f / 16.f;
		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD_OFFSET, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration, true, 1);
		
		Actor::InsertAnimationToAnimator(eTextureSlot::T0);

		Animator* animator = GetComponent<Animator>();
		if (animator)
		{
			animator->Play(WIDE("RustyShortSwordIdle"));

			animator->GetEvent(WIDE("RustyShortSwordAttack"), 6) = std::bind(&RustyShortSword::DamageOn, this);
			animator->GetCompleteEvent(WIDE("RustyShortSwordAttack")) = std::bind(&RustyShortSword::DamageOff, this);
		}

	}
	void RustyShortSword::DamageOn()
	{
		if (mAttackScript)
		{
			mAttackScript->SetDamageOn(true);
		}
	}
	void RustyShortSword::DamageOff()
	{
		if (mAttackScript)
		{
			mAttackScript->ClearDamagedObjects();
			mAttackScript->SetDamageOn(false);
			Animator* animator = GetComponent<Animator>();
			
			if(animator)
				animator->Play(WIDE("RustyShortSwordIdle"));
		}
	}
}