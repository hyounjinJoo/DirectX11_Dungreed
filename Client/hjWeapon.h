#pragma once
#include "hjItem.h"
#include "hjTexture.h"

namespace hj::object::item::weapon
{
	enum class eWeaponType
	{
		None,
		Melee,
		Ranged,
	};

    struct weaponInfo
    {
        std::bitset<sizeof(UINT)> mWeaponID;
        std::string mWeaponName;
        int mAttack;
        float mAttackPerSec;
        eWeaponType mWeaponType;

        weaponInfo()
            : mAttack(1)
            , mAttackPerSec(1)
            , mWeaponType(eWeaponType::None)
        {
        }
    };

    class Weapon :
        public Item
    {
    public:
        Weapon() = delete;
        Weapon(eWeaponType type);
        virtual ~Weapon();

        virtual void Initialize() = 0;
        virtual void Update() = 0;
        virtual void FixedUpdate() = 0;
        virtual void Render() = 0;

    protected:
        std::shared_ptr<hj::graphics::Texture> mWeaponTex;
        weaponInfo mInfo;
    };
}