#include "hjOakBow.h"
#include "hjAnimator.h"
#include "hjObject.h"
#include "hjTime.h"
#include "hjPlayerHand.h"

namespace hj::object::item::weapon
{
	OakBow::OakBow()
		: Actor()
		, Weapon(eWeaponType::Ranged, eItemClass::Common)
		, mAttackCoolTime(0.f)
	{
		SetName(WIDE("WEAPON_RANGED_01_떡갈나무 활"));

		mItemName = std::string("떡갈나무 활");
		mItemDescription = std::string("단단해서 때리는 데 쓸 수도 있을 것 같다.");
		mWeaponInfo.mMinAttack = 20;
		mWeaponInfo.mMaxAttack = 35;
		mWeaponInfo.mAttackPerSec = 1.72f;
		mWeaponInfo.mbUseManualDistance = true;
		mWeaponInfo.mManualDistance = 0.f;
		mWeaponInfo.mFirstOffsetAngle = 0.f;
		mWeaponInfo.mSecondOffsetAngle = 0.f;
		mWeaponInfo.mFirstMinDistance = Vector2(30.f, 0.f);
		mWeaponInfo.mFirstMaxDistance = Vector2(30.f, 0.f);

		mWeaponInfo.mSecondMinDistance = Vector2::Zero;
		mWeaponInfo.mSecondMaxDistance = Vector2::Zero;
		mWeaponInfo.mbIsNeedToRotX = false;
		mWeaponInfo.mbUseOriginAngle = true;
		mWeaponInfo.mbNotAllowMinusHandPosX = true;

		SetNameAndCreateSpriteRenderer(GetName(),
			WIDE("MTRL_Weapon_Common"), WIDE("Mesh_Rect"));

		// 2. Animator 생성 및 Animation 추가
		Animator* animator = AddComponent<Animator>();
		if (animator)
		{
			CreateAnimation();
		}
	}

	OakBow::~OakBow()
	{
	}
	
	void OakBow::Attack()
	{
		if (mAttackCoolTimer > 0.f)
		{
			return;
		}

		mAttackCoolTimer = mAttackCoolTime;

		GetComponent<Animator>()->Play(WIDE("OakBowAttack"));
	}
	
	void OakBow::CreateAnimation()
	{
		CheckHasMaterialAndTexture(eTextureSlot::T0);

		std::wstring path = WIDE("02_Object/03_Weapon/Weapon_Common.xml");
		std::wstring searchWstr = WIDE("OakBowIdle");
		std::wstring animWstr = WIDE("OakBowIdle");
		float frameDuration = 1.f / (mWeaponInfo.mAttackPerSec * 18.f);

		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD_OFFSET, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration);
		
		searchWstr = WIDE("OakBowAttack");
		animWstr = WIDE("OakBowAttack");
		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD_OFFSET, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration, true);
		Actor::InsertAnimationToAnimator(eTextureSlot::T0);

		Animator* animator = GetComponent<Animator>();
		animator->Play(WIDE("OakBowIdle"));

		animator->GetEvent(animWstr, 4) = std::bind(&OakBow::Shot, this);
		animator->GetCompleteEvent(animWstr) = std::bind(&OakBow::ChangeToIdle, this);

		Animation* attackAnim = animator->FindAnimation(WIDE("OakBowAttack"));
		if (attackAnim)
		{
			attackAnim->ChangePlayDuration(3, 0.5f);
		}

		float animPlayTime = attackAnim->GetTotalPlayTime();
		mAttackCoolTime = animPlayTime + (1.f / mWeaponInfo.mAttackPerSec);
	}

	void OakBow::ChangeToIdle()
	{
		Animator* animator = GetComponent<Animator>();

		if(animator)
			animator->Play(WIDE("OakBowIdle"));
	}

	void OakBow::Shot()
	{
		Arrow* arrow = object::Instantiate<Arrow>(eLayerType::PlayerAttack_ForeGround);
		if (arrow)
		{
			arrow->SetPositionXY(GetWorldPositionXY());
			float radian = GetWorldRotationZ();
			radian -= (XM_PI * 0.5f);

			if (radian == -0.f)
				radian = 0.f;

			arrow->SetRotationZ(radian);
			arrow->GetTransform()->FixedUpdate();
			arrow->DamageOn();
		}
	}

	void OakBow::Update()
	{
		if (mAttackCoolTimer > 0.f)
		{
			mAttackCoolTimer -= Time::FixedDeltaTime();

			if (mAttackCoolTimer < 0.f)
				mAttackCoolTimer = 0.f;
		}

		if (mHand)
		{
			PlayerHand* playerHand = reinterpret_cast<PlayerHand*>(mHand);
			if (playerHand)
			{
				float handPosZ = playerHand->GetPositionZ();
				if (handPosZ > 0.f)
				{
					playerHand->SetPositionZ(-0.2f);
				}
			}
		}

		Actor::Update();
	}

}