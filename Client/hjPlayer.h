#pragma once
#include "hjGameObject.h"

#include "hjAnimator.h"
namespace hj
{
    enum class ePlayerCostume
    {
        Adventurer,
        Alice,
        ArmoredWarrior,
        CriminalSilhouette,
        DevilSwordsman,
        Fatso,
        HumanLasley,
        IkinaBear,
        KingOfPickaxes,
        Lotus,
        MasterChef,
        RiderH,
        SunsetGunman,
        UnUsed_1,
        UnUsed_2,
        UnUsed_3,
        End
    };

    enum class ePlayerState
    {
        Idle,
        Jump,
        Run,
        Die,
        End,
    };

    class Player :
        public GameObject
    {
    public:
        Player();
        virtual ~Player();

        void Initialize() override;
        void Update() override;
        void FixedUpdate() override;
        void Render() override;
        void ChangeState(ePlayerState state);

    private:
        class PlayerHand* mHand;
        ePlayerState mState;
        class Animator* mAnimator;
        class RigidBody* mRigidBody;

        void CreateAnimation();

        void Idle();
        void Run();
        void Jump();
    };
}
