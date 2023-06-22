#include "hjWeapon.h"
#include "hjActor.h"

namespace hj::object::item::weapon
{
	UINT Weapon::mID = 1;

	Weapon::Weapon(eWeaponType type, eItemClass eClass)
		: Item(eItemType::Weapon, eClass)
		, mWeaponInfo{}
		, mAttackCoolTimer(0.f)
	{
		mWeaponInfo.mWeaponID = mID++;
		mWeaponInfo.mWeaponType = type;
	}

	Weapon::~Weapon()
	{
	}
}