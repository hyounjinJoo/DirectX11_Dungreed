#include "hjWeapon.h"

namespace hj::object::item::weapon
{
	UINT Weapon::mID = 1;

	Weapon::Weapon(eWeaponType type, eItemClass eClass)
		: Item(eItemType::Weapon, eClass)
		, mWeaponInfo{}
	{
		mWeaponInfo.mWeaponID = mID++;
		mWeaponInfo.mWeaponType = type;
	}

	Weapon::~Weapon()
	{
	}
}