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
        UINT mWeaponID;
        int mMinAttack;
        int mMaxAttack;
        float mAttackPerSec;
        eWeaponType mWeaponType;

#pragma region ArmRotationInfo
        bool mbUseManualDistance;
        float mManualDistance;
        float mFirstOffsetAngle;
        float mSecondOffsetAngle;
        Vector2 mFirstMinDistance;
        Vector2 mFirstMaxDistance;
		Vector2 mSecondMinDistance;
		Vector2 mSecondMaxDistance;
#pragma endregion

        weaponInfo()
            : mMinAttack(1)
            , mMaxAttack(1)
            , mAttackPerSec(1)
            , mWeaponType(eWeaponType::None)
            , mbUseManualDistance(false)
            , mManualDistance(0.f)
            , mFirstOffsetAngle(0.f)
            , mSecondOffsetAngle(0.f)
			, mFirstMinDistance(Vector2::Zero)
			, mFirstMaxDistance(Vector2::Zero)
			, mSecondMinDistance(Vector2::Zero)
			, mSecondMaxDistance(Vector2::Zero)
        {
        }
    };

    class Weapon :
        public Item
    {
    public:
        Weapon() = delete;
        Weapon(eWeaponType type, eItemClass eClass);
        virtual ~Weapon();

        virtual void Attack() = 0;

    public:
        float GetFirstOffsetAngle() { return mWeaponInfo.mFirstOffsetAngle; }
        float GetSecondOffsetAngle() { return mWeaponInfo.mSecondOffsetAngle; }
        Vector2 GetOffsetAngle() { return Vector2(mWeaponInfo.mFirstOffsetAngle, mWeaponInfo.mSecondOffsetAngle); }
        Vector2 GetFirstMinDistance() { return mWeaponInfo.mFirstMinDistance; }
		Vector2 GetFirstMaxDistance() { return mWeaponInfo.mFirstMaxDistance; }
		Vector2 GetSecondMinDistance() { return mWeaponInfo.mSecondMinDistance; }
		Vector2 GetSecondMaxDistance() { return mWeaponInfo.mSecondMaxDistance; }

        const weaponInfo& GetWeaponInfo() { return mWeaponInfo; }
    protected:
        weaponInfo mWeaponInfo;

    public:
        static UINT mID;
    };
}