#include "hjSkeletonHand.h"
#include "hjAnimator.h"
#include "hjWeapon.h"
#include "hjSkeletonBase.h"
#include "hjCollider2D.h"
#include "hjObject.h"
#include "hjOakBow.h"

namespace hj
{
	SkeletonHand::SkeletonHand()
		: Hand()
	{
		SetName(WIDE("SkelHand"));

		if (mAnimator)
		{
			CreateAnimation();
		}
	}

	SkeletonHand::~SkeletonHand()
	{
	}

	void SkeletonHand::Initialize()
	{
		GameObject::Initialize();
	}

	void SkeletonHand::Update()
	{
		GameObject::Update();
	}

	void SkeletonHand::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void SkeletonHand::Render()
	{
		GameObject::Render();
	}

	void SkeletonHand::SetHandOwner(Actor* owner)
	{
		SkeletonBase* monster = dynamic_cast<SkeletonBase*>(owner);;
		if (!monster)
			return;

		mHandOwner = monster;
		mHandOwnerTR = monster->GetCenter()->GetTransform();

		mRotatorScript = monster->GetCenter()->GetScript<ArmRotatorScript>();
		if (mRotatorScript)
		{
			InitRotatorScript();
		}
	}

	Transform* SkeletonHand::GetWeaponTR()
	{
		if (mWeapon)
			return mWeapon->GetTransform();
		else
			return nullptr;
	}

	void SkeletonHand::Attack()
	{
		if (mWeapon)
		{
			object::item::weapon::Weapon* weaponPtr = dynamic_cast<object::item::weapon::Weapon*>(mWeapon);

			if (weaponPtr)
			{
				weaponPtr->Attack();
			}
		}
	}

	void SkeletonHand::CreateAnimation()
	{
		SetNameAndCreateSpriteRenderer(GetName(),
			WIDE("MTRL_Stage1"), WIDE("Mesh_Rect"));

		CheckHasMaterialAndTexture(eTextureSlot::T0);

		std::wstring path = WIDE("02_Object/02_Monster/00_Normal/Stage01.xml");
		std::wstring searchWstr = WIDE("SkelHand");
		std::wstring animWstr = WIDE("SkelHand");
		float frameDuration = 1.f;

		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD_OFFSET, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration);
		Actor::InsertAnimationToAnimator(eTextureSlot::T0);

		Animator* animator = GetComponent<Animator>();
		animator->Play(animWstr);
	}
}