#pragma once
#include "hjItem.h"
#include "hjTexture.h"

class Actor;
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
        bool mbIsNeedToRotX;
        bool mbUseOriginAngle;
        bool mbNotAllowMinusHandPosX;
#pragma endregion

        weaponInfo()
            : mWeaponID(UINT_MAX)
            , mMinAttack(1)
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
            , mbIsNeedToRotX(true)
            , mbUseOriginAngle(false)
            , mbNotAllowMinusHandPosX(false)
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

        virtual void Attack() {};

    public:
        float GetFirstOffsetAngle() { return mWeaponInfo.mFirstOffsetAngle; }
        float GetSecondOffsetAngle() { return mWeaponInfo.mSecondOffsetAngle; }
        Vector2 GetOffsetAngle() { return Vector2(mWeaponInfo.mFirstOffsetAngle, mWeaponInfo.mSecondOffsetAngle); }
        Vector2 GetFirstMinDistance() { return mWeaponInfo.mFirstMinDistance; }
		Vector2 GetFirstMaxDistance() { return mWeaponInfo.mFirstMaxDistance; }
		Vector2 GetSecondMinDistance() { return mWeaponInfo.mSecondMinDistance; }
		Vector2 GetSecondMaxDistance() { return mWeaponInfo.mSecondMaxDistance; }
        bool GetIsNeedToRotX() { return mWeaponInfo.mbIsNeedToRotX; }
        bool GetIsUseManualDistance() { return mWeaponInfo.mbUseManualDistance; }
        float GetManualDistance() { return mWeaponInfo.mManualDistance; }
        eWeaponType GetWeaponType() { return mWeaponInfo.mWeaponType; }
        bool GetIsUseOriginAngle() { return mWeaponInfo.mbUseOriginAngle; }
        bool GetIsNotAllowMinusHandPosX() { return mWeaponInfo.mbNotAllowMinusHandPosX; }

        const weaponInfo& GetWeaponInfo() { return mWeaponInfo; }

        void SetHand(Actor* hand)
        {
            if (hand)
            {
                mHand = hand;
            }
        }

    protected:
        weaponInfo mWeaponInfo;
        Actor* mHand;

    protected:
		float mAttackCoolTimer;

    public:
        static UINT mID;
    };
}