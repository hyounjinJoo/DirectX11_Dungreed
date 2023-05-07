#include "hjWeapon.h"

namespace hj::object::item::weapon
{
	Weapon::Weapon(eWeaponType type)
		: Item(eItemType::Weapon)
		, mWeaponInfo{}
	{
		mWeaponInfo.mWeaponType = type;
	}

	Weapon::~Weapon()
	{
	}
}