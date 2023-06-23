#include "hjHand.h"
#include "hjCollider2D.h"
#include "hjAnimator.h"
#include "hjWeapon.h"

namespace hj
{
	Hand::Hand()
		: Actor()
		, mHandOwner(nullptr)
		, mHandOwnerTR(nullptr)
		, mRotatorScript(nullptr)
		, mAnimator(nullptr)
		, mWeapon(nullptr)
	{
		SetName(WIDE("SkelHand"));

		AddComponent<Collider2D>();
		SetScaleXY(Vector2(12.f, 12.f));

		mAnimator = AddComponent<Animator>();
	}

	Hand::~Hand()
	{
	}
	
	void Hand::Initialize()
	{
		GameObject::Initialize();
	}
	
	void Hand::Update()
	{
		GameObject::Update();
	}
	
	void Hand::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}
	
	void Hand::Render()
	{
		GameObject::Render();
	}
	
	void Hand::SetWeapon(Actor* weapon)
	{
		if (weapon)
		{
			mWeapon = weapon;
			mWeapon->GetTransform()->SetParent(this->GetTransform());
			InitRotatorScript();
		}
	}

	Transform* Hand::GetWeaponTR()
	{
		if (mWeapon)
			return mWeapon->GetTransform();
		else
			return nullptr;
	}

	void Hand::InitRotatorScript()
	{
		if (!mRotatorScript || !mWeapon)
			return;

		object::item::weapon::Weapon* weaponPtr = dynamic_cast<object::item::weapon::Weapon*>(mWeapon);
		if (weaponPtr)
		{
			weaponPtr->SetHand(this);

			mRotatorScript->SetArmRotatorFactor(weaponPtr->GetOffsetAngle().x, weaponPtr->GetIsUseManualDistance()
				, weaponPtr->GetIsUseManualDistance(), weaponPtr->GetIsUseOriginAngle(), weaponPtr->GetIsNotAllowMinusHandPosX());

			Vector2 MinDistance = weaponPtr->GetFirstMinDistance();
			Vector2 MaxDistance = weaponPtr->GetFirstMaxDistance();
			mRotatorScript->SetDistanceInfo(MinDistance, MaxDistance);
		}
	}

}