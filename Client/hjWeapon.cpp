#include "hjWeapon.h"

namespace hj::object::item::weapon
{
	Weapon::Weapon(eWeaponType type)
		: Item(eItemType::Weapon)
		, mInfo{}
	{
		mInfo.mWeaponType = type;
	}

	Weapon::~Weapon()
	{
	}
}