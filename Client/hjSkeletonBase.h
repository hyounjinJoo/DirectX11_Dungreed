#pragma once
#include "hjMonster.h"

namespace hj
{
	class SkeletonBase :
		public Monster
	{
	public: 
		SkeletonBase();
		virtual ~SkeletonBase();

	private:
		class Weapon* mWeapon;
		class Player* mPlayer;
	};
}