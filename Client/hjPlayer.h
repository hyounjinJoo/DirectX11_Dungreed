#pragma once
#include "hjGameObject.h"

#include "hjAnimator.h"
#include "hjMaterial.h"
#include "hjTexture.h"
namespace hj
{
    enum class ePlayerCostume
    {
        Adventurer = 0,
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
        End
	};

	struct Costume
	{
        // 보유 애니메이션 이름
        std::wstring costumeIdleAnim;
		std::wstring costumeJumpAnim;
		std::wstring costumeRunAnim;
        std::wstring costumeDieAnim;

        // 보유 손 애니메이션 이름
        std::wstring costumeHandAnim;

        // 캐릭터의 크기?
        Vector2 costumeSize;
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
        void ChangeCostume(ePlayerCostume costume);
        ePlayerCostume GetCurrentCostume() { return mCurrentCostume; }

        class PlayerHand* GetHand() { return mLeftHand; }

    private:
        GameObject* mCenterObj;
        class PlayerHand* mLeftHand;
        ePlayerState mState;
        ePlayerCostume mCurrentCostume;
        std::vector<Costume> mCostume;
        class Animator* mAnimator;
        class RigidBody* mRigidBody;
        class PlayerScript* mPlayerScript;

        void GetCurrentCostumeString(std::string& stringToGet) const;
		void GetCostumeString(UINT costumeNumber, std::string& stringToGet) const;
		void GetCostumeString(UINT costumeNumber, std::wstring& stringToGet) const;
        void FlipBasedOnMousePos();

        void Idle();
        void Run();
        void Jump();
        void Die();

        void CreateAnimation();
		void CreateAdventurerAnimation(std::shared_ptr<hj::graphics::Material> material, std::shared_ptr<hj::graphics::Texture> texture, const Vector2& atlasTexSize);
		void CreateAliceAnimation(std::shared_ptr<hj::graphics::Material> material, std::shared_ptr<hj::graphics::Texture> texture, const Vector2& atlasTexSize);
		void CreateArmoredWarriorAnimation(std::shared_ptr<hj::graphics::Material> material, std::shared_ptr<hj::graphics::Texture> texture, const Vector2& atlasTexSize);
		void CreateCriminalSilhouetteAnimation(std::shared_ptr<hj::graphics::Material> material, std::shared_ptr<hj::graphics::Texture> texture, const Vector2& atlasTexSize);
		void CreateDevilSwordsmanAnimation(std::shared_ptr<hj::graphics::Material> material, std::shared_ptr<hj::graphics::Texture> texture, const Vector2& atlasTexSize);
		void CreateFatsoAnimation(std::shared_ptr<hj::graphics::Material> material, std::shared_ptr<hj::graphics::Texture> texture, const Vector2& atlasTexSize);
		void CreateHumanLasleyAnimation(std::shared_ptr<hj::graphics::Material> material, std::shared_ptr<hj::graphics::Texture> texture, const Vector2& atlasTexSize);
		void CreateIkinaBearAnimation(std::shared_ptr<hj::graphics::Material> material, std::shared_ptr<hj::graphics::Texture> texture, const Vector2& atlasTexSize);
		void CreateKingOfPickaxesAnimation(std::shared_ptr<hj::graphics::Material> material, std::shared_ptr<hj::graphics::Texture> texture, const Vector2& atlasTexSize);
		void CreateLotusAnimation(std::shared_ptr<hj::graphics::Material> material, std::shared_ptr<hj::graphics::Texture> texture, const Vector2& atlasTexSize);
		void CreateMasterChefAnimation(std::shared_ptr<hj::graphics::Material> material, std::shared_ptr<hj::graphics::Texture> texture, const Vector2& atlasTexSize);
		void CreateRiderHAnimation(std::shared_ptr<hj::graphics::Material> material, std::shared_ptr<hj::graphics::Texture> texture, const Vector2& atlasTexSize);
		void CreateSunsetGunmanAnimation(std::shared_ptr<hj::graphics::Material> material, std::shared_ptr<hj::graphics::Texture> texture, const Vector2& atlasTexSize);
    };
}
