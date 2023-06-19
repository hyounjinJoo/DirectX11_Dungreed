#include "hjSwordOfExplorer.h"
#include "hjAnimator.h"
#include "hjCollider2D.h"
#include "hjSwingFx.h"
#include "hjObject.h"

namespace hj::object::item::weapon
{
	SwordOfExplorer::SwordOfExplorer()
		: Actor()
		, Weapon(eWeaponType::Melee, eItemClass::Common)
		, mAttackFx(nullptr)
	{
		SetName(WIDE("WEAPON_MEELEE_01_모험가의 검"));

		mItemName = std::string("모험가의 검");
		mItemDescription = std::string("잘 손질된 모험가의 검");
		mWeaponInfo.mMinAttack = 10;
		mWeaponInfo.mMaxAttack = 11;
		mWeaponInfo.mAttackPerSec = 3.3f;
		mWeaponInfo.mbUseManualDistance = false;
		mWeaponInfo.mManualDistance = 0.f;
		mWeaponInfo.mFirstOffsetAngle = 20.f;
		mWeaponInfo.mSecondOffsetAngle = -200.f;
		mWeaponInfo.mFirstMinDistance = Vector2(-10.f, 10.f);
		mWeaponInfo.mFirstMaxDistance = Vector2(35.f, 45.f);

		mWeaponInfo.mSecondMinDistance = Vector2(25.f, -40.f);
		mWeaponInfo.mSecondMaxDistance = Vector2(2.f, 25.f);

		SetNameAndCreateSpriteRenderer(GetName(),
			WIDE("MTRL_Weapon_Common"), WIDE("Mesh_Rect"));

		// 2. Animator 생성 및 Animation 추가
		Animator* animator = AddComponent<Animator>();
		if (animator)
		{
			CreateAnimation();
		}

		AddComponent<Collider2D>();

		mAttackFx = object::Instantiate<SwingFx>(eLayerType::PlayerAttack_NotForeGround);
		mAttackFx->SetDamageRange(mWeaponInfo.mMinAttack, mWeaponInfo.mMaxAttack);
	}

	SwordOfExplorer::~SwordOfExplorer()
	{
	}
	
	void SwordOfExplorer::Initialize()
	{
		Actor::Initialize();
	}
	
	void SwordOfExplorer::Update()
	{
		Actor::Update();
	}
	
	void SwordOfExplorer::FixedUpdate()
	{
		Actor::FixedUpdate();
	}
	
	void SwordOfExplorer::Render()
	{
		Actor::Render();
	}

	void SwordOfExplorer::Attack()
	{
		mAttackFx->PlayFx();
	}

	void SwordOfExplorer::CreateAnimation()
	{
		CheckHasMaterialAndTexture(eTextureSlot::T0);

		std::wstring path = WIDE("02_Object/03_Weapon/Weapon_Common.xml");
		std::wstring searchWstr = WIDE("SwordOfExplorer");
		std::wstring animWstr = WIDE("SwordOfExplorer");
		float frameDuration = 1.f / (mWeaponInfo.mAttackPerSec);

		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD_OFFSET, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration);
		Actor::InsertAnimationToAnimator(eTextureSlot::T0);

		GetComponent<Animator>()->Play(animWstr);
	}

	hj::Actor* SwordOfExplorer::GetFxActor()
	{
		Actor* result = nullptr;

		if (mAttackFx)
		{
			result = mAttackFx;
		}

		return result;
	}

}