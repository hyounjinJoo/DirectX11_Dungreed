#include "hjPlayer.h"
#include "hjPlayerHand.h"
#include "hjSpriteRenderer.h"
#include "hjResources.h"
#include "hjRigidBody.h"
#include "hjPlayerScript.h"
#include "hjInput.h"
#include "hjCollider2D.h"
#include "hjObject.h"
#include "hjArmRotatorScript.h"

namespace hj
{
	Player::Player()
		: GameObject()
		, mCenterObj(nullptr)
		, mLeftHand(nullptr)
		, mState(ePlayerState::End)
		, mCurrentCostume(ePlayerCostume::Adventurer)
		, mbIsFlip(false)
		, mSecondStepDustCreatedIndex(4)
	{
		SetName(WIDE("플레이어"));

		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = MTRL_FIND("MTRL_Char_Adventurer");
		std::shared_ptr<Mesh> mesh = MESH_FIND("Mesh_Rect");
		sr->SetMaterial(material);
		sr->SetMesh(mesh);

		mAnimator = AddComponent<Animator>();
		if (material && mAnimator)
		{
			std::shared_ptr<Texture> texture = material->GetTexture(eTextureSlot::T0);
			if (texture)
			{
				mCostume.reserve(static_cast<size_t>(ePlayerCostume::End));
				CreateAnimation();
			}
		}

		mRigidBody = AddComponent<RigidBody>();
		mPlayerScript = AddComponent<PlayerScript>();
		AddComponent<Collider2D>();

		mCenterObj = object::Instantiate<GameObject>(eLayerType::PlayerHas, Vector3(30.f, 0.f, 0.f), Vector3(0.f, 0.f, 0.f), Vector3(1.f, 1.f, 1.f));
		mCenterObj->SetName(WIDE("플레이어 핸드 소켓"));
		mCenterObj->GetTransform()->SetParent(GetTransform());
		//mCenterObj->AddComponent<Collider2D>();
		ArmRotatorScript* armScript = mCenterObj->AddComponent<ArmRotatorScript>();
		armScript->SetBody(this);

		mLeftHand = object::Instantiate<PlayerHand>(eLayerType::PlayerHas, Vector3(GetScaleX() * 0.5f, 0.f, 1.f));
		mLeftHand->GetTransform()->SetParent(mCenterObj->GetTransform());
		mLeftHand->GetTransform()->SetInheritParentScale(false);

		armScript->SetTarget(mLeftHand);
		armScript->SetUsingMouseRotation(true);

		for (int iter = 0; iter < 2; ++iter)
		{
			FxPlayerDust* dust = object::Instantiate<FxPlayerDust>(eLayerType::PlayerHas, Vector3(0.f, 0.f, -0.1f));
			dust->SetOwner(this);
			mPlayerDusts.push_back(dust);
		}

		mPlayerJump = object::Instantiate<FxPlayerJump>(eLayerType::PlayerHas, Vector3(0.f, 0.f, -0.1f));
		mPlayerJump->SetOwner(this);
	}

	Player::~Player()
	{
		{
			auto iter = mCostume.begin();
			auto iterEnd = mCostume.end();

			for (; iter != iterEnd; ++iter)
			{
				if (*iter)
				{
					delete *iter;
					*iter = nullptr;
				}
			}

			mCostume.clear();
		}
	}

	void Player::Initialize()
	{
		GameObject::Initialize();
	}

	void Player::Update()
	{
		GameObject::Update();

	}

	void Player::FixedUpdate()
	{
		GameObject::FixedUpdate();

		FlipBasedOnMousePos();
		//Vector3 centerPos = mCenterObj->GetWorldPosition();
		//Vector3 centerRot = mCenterObj->GetWorldRotation();

		//Vector3 handPos = mLeftHand->GetWorldPosition();
		//Vector3 handRot = mLeftHand->GetWorldRotation();

		//Vector3 weaponPos = mLeftHand->GetWeaponTR()->GetWorldPosition();
		//Vector3 weaponRot = mLeftHand->GetWeaponTR()->GetWorldRotation();

		//Vector3 playerPos = GetWorldPosition();
		//Vector3 handLocalPos = mLeftHand->GetPosition();
		//DEBUG_PRINT("Player Pos : % 3.2f % 3.2f % 3.2f", playerPos.x, playerPos.y, playerPos.z);
		//DEBUG_PRINT("Center Pos : %3.2f %3.2f %3.2f  Hand Pos : %3.2f %3.2f %3.2f  Weapon Pos : %3.2f %3.2f %3.2f", centerPos.x, centerPos.y, centerPos.z  ,handPos.x, handPos.y, handPos.z, weaponPos.x, weaponPos.y, weaponPos.z);
		//DEBUG_PRINT("Center Rot : %3.2f %3.2f %3.2f  Hand Rot : %3.2f %3.2f %3.2f  Weapon Rot : %3.2f %3.2f %3.2f", RadianToDegree(centerRot.x), RadianToDegree(centerRot.y), RadianToDegree(centerRot.z), RadianToDegree(handRot.x), RadianToDegree(handRot.y), RadianToDegree(handRot.z), RadianToDegree(weaponRot.x), RadianToDegree(weaponRot.y), RadianToDegree(weaponRot.z));

		//DEBUG_PRINT("Hand local : %3.2f %3.2f %3.2f", handLocalPos.x, handLocalPos.y, handLocalPos.z);
	}

	void Player::Render()
	{
		GameObject::Render();
	}

	void Player::GetCurrentCostumeString(std::string& stringToGet) const
	{
		GetCostumeString(static_cast<UINT>(mCurrentCostume), stringToGet);
	}

	void Player::GetCurrentCostumeString(std::wstring& stringToGet) const
	{
		GetCostumeString(static_cast<UINT>(mCurrentCostume), stringToGet);
	}

	void Player::GetCostumeString(UINT costumeNumber, std::string& stringToGet)
	{
		switch ((ePlayerCostume)costumeNumber)
		{
		case ePlayerCostume::Adventurer:
			stringToGet = ENUM_TO_STRING(ePlayerCostume::Adventurer);
			break;
		case ePlayerCostume::Alice:
			stringToGet = ENUM_TO_STRING(ePlayerCostume::Alice);
			break;
		case ePlayerCostume::ArmoredWarrior:
			stringToGet = ENUM_TO_STRING(ePlayerCostume::ArmoredWarrior);
			break;
		case ePlayerCostume::CriminalSilhouette:
			stringToGet = ENUM_TO_STRING(ePlayerCostume::CriminalSilhouette);
			break;
		case ePlayerCostume::DevilSwordsman:
			stringToGet = ENUM_TO_STRING(ePlayerCostume::DevilSwordsman);
			break;
		case ePlayerCostume::Fatso:
			stringToGet = ENUM_TO_STRING(ePlayerCostume::Fatso);
			break;
		case ePlayerCostume::HumanLasley:
			stringToGet = ENUM_TO_STRING(ePlayerCostume::HumanLasley);
			break;
		case ePlayerCostume::IkinaBear:
			stringToGet = ENUM_TO_STRING(ePlayerCostume::IkinaBear);
			break;
		case ePlayerCostume::KingOfPickaxes:
			stringToGet = ENUM_TO_STRING(ePlayerCostume::KingOfPickaxes);
			break;
		case ePlayerCostume::Lotus:
			stringToGet = ENUM_TO_STRING(ePlayerCostume::Lotus);
			break;
		case ePlayerCostume::MasterChef:
			stringToGet = ENUM_TO_STRING(ePlayerCostume::MasterChef);
			break;
		case ePlayerCostume::RiderH:
			stringToGet = ENUM_TO_STRING(ePlayerCostume::RiderH);
			break;
		case ePlayerCostume::SunsetGunman:
			stringToGet = ENUM_TO_STRING(ePlayerCostume::SunsetGunman);
			break;
		case ePlayerCostume::End:
		default:
			break;
		}
	}

	void Player::GetCostumeString(UINT costumeNumber, std::wstring& wstringToGet)
	{
		std::string stringToGet;
		GetCostumeString(costumeNumber, stringToGet);

		wstringToGet.assign(stringToGet.begin(), stringToGet.end());
	}

	void Player::FlipBasedOnMousePos()
	{
		float posX = mPlayerScript->GetOwnerScreenPos().x;
		float mouseScreenPosX = Input::GetMousePosition().x;

		if (posX < mouseScreenPosX)
		{
			SetRotationY(0.f); 
			mbIsFlip = false;
		}
		else
		{
			SetRotationY(PI);
			mbIsFlip = true;
		}
	}

	void Player::ChangeState(ePlayerState state)
	{
		if(mState == ePlayerState::Run)
			CreateDustIfNeed();

		if (mState == state)
			return;

		if (mState == ePlayerState::Die)
			return;

		mState = state;

		switch (state)
		{
		case ePlayerState::Idle:
			Idle();
			break;
		case ePlayerState::Jump:
			Jump();
			break;
		case ePlayerState::Run:
			Run();
			break;
		case ePlayerState::Die:
			Die();
			break;
		case ePlayerState::End:
		default:
			break;
		}
	}

	void Player::ChangeCostume(ePlayerCostume costume)
	{
		if (mCurrentCostume == costume)
			return;

		mCurrentCostume = costume;
		SetScaleXY(mCostume[static_cast<UINT>(mCurrentCostume)]->costumeSize);
		Idle();

		ChangeDustSpecificAttribute();
	}

	void Player::ChangeDustSpecificAttribute()
	{
		Animation* runAnim = mAnimator->FindAnimation(mCostume[static_cast<UINT>(mCurrentCostume)]->costumeRunAnim);
		if (!runAnim)
			return;

		int runSheetSize = runAnim->GetSheetSize();
		float runDuration = runAnim->GetDuration();
		if (runDuration == 0.f)
			return;

		int frameCountInStep = static_cast<int>(std::floorf(static_cast<float>(runSheetSize) * 0.5f));
		mSecondStepDustCreatedIndex = frameCountInStep;
		float newDustPlayTime = frameCountInStep * runDuration;

		for (int iter = 0; iter < 2; ++iter)
		{
			mPlayerDusts[iter]->ChangeAnimationDuration(newDustPlayTime);
		}
	}

	void Player::Idle()
	{
		mAnimator->Play(mCostume[static_cast<UINT>(mCurrentCostume)]->costumeIdleAnim);
	}

	void Player::Run()
	{
		mAnimator->Play(mCostume[static_cast<UINT>(mCurrentCostume)]->costumeRunAnim);
	}

	void Player::Jump()
	{
		mAnimator->Play(mCostume[static_cast<UINT>(mCurrentCostume)]->costumeJumpAnim);
	}

	void Player::Die()
	{
		mAnimator->Play(mCostume[static_cast<UINT>(mCurrentCostume)]->costumeDieAnim);
	}

	void Player::CreateDustIfNeed()
	{
#define LEFT_STEP_FRAME_INDEX 0

		Animation* currentAnimation = mAnimator->GetCurrentAnimation();
		if (nullptr == currentAnimation)
			return;

		int frameIndex = currentAnimation->GetCurrentFrameNumber();
		if (frameIndex == LEFT_STEP_FRAME_INDEX)
		{
			mPlayerDusts[0]->ActivateEffect();
		}
		if (frameIndex == mSecondStepDustCreatedIndex)
		{
			mPlayerDusts[1]->ActivateEffect();
		}
	}

	void Player::JumpEffectActive(JumpEffect jumpEffect)
	{
		if (!mPlayerJump)
			return;

		mPlayerJump->ActivateEffect(jumpEffect);
	}

#pragma region Create Animation
	void Player::CreateAnimation()
	{
		SpriteRenderer* sr = GetComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = sr->GetMaterial();
		std::shared_ptr<Texture> texture = material->GetTexture(eTextureSlot::T0);
		Vector2 atlasTexSize = texture->GetTexSize();

		CreateAdventurerAnimation(material, texture, atlasTexSize);
		CreateAliceAnimation(material, texture, atlasTexSize);
		CreateArmoredWarriorAnimation(material, texture, atlasTexSize);
		CreateCriminalSilhouetteAnimation(material, texture, atlasTexSize);
		CreateDevilSwordsmanAnimation(material, texture, atlasTexSize);
		CreateFatsoAnimation(material, texture, atlasTexSize);
		CreateHumanLasleyAnimation(material, texture, atlasTexSize);
		CreateIkinaBearAnimation(material, texture, atlasTexSize);
		CreateKingOfPickaxesAnimation(material, texture, atlasTexSize);
		CreateLotusAnimation(material, texture, atlasTexSize);
		CreateMasterChefAnimation(material, texture, atlasTexSize);
		CreateRiderHAnimation(material, texture, atlasTexSize);
		CreateSunsetGunmanAnimation(material, texture, atlasTexSize);

		GetTransform()->SetScaleXY(mCostume[static_cast<UINT>(mCurrentCostume)]->costumeSize);
		mAnimator->Play(mCostume[static_cast<UINT>(mCurrentCostume)]->costumeIdleAnim);
	}

	void Player::CreateAdventurerAnimation(std::shared_ptr<Material> material, std::shared_ptr<Texture> texture, const Vector2& atlasTexSize)
	{
		float duration = 0.f;
		duration = 1.f / 10.f;
		CREATE_ANIM(animAdventurerIdle, frame, atlasTexSize, duration);

		FRAME_ADD_OFFSETX(frame, 160.f, 0.f, 65.f, 95.f, 0.538462f, animAdventurerIdle);
		FRAME_ADD_OFFSETX(frame, 225.f, 0.f, 65.f, 100.f, 0.538462f, animAdventurerIdle);
		FRAME_ADD_OFFSETX(frame, 290.f, 0.f, 70.f, 100.f, 0.5f, animAdventurerIdle);
		FRAME_ADD_OFFSETX(frame, 360.f, 0.f, 75.f, 95.f, 0.466667f, animAdventurerIdle);
		FRAME_ADD_OFFSETX(frame, 435.f, 0.f, 70.f, 95.f, 0.5f, animAdventurerIdle);

		CREATE_SHEET(animAdventurerDie);
		FRAME_ADD_OFFSETX(frame, 65.f, 0.f, 95.f, 60.f, 0.473684f, animAdventurerDie);

		CREATE_SHEET(animAdventurerJump);
		FRAME_ADD_OFFSETX(frame, 505.f, 0.f, 65.f, 95.f, 0.538462f, animAdventurerJump);

		CREATE_SHEET(animAdventurerRun);
		frame.duration = 1.f / 16.f;
		FRAME_ADD_OFFSETX(frame, 570.f, 0.f, 85.f, 95.f, 0.470588f, animAdventurerRun);
		FRAME_ADD_OFFSETX(frame, 655.f, 0.f, 70.f, 100.f, 0.5f, animAdventurerRun);
		FRAME_ADD_OFFSETX(frame, 725.f, 0.f, 70.f, 100.f, 0.428571f, animAdventurerRun);
		FRAME_ADD_OFFSETX(frame, 795.f, 0.f, 75.f, 95.f, 0.4f, animAdventurerRun);
		FRAME_ADD_OFFSETX(frame, 870.f, 0.f, 75.f, 95.f, 0.4f, animAdventurerRun);
		FRAME_ADD_OFFSETX(frame, 945.f, 0.f, 65.f, 100.f, 0.461538f, animAdventurerRun);
		FRAME_ADD_OFFSETX(frame, 0.f, 105.f, 70.f, 100.f, 0.428571f, animAdventurerRun);
		FRAME_ADD_OFFSETX(frame, 70.f, 105.f, 80.f, 95.f, 0.4375f, animAdventurerRun);

		AUTO_OFFSET_CALC_Y(animAdventurerIdle);
		AUTO_OFFSET_CALC_Y(animAdventurerDie);
		AUTO_OFFSET_CALC_Y(animAdventurerJump);
		AUTO_OFFSET_CALC_Y(animAdventurerRun);

		std::wstring costumeString;
		UINT costumeNumber = static_cast<UINT>(ePlayerCostume::Adventurer);
		GetCostumeString(costumeNumber, costumeString);

		Costume* adventurerCostume = new Costume();

		std::wstring idleAnimWstr = adventurerCostume->costumeIdleAnim = costumeString + WIDE("_Idle");
		std::wstring dieAnimWstr = adventurerCostume->costumeDieAnim = costumeString + WIDE("_Die");
		std::wstring jumpAnimWstr = adventurerCostume->costumeJumpAnim = costumeString + WIDE("_Jump");
		std::wstring runAnimWstr = adventurerCostume->costumeRunAnim = costumeString + WIDE("_Run");

		adventurerCostume->costumeSize = canvasSize;

		mAnimator->Create(idleAnimWstr, texture, animAdventurerIdle, canvasSize, false);
		mAnimator->Create(dieAnimWstr, texture, animAdventurerDie, canvasSize, false);
		mAnimator->Create(jumpAnimWstr, texture, animAdventurerJump, canvasSize, false);
		mAnimator->Create(runAnimWstr, texture, animAdventurerRun, canvasSize, false);

		mCostume.push_back(adventurerCostume);
	}

	void Player::CreateAliceAnimation(std::shared_ptr<Material> material, std::shared_ptr<Texture> texture, const Vector2& atlasTexSize)
	{
		float duration = 0.f;
		duration = 1.f / 10.f;
		CREATE_ANIM(animAliceIdle, frame, atlasTexSize, duration);
		FRAME_ADD_OFFSETX(frame, 245.f, 105.f, 65.f, 95.f, 0.5f, animAliceIdle);
		FRAME_ADD_OFFSETX(frame, 310.f, 105.f, 65.f, 95.f, 0.5f, animAliceIdle);
		FRAME_ADD_OFFSETX(frame, 375.f, 105.f, 65.f, 95.f, 0.5f, animAliceIdle);
		FRAME_ADD_OFFSETX(frame, 440.f, 105.f, 65.f, 90.f, 0.5f, animAliceIdle);
		FRAME_ADD_OFFSETX(frame, 505.f, 105.f, 65.f, 90.f, 0.5f, animAliceIdle);

		CREATE_SHEET(animAliceDie);
		FRAME_ADD_OFFSETX(frame, 150, 105.f, 95.f, 50.f, 0.473684f, animAliceDie);

		CREATE_SHEET(animAliceJump);
		FRAME_ADD_OFFSETX(frame, 570.f, 105.f, 65.f, 95.f, 0.461538f, animAliceJump);

		CREATE_SHEET(animAliceRun);
		frame.duration = 1.f / 12.f;
		FRAME_ADD_OFFSETX(frame, 635.f, 105.f, 75.f, 85.f, 0.4f, animAliceRun);
		FRAME_ADD_OFFSETX(frame, 710.f, 105.f, 75.f, 90.f, 0.4f, animAliceRun);
		FRAME_ADD_OFFSETX(frame, 785.f, 105.f, 75.f, 90.f, 0.4f, animAliceRun);
		FRAME_ADD_OFFSETX(frame, 860.f, 105.f, 75.f, 85.f, 0.4f, animAliceRun);
		FRAME_ADD_OFFSETX(frame, 935.f, 105.f, 75.f, 90.f, 0.4f, animAliceRun);
		FRAME_ADD_OFFSETX(frame, 0.f,   205.f, 75.f, 90.f, 0.4f, animAliceRun);

		AUTO_OFFSET_CALC_Y(animAliceIdle);
		AUTO_OFFSET_CALC_Y(animAliceDie);
		AUTO_OFFSET_CALC_Y(animAliceJump);
		AUTO_OFFSET_CALC_Y(animAliceRun);

		std::wstring costumeString;
		UINT costumeNumber = static_cast<UINT>(ePlayerCostume::Alice);
		GetCostumeString(costumeNumber, costumeString);

		Costume* aliceCostume = new Costume();

		std::wstring idleAnimWstr = aliceCostume->costumeIdleAnim = costumeString + WIDE("_Idle");
		std::wstring dieAnimWstr = aliceCostume->costumeDieAnim = costumeString + WIDE("_Die");
		std::wstring jumpAnimWstr = aliceCostume->costumeJumpAnim = costumeString + WIDE("_Jump");
		std::wstring runAnimWstr = aliceCostume->costumeRunAnim = costumeString + WIDE("_Run");

		aliceCostume->costumeSize = canvasSize;

		mAnimator->Create(idleAnimWstr, texture, animAliceIdle, canvasSize, false);
		mAnimator->Create(dieAnimWstr, texture, animAliceDie, canvasSize, false);
		mAnimator->Create(jumpAnimWstr, texture, animAliceJump, canvasSize, false);
		mAnimator->Create(runAnimWstr, texture, animAliceRun, canvasSize, false);

		mCostume.push_back(aliceCostume);
	}

	void Player::CreateArmoredWarriorAnimation(std::shared_ptr<Material> material, std::shared_ptr<Texture> texture, const Vector2& atlasTexSize)
	{
		float duration = 0.f;
		duration = 1.f / 10.f;
		CREATE_ANIM(animArmoredWarriorIdle, frame, atlasTexSize, duration);
		FRAME_ADD_OFFSETX(frame, 535.f, 205.f, 65.f, 100.f,  0.461538f,animArmoredWarriorIdle);
		FRAME_ADD_OFFSETX(frame, 600.f, 205.f, 65.f, 100.f, 0.461538f,animArmoredWarriorIdle);
		FRAME_ADD_OFFSETX(frame, 665.f, 205.f, 65.f, 100.f, 0.461538f,animArmoredWarriorIdle);
		FRAME_ADD_OFFSETX(frame, 730.f, 205.f, 65.f, 95.f, 0.461538f,animArmoredWarriorIdle);
		FRAME_ADD_OFFSETX(frame, 795.f, 205.f, 65.f, 95.f, 0.461538f,animArmoredWarriorIdle);

		CREATE_SHEET(animArmoredWarriorDie);
		FRAME_ADD_OFFSETX(frame, 435.f, 205.f, 100.f, 55.f, 0.5f,animArmoredWarriorDie);

		CREATE_SHEET(animArmoredWarriorJump);
		FRAME_ADD_OFFSETX(frame, 860.f, 205.f, 65.f, 100.f, 0.538462f,animArmoredWarriorJump);

		CREATE_SHEET(animArmoredWarriorRun);
		frame.duration = 1.f / 16.f;
		FRAME_ADD_OFFSETX(frame, 925.f, 205.f, 70.f, 95.f, 0.571429f, animArmoredWarriorRun);
		FRAME_ADD_OFFSETX(frame, 0.f, 310.f, 65.f, 100.f, 0.538462f, animArmoredWarriorRun);
		FRAME_ADD_OFFSETX(frame, 65.f, 310.f, 60.f, 100.f, 0.5f, animArmoredWarriorRun);
		FRAME_ADD_OFFSETX(frame, 125.f, 310.f, 60.f, 95.f, 0.5f, animArmoredWarriorRun);
		FRAME_ADD_OFFSETX(frame, 185.f, 310.f, 60.f, 95.f, 0.5f, animArmoredWarriorRun);
		FRAME_ADD_OFFSETX(frame, 245.f, 310.f, 60.f, 100.f, 0.5f, animArmoredWarriorRun);
		FRAME_ADD_OFFSETX(frame, 305.f, 310.f, 65.f, 100.f, 0.538462f, animArmoredWarriorRun);
		FRAME_ADD_OFFSETX(frame, 370.f, 310.f, 65.f, 95.f, 0.538462f,animArmoredWarriorRun);

		AUTO_OFFSET_CALC_Y(animArmoredWarriorIdle);
		AUTO_OFFSET_CALC_Y(animArmoredWarriorDie);
		AUTO_OFFSET_CALC_Y(animArmoredWarriorJump);
		AUTO_OFFSET_CALC_Y(animArmoredWarriorRun);

		std::wstring costumeString;
		UINT costumeNumber = static_cast<UINT>(ePlayerCostume::ArmoredWarrior);
		GetCostumeString(costumeNumber, costumeString);

		Costume* armoredWarriorCostume = new Costume();

		std::wstring idleAnimWstr = armoredWarriorCostume->costumeIdleAnim = costumeString + WIDE("_Idle");
		std::wstring dieAnimWstr = armoredWarriorCostume->costumeDieAnim = costumeString + WIDE("_Die");
		std::wstring jumpAnimWstr = armoredWarriorCostume->costumeJumpAnim = costumeString + WIDE("_Jump");
		std::wstring runAnimWstr = armoredWarriorCostume->costumeRunAnim = costumeString + WIDE("_Run");

		armoredWarriorCostume->costumeSize = canvasSize;

		mAnimator->Create(idleAnimWstr, texture, animArmoredWarriorIdle, canvasSize, false);
		mAnimator->Create(dieAnimWstr, texture, animArmoredWarriorDie, canvasSize, false);
		mAnimator->Create(jumpAnimWstr, texture, animArmoredWarriorJump, canvasSize, false);
		mAnimator->Create(runAnimWstr, texture, animArmoredWarriorRun, canvasSize, false);

		mCostume.push_back(armoredWarriorCostume);
	}

	void Player::CreateCriminalSilhouetteAnimation(std::shared_ptr<Material> material, std::shared_ptr<Texture> texture, const Vector2& atlasTexSize)
	{
		float duration = 0.f;
		duration = 1.f / 10.f;
		CREATE_ANIM(animCriminalSilhouetteIdle, frame, atlasTexSize, duration);
		FRAME_ADD_OFFSETX(frame, 530.f, 310.f, 55.f, 95.f, 0.454545f, animCriminalSilhouetteIdle);
		FRAME_ADD_OFFSETX(frame, 585.f, 310.f, 55.f, 95.f, 0.454545f, animCriminalSilhouetteIdle);
		FRAME_ADD_OFFSETX(frame, 640.f, 310.f, 55.f, 95.f, 0.454545f, animCriminalSilhouetteIdle);
		FRAME_ADD_OFFSETX(frame, 695.f, 310.f, 55.f, 90.f, 0.454545f, animCriminalSilhouetteIdle);
		FRAME_ADD_OFFSETX(frame, 750.f, 310.f, 55.f, 90.f, 0.454545f,animCriminalSilhouetteIdle);

		CREATE_SHEET(animCriminalSilhouetteDie);
		FRAME_ADD_OFFSETX(frame, 435.f, 310.f, 95.f, 50.f, 0.473684f, animCriminalSilhouetteDie);

		CREATE_SHEET(animCriminalSilhouetteJump);
		FRAME_ADD_OFFSETX(frame, 805.f, 310.f, 60.f, 95.f, 0.416667f,animCriminalSilhouetteJump);

		CREATE_SHEET(animCriminalSilhouetteRun);
		frame.duration = 1.f / 12.f;
		FRAME_ADD_OFFSETX(frame, 865.f, 310.f, 65.f, 90.f, 0.384615f, animCriminalSilhouetteRun);
		FRAME_ADD_OFFSETX(frame, 930.f, 310.f, 60.f, 95.f, 0.333333f, animCriminalSilhouetteRun);
		FRAME_ADD_OFFSETX(frame, 0.f,   410.f, 60.f, 95.f, 0.333333f, animCriminalSilhouetteRun);
		FRAME_ADD_OFFSETX(frame, 60.f,  410.f, 60.f, 90.f, 0.333333f, animCriminalSilhouetteRun);
		FRAME_ADD_OFFSETX(frame, 120.f, 410.f, 60.f, 95.f, 0.333333f, animCriminalSilhouetteRun);
		FRAME_ADD_OFFSETX(frame, 180.f, 410.f, 60.f, 95.f, 0.333333f,animCriminalSilhouetteRun);

		AUTO_OFFSET_CALC_Y(animCriminalSilhouetteIdle);
		AUTO_OFFSET_CALC_Y(animCriminalSilhouetteDie);
		AUTO_OFFSET_CALC_Y(animCriminalSilhouetteJump);
		AUTO_OFFSET_CALC_Y(animCriminalSilhouetteRun);

		std::wstring costumeString;
		UINT costumeNumber = static_cast<UINT>(ePlayerCostume::CriminalSilhouette);
		GetCostumeString(costumeNumber, costumeString);

		Costume* criminalSilhouetteCostume = new Costume();

		std::wstring idleAnimWstr = criminalSilhouetteCostume->costumeIdleAnim = costumeString + WIDE("_Idle");
		std::wstring dieAnimWstr = criminalSilhouetteCostume->costumeDieAnim = costumeString + WIDE("_Die");
		std::wstring jumpAnimWstr = criminalSilhouetteCostume->costumeJumpAnim = costumeString + WIDE("_Jump");
		std::wstring runAnimWstr = criminalSilhouetteCostume->costumeRunAnim = costumeString + WIDE("_Run");

		criminalSilhouetteCostume->costumeSize = canvasSize;

		mAnimator->Create(idleAnimWstr, texture, animCriminalSilhouetteIdle, canvasSize, false);
		mAnimator->Create(dieAnimWstr, texture, animCriminalSilhouetteDie, canvasSize, false);
		mAnimator->Create(jumpAnimWstr, texture, animCriminalSilhouetteJump, canvasSize, false);
		mAnimator->Create(runAnimWstr, texture, animCriminalSilhouetteRun, canvasSize, false);

		mCostume.push_back(criminalSilhouetteCostume);
	}

	void Player::CreateDevilSwordsmanAnimation(std::shared_ptr<Material> material, std::shared_ptr<Texture> texture, const Vector2& atlasTexSize)
	{
		float duration = 0.f;
		duration = 1.f / 10.f;
		CREATE_ANIM(animDevilSwordsManIdle, frame, atlasTexSize, duration);
		FRAME_ADD_OFFSETX(frame, 345.f, 410.f, 70.f, 105.f, 0.5f, animDevilSwordsManIdle);
		FRAME_ADD_OFFSETX(frame, 415.f, 410.f, 70.f, 100.f, 0.5f, animDevilSwordsManIdle);
		FRAME_ADD_OFFSETX(frame, 485.f, 410.f, 70.f, 100.f, 0.5f, animDevilSwordsManIdle);
		FRAME_ADD_OFFSETX(frame, 555.f, 410.f, 70.f, 100.f, 0.5f, animDevilSwordsManIdle);
		FRAME_ADD_OFFSETX(frame, 625.f, 410.f, 70.f, 105.f, 0.5f,animDevilSwordsManIdle);

		CREATE_SHEET(animDevilSwordsManDie);
		FRAME_ADD_OFFSETX(frame, 240.f, 410.f, 105.f, 60.f, 0.47619f,animDevilSwordsManDie);

		CREATE_SHEET(animDevilSwordsManJump);
		FRAME_ADD_OFFSETX(frame, 695.f, 410.f, 70.f, 105.f, 0.5f,animDevilSwordsManJump);

		CREATE_SHEET(animDevilSwordsManRun);
		frame.duration = 1.f / 16.f;
		FRAME_ADD_OFFSETX(frame, 765.f, 410.f, 85.f, 95.f, 0.470588f, animDevilSwordsManRun);
		FRAME_ADD_OFFSETX(frame, 850.f, 410.f, 80.f, 95.f, 0.4375f, animDevilSwordsManRun);
		FRAME_ADD_OFFSETX(frame, 930.f, 410.f, 70.f, 100.f, 0.357143f, animDevilSwordsManRun);
		FRAME_ADD_OFFSETX(frame, 0.f,   515.f, 70.f, 95.f, 0.357143f, animDevilSwordsManRun);
		FRAME_ADD_OFFSETX(frame, 70.f,  515.f, 75.f, 95.f, 0.4f, animDevilSwordsManRun);
		FRAME_ADD_OFFSETX(frame, 145.f, 515.f, 75.f, 95.f, 0.4f, animDevilSwordsManRun);
		FRAME_ADD_OFFSETX(frame, 220.f, 515.f, 75.f, 100.f, 0.4f, animDevilSwordsManRun);
		FRAME_ADD_OFFSETX(frame, 295.f, 515.f, 80.f, 95.f, 0.4375f,animDevilSwordsManRun);

		AUTO_OFFSET_CALC_Y(animDevilSwordsManIdle);
		AUTO_OFFSET_CALC_Y(animDevilSwordsManDie);
		AUTO_OFFSET_CALC_Y(animDevilSwordsManJump);
		AUTO_OFFSET_CALC_Y(animDevilSwordsManRun);

		std::wstring costumeString;
		UINT costumeNumber = static_cast<UINT>(ePlayerCostume::DevilSwordsman);
		GetCostumeString(costumeNumber, costumeString);

		Costume* DevilSwordsManCostume = new Costume();

		std::wstring idleAnimWstr = DevilSwordsManCostume->costumeIdleAnim = costumeString + WIDE("_Idle");
		std::wstring dieAnimWstr = DevilSwordsManCostume->costumeDieAnim = costumeString + WIDE("_Die");
		std::wstring jumpAnimWstr = DevilSwordsManCostume->costumeJumpAnim = costumeString + WIDE("_Jump");
		std::wstring runAnimWstr = DevilSwordsManCostume->costumeRunAnim = costumeString + WIDE("_Run");

		DevilSwordsManCostume->costumeSize = canvasSize;

		mAnimator->Create(idleAnimWstr, texture, animDevilSwordsManIdle, canvasSize, false);
		mAnimator->Create(dieAnimWstr, texture, animDevilSwordsManDie, canvasSize, false);
		mAnimator->Create(jumpAnimWstr, texture, animDevilSwordsManJump, canvasSize, false);
		mAnimator->Create(runAnimWstr, texture, animDevilSwordsManRun, canvasSize, false);

		mCostume.push_back(DevilSwordsManCostume);
	}

	void Player::CreateFatsoAnimation(std::shared_ptr<Material> material, std::shared_ptr<Texture> texture, const Vector2& atlasTexSize)
	{
		float duration = 0.f;
		duration = 1.f / 10.f;
		CREATE_ANIM(animFatsoIdle, frame, atlasTexSize, duration);
		FRAME_ADD_OFFSETX(frame, 465.f, 515.f, 80.f, 90.f, 0.5f, animFatsoIdle);
		FRAME_ADD_OFFSETX(frame, 545.f, 515.f, 80.f, 90.f, 0.5f, animFatsoIdle);
		FRAME_ADD_OFFSETX(frame, 625.f, 515.f, 80.f, 90.f, 0.5f, animFatsoIdle);
		FRAME_ADD_OFFSETX(frame, 705.f, 515.f, 75.f, 85.f, 0.533333f, animFatsoIdle);
		FRAME_ADD_OFFSETX(frame, 780.f, 515.f, 75.f, 85.f, 0.533333f,animFatsoIdle);

		CREATE_SHEET(animFatsoDie);
		FRAME_ADD_OFFSETX(frame, 375.f, 515.f, 90.f, 75.f, 0.5f,animFatsoDie);

		CREATE_SHEET(animFatsoJump);
		FRAME_ADD_OFFSETX(frame, 855.f, 515.f, 75.f, 85.f, 0.5f,animFatsoJump);

		CREATE_SHEET(animFatsoRun);
		frame.duration = 1.f / 16.f;
		FRAME_ADD_OFFSETX(frame, 935.f, 515.f, 80.f, 90.f, 0.5f, animFatsoRun);
		FRAME_ADD_OFFSETX(frame, 0.f,   615.f, 80.f, 95.f, 0.5f, animFatsoRun);
		FRAME_ADD_OFFSETX(frame, 80.f,  615.f, 80.f, 90.f, 0.5f, animFatsoRun);
		FRAME_ADD_OFFSETX(frame, 160.f, 615.f, 80.f, 85.f, 0.5f, animFatsoRun);
		FRAME_ADD_OFFSETX(frame, 240.f, 615.f, 80.f, 90.f, 0.5f, animFatsoRun);
		FRAME_ADD_OFFSETX(frame, 320.f, 615.f, 80.f, 95.f, 0.5f, animFatsoRun);
		FRAME_ADD_OFFSETX(frame, 400.f, 615.f, 80.f, 90.f, 0.5f, animFatsoRun);
		FRAME_ADD_OFFSETX(frame, 480.f, 615.f, 80.f, 85.f, 0.5f,animFatsoRun);

		AUTO_OFFSET_CALC_Y(animFatsoIdle);
		AUTO_OFFSET_CALC_Y(animFatsoDie);
		AUTO_OFFSET_CALC_Y(animFatsoJump);
		AUTO_OFFSET_CALC_Y(animFatsoRun);

		std::wstring costumeString;
		UINT costumeNumber = static_cast<UINT>(ePlayerCostume::Fatso);
		GetCostumeString(costumeNumber, costumeString);

		Costume* fatsoCostume = new Costume();

		std::wstring idleAnimWstr = fatsoCostume->costumeIdleAnim = costumeString + WIDE("_Idle");
		std::wstring dieAnimWstr = fatsoCostume->costumeDieAnim = costumeString + WIDE("_Die");
		std::wstring jumpAnimWstr = fatsoCostume->costumeJumpAnim = costumeString + WIDE("_Jump");
		std::wstring runAnimWstr = fatsoCostume->costumeRunAnim = costumeString + WIDE("_Run");

		fatsoCostume->costumeSize = canvasSize;

		mAnimator->Create(idleAnimWstr, texture, animFatsoIdle, canvasSize, false);
		mAnimator->Create(dieAnimWstr, texture, animFatsoDie, canvasSize, false);
		mAnimator->Create(jumpAnimWstr, texture, animFatsoJump, canvasSize, false);
		mAnimator->Create(runAnimWstr, texture, animFatsoRun, canvasSize, false);

		mCostume.push_back(fatsoCostume);
	}

	void Player::CreateHumanLasleyAnimation(std::shared_ptr<Material> material, std::shared_ptr<Texture> texture, const Vector2& atlasTexSize)
	{
		float duration = 0.f;
		duration = 1.f / 10.f;
		CREATE_ANIM(animHumanLasleyIdle, frame, atlasTexSize, duration);
		FRAME_ADD_OFFSETX(frame, 865.f, 615.f, 70.f, 100.f, 0.5f, animHumanLasleyIdle);
		FRAME_ADD_OFFSETX(frame, 935.f, 615.f, 70.f, 100.f, 0.5f, animHumanLasleyIdle);
		FRAME_ADD_OFFSETX(frame, 0.f, 715.f, 70.f, 100.f, 0.5f, animHumanLasleyIdle);
		FRAME_ADD_OFFSETX(frame, 70.f, 715.f, 75.f, 95.f, 0.533333f, animHumanLasleyIdle);
		FRAME_ADD_OFFSETX(frame, 145.f, 715.f, 70.f, 95.f, 0.5f, animHumanLasleyIdle);

		CREATE_SHEET(animHumanLasleyDie);
		FRAME_ADD_OFFSETX(frame, 765.f, 615.f, 100.f, 55.f, 0.5f, animHumanLasleyDie);

		CREATE_SHEET(animHumanLasleyJump);
		FRAME_ADD_OFFSETX(frame, 215.f, 715.f, 70.f, 100.f, 0.5f, animHumanLasleyJump);

		CREATE_SHEET(animHumanLasleyRun);
		frame.duration = 1.f / 16.f;
		FRAME_ADD_OFFSETX(frame, 285.f, 715.f, 85.f, 95.f, 0.529412f, animHumanLasleyRun);
		FRAME_ADD_OFFSETX(frame, 370.f, 715.f, 80.f, 100.f, 0.5f, animHumanLasleyRun);
		FRAME_ADD_OFFSETX(frame, 450.f, 715.f, 75.f, 100.f, 0.466667f, animHumanLasleyRun);
		FRAME_ADD_OFFSETX(frame, 525.f, 715.f, 75.f, 95.f, 0.466667f, animHumanLasleyRun);
		FRAME_ADD_OFFSETX(frame, 600.f, 715.f, 80.f, 95.f, 0.5f, animHumanLasleyRun);
		FRAME_ADD_OFFSETX(frame, 680.f, 715.f, 80.f, 100.f, 0.5f, animHumanLasleyRun);
		FRAME_ADD_OFFSETX(frame, 760.f, 715.f, 75.f, 100.f, 0.466667f, animHumanLasleyRun);
		FRAME_ADD_OFFSETX(frame, 835.f, 715.f, 80.f, 95.f, 0.5f, animHumanLasleyRun);

		AUTO_OFFSET_CALC_Y(animHumanLasleyIdle);
		AUTO_OFFSET_CALC_Y(animHumanLasleyDie);
		AUTO_OFFSET_CALC_Y(animHumanLasleyJump);
		AUTO_OFFSET_CALC_Y(animHumanLasleyRun);

		std::wstring costumeString;
		UINT costumeNumber = static_cast<UINT>(ePlayerCostume::HumanLasley);
		GetCostumeString(costumeNumber, costumeString);

		Costume* humanLasleyCostume = new Costume();

		std::wstring idleAnimWstr = humanLasleyCostume->costumeIdleAnim = costumeString + WIDE("_Idle");
		std::wstring dieAnimWstr = humanLasleyCostume->costumeDieAnim = costumeString + WIDE("_Die");
		std::wstring jumpAnimWstr = humanLasleyCostume->costumeJumpAnim = costumeString + WIDE("_Jump");
		std::wstring runAnimWstr = humanLasleyCostume->costumeRunAnim = costumeString + WIDE("_Run");

		humanLasleyCostume->costumeSize = canvasSize;

		mAnimator->Create(idleAnimWstr, texture, animHumanLasleyIdle, canvasSize, false);
		mAnimator->Create(dieAnimWstr, texture, animHumanLasleyDie, canvasSize, false);
		mAnimator->Create(jumpAnimWstr, texture, animHumanLasleyJump, canvasSize, false);
		mAnimator->Create(runAnimWstr, texture, animHumanLasleyRun, canvasSize, false);

		mCostume.push_back(humanLasleyCostume);
	}

	void Player::CreateIkinaBearAnimation(std::shared_ptr<Material> material, std::shared_ptr<Texture> texture, const Vector2& atlasTexSize)
	{
		float duration = 0.f;
		duration = 1.f / 10.f;
		CREATE_ANIM(animIkinaBearIdle, frame, atlasTexSize, duration);
		FRAME_ADD_OFFSETX(frame, 0.f, 815.f, 75.f, 100.f, 0.533333f, animIkinaBearIdle);
		FRAME_ADD_OFFSETX(frame, 75.f, 815.f, 75.f, 105.f, 0.533333f, animIkinaBearIdle);
		FRAME_ADD_OFFSETX(frame, 150.f, 815.f, 75.f, 105.f, 0.533333f, animIkinaBearIdle);
		FRAME_ADD_OFFSETX(frame, 225.f, 815.f, 75.f, 100.f, 0.533333f, animIkinaBearIdle);
		FRAME_ADD_OFFSETX(frame, 300.f, 815.f, 75.f, 100.f, 0.533333f, animIkinaBearIdle);

		CREATE_SHEET(animIkinaBearDie);
		FRAME_ADD_OFFSETX(frame, 915.f, 715.f, 100.f, 60.f, 0.5f, animIkinaBearDie);

		CREATE_SHEET(animIkinaBearJump);
		FRAME_ADD_OFFSETX(frame, 375.f, 815.f, 75.f, 100.f, 0.533333f, animIkinaBearJump);

		CREATE_SHEET(animIkinaBearRun);
		frame.duration = 1.f / 12.f;
		FRAME_ADD_OFFSETX(frame, 450.f, 815.f, 75.f, 100.f, 0.466667f, animIkinaBearRun);
		FRAME_ADD_OFFSETX(frame, 525.f, 815.f, 75.f, 100.f, 0.466667f, animIkinaBearRun);
		FRAME_ADD_OFFSETX(frame, 600.f, 815.f, 75.f, 105.f, 0.466667f, animIkinaBearRun);
		FRAME_ADD_OFFSETX(frame, 675.f, 815.f, 75.f, 100.f, 0.466667f, animIkinaBearRun);
		FRAME_ADD_OFFSETX(frame, 750.f, 815.f, 75.f, 100.f, 0.466667f, animIkinaBearRun);
		FRAME_ADD_OFFSETX(frame, 825.f, 815.f, 75.f, 105.f, 0.466667f, animIkinaBearRun);

		AUTO_OFFSET_CALC_Y(animIkinaBearIdle);
		AUTO_OFFSET_CALC_Y(animIkinaBearDie);
		AUTO_OFFSET_CALC_Y(animIkinaBearJump);
		AUTO_OFFSET_CALC_Y(animIkinaBearRun);

		std::wstring costumeString;
		UINT costumeNumber = static_cast<UINT>(ePlayerCostume::IkinaBear);
		GetCostumeString(costumeNumber, costumeString);

		Costume* ikinaBearCostume = new Costume();

		std::wstring idleAnimWstr = ikinaBearCostume->costumeIdleAnim = costumeString + WIDE("_Idle");
		std::wstring dieAnimWstr = ikinaBearCostume->costumeDieAnim = costumeString + WIDE("_Die");
		std::wstring jumpAnimWstr = ikinaBearCostume->costumeJumpAnim = costumeString + WIDE("_Jump");
		std::wstring runAnimWstr = ikinaBearCostume->costumeRunAnim = costumeString + WIDE("_Run");

		ikinaBearCostume->costumeSize = canvasSize;

		mAnimator->Create(idleAnimWstr, texture, animIkinaBearIdle, canvasSize, false);
		mAnimator->Create(dieAnimWstr, texture, animIkinaBearDie, canvasSize, false);
		mAnimator->Create(jumpAnimWstr, texture, animIkinaBearJump, canvasSize, false);
		mAnimator->Create(runAnimWstr, texture, animIkinaBearRun, canvasSize, false);

		mCostume.push_back(ikinaBearCostume);
	}

	void Player::CreateKingOfPickaxesAnimation(std::shared_ptr<Material> material, std::shared_ptr<Texture> texture, const Vector2& atlasTexSize)
	{
		float duration = 0.f;
		duration = 1.f / 10.f;
		CREATE_ANIM(animKingOfPickaxesIdle, frame, atlasTexSize, duration);
		FRAME_ADD_OFFSETX(frame, 0.f, 920.f, 70.f, 100.f, 0.5f, animKingOfPickaxesIdle);
		FRAME_ADD_OFFSETX(frame, 70.f, 920.f, 70.f, 105.f, 0.5f, animKingOfPickaxesIdle);
		FRAME_ADD_OFFSETX(frame, 140.f, 920.f, 70.f, 105.f, 0.5f, animKingOfPickaxesIdle);
		FRAME_ADD_OFFSETX(frame, 210.f, 920.f, 70.f, 100.f, 0.5f, animKingOfPickaxesIdle);
		FRAME_ADD_OFFSETX(frame, 280.f, 920.f, 70.f, 95.f, 0.5f, animKingOfPickaxesIdle);

		CREATE_SHEET(animKingOfPickaxesDie);
		FRAME_ADD_OFFSETX(frame, 900.f, 815.f, 100.f, 60.f, 0.5f, animKingOfPickaxesDie);

		CREATE_SHEET(animKingOfPickaxesJump);
		FRAME_ADD_OFFSETX(frame, 350.f, 920.f, 70.f, 100.f, 0.5f, animKingOfPickaxesJump);

		CREATE_SHEET(animKingOfPickaxesRun);
		frame.duration = 1.f / 16.f;
		FRAME_ADD_OFFSETX(frame, 420.f, 920.f, 75.f, 100.f, 0.533333f, animKingOfPickaxesRun);
		FRAME_ADD_OFFSETX(frame, 495.f, 920.f, 70.f, 100.f, 0.5f, animKingOfPickaxesRun);
		FRAME_ADD_OFFSETX(frame, 565.f, 920.f, 70.f, 105.f, 0.5f, animKingOfPickaxesRun);
		FRAME_ADD_OFFSETX(frame, 635.f, 920.f, 70.f, 100.f, 0.5f, animKingOfPickaxesRun);
		FRAME_ADD_OFFSETX(frame, 705.f, 920.f, 70.f, 100.f, 0.5f, animKingOfPickaxesRun);
		FRAME_ADD_OFFSETX(frame, 775.f, 920.f, 70.f, 100.f, 0.5f, animKingOfPickaxesRun);
		FRAME_ADD_OFFSETX(frame, 845.f, 920.f, 70.f, 105.f, 0.5f, animKingOfPickaxesRun);
		FRAME_ADD_OFFSETX(frame, 915.f, 920.f, 70.f, 100.f, 0.5f, animKingOfPickaxesRun);

		AUTO_OFFSET_CALC_Y(animKingOfPickaxesIdle);
		AUTO_OFFSET_CALC_Y(animKingOfPickaxesDie);
		AUTO_OFFSET_CALC_Y(animKingOfPickaxesJump);
		AUTO_OFFSET_CALC_Y(animKingOfPickaxesRun);

		std::wstring costumeString;
		UINT costumeNumber = static_cast<UINT>(ePlayerCostume::KingOfPickaxes);
		GetCostumeString(costumeNumber, costumeString);

		Costume* kingOfPickaxesCostume = new Costume();

		std::wstring idleAnimWstr = kingOfPickaxesCostume->costumeIdleAnim = costumeString + WIDE("_Idle");
		std::wstring dieAnimWstr = kingOfPickaxesCostume->costumeDieAnim = costumeString + WIDE("_Die");
		std::wstring jumpAnimWstr = kingOfPickaxesCostume->costumeJumpAnim = costumeString + WIDE("_Jump");
		std::wstring runAnimWstr = kingOfPickaxesCostume->costumeRunAnim = costumeString + WIDE("_Run");

		kingOfPickaxesCostume->costumeSize = canvasSize;

		mAnimator->Create(idleAnimWstr, texture, animKingOfPickaxesIdle, canvasSize, false);
		mAnimator->Create(dieAnimWstr, texture, animKingOfPickaxesDie, canvasSize, false);
		mAnimator->Create(jumpAnimWstr, texture, animKingOfPickaxesJump, canvasSize, false);
		mAnimator->Create(runAnimWstr, texture, animKingOfPickaxesRun, canvasSize, false);

		mCostume.push_back(kingOfPickaxesCostume);
	}

	void Player::CreateLotusAnimation(std::shared_ptr<Material> material, std::shared_ptr<Texture> texture, const Vector2& atlasTexSize)
	{
		float duration = 0.f;
		duration = 1.f / 10.f;
		CREATE_ANIM(animLotusIdle, frame, atlasTexSize, duration);
		FRAME_ADD_OFFSETX(frame, 105.f, 1025.f, 75.f, 95.f, 0.533333f, animLotusIdle);
		FRAME_ADD_OFFSETX(frame, 180.f, 1025.f, 75.f, 100.f, 0.533333f, animLotusIdle);
		FRAME_ADD_OFFSETX(frame, 255.f, 1025.f, 75.f, 100.f, 0.533333f, animLotusIdle);
		FRAME_ADD_OFFSETX(frame, 330.f, 1025.f, 75.f, 95.f, 0.533333f, animLotusIdle);
		FRAME_ADD_OFFSETX(frame, 405.f, 1025.f, 75.f, 95.f, 0.533333f, animLotusIdle);

		CREATE_SHEET(animLotusDie);
		FRAME_ADD_OFFSETX(frame, 0.f, 1025.f, 105.f, 50.f, 0.428571f, animLotusDie);

		CREATE_SHEET(animLotusJump);
		FRAME_ADD_OFFSETX(frame, 480.f, 1025.f, 75.f, 100.f, 0.533333f, animLotusJump);

		CREATE_SHEET(animLotusRun);
		frame.duration = 1.f / 12.f;
		FRAME_ADD_OFFSETX(frame, 555.f, 1025.f, 75.f, 90.f, 0.4f, animLotusRun);
		FRAME_ADD_OFFSETX(frame, 630.f, 1025.f, 75.f, 90.f, 0.4f, animLotusRun);
		FRAME_ADD_OFFSETX(frame, 705.f, 1025.f, 75.f, 95.f, 0.4f, animLotusRun);
		FRAME_ADD_OFFSETX(frame, 780.f, 1025.f, 75.f, 90.f, 0.4f, animLotusRun);
		FRAME_ADD_OFFSETX(frame, 855.f, 1025.f, 75.f, 90.f, 0.4f, animLotusRun);
		FRAME_ADD_OFFSETX(frame, 930.f, 1025.f, 75.f, 95.f, 0.4f, animLotusRun);

		AUTO_OFFSET_CALC_Y(animLotusIdle);
		AUTO_OFFSET_CALC_Y(animLotusDie);
		AUTO_OFFSET_CALC_Y(animLotusJump);
		AUTO_OFFSET_CALC_Y(animLotusRun);

		std::wstring costumeString;
		UINT costumeNumber = static_cast<UINT>(ePlayerCostume::Lotus);
		GetCostumeString(costumeNumber, costumeString);

		Costume* lotusCostume = new Costume();

		std::wstring idleAnimWstr = lotusCostume->costumeIdleAnim = costumeString + WIDE("_Idle");
		std::wstring dieAnimWstr = lotusCostume->costumeDieAnim = costumeString + WIDE("_Die");
		std::wstring jumpAnimWstr = lotusCostume->costumeJumpAnim = costumeString + WIDE("_Jump");
		std::wstring runAnimWstr = lotusCostume->costumeRunAnim = costumeString + WIDE("_Run");

		lotusCostume->costumeSize = canvasSize;

		mAnimator->Create(idleAnimWstr, texture, animLotusIdle, canvasSize, false);
		mAnimator->Create(dieAnimWstr, texture, animLotusDie, canvasSize, false);
		mAnimator->Create(jumpAnimWstr, texture, animLotusJump, canvasSize, false);
		mAnimator->Create(runAnimWstr, texture, animLotusRun, canvasSize, false);

		mCostume.push_back(lotusCostume);
	}

	void Player::CreateMasterChefAnimation(std::shared_ptr<Material> material, std::shared_ptr<Texture> texture, const Vector2& atlasTexSize)
	{
		float duration = 0.f;
		duration = 1.f / 10.f;
		CREATE_ANIM(animMasterChefIdle, frame, atlasTexSize, duration);
		FRAME_ADD_OFFSETX(frame, 115.f, 1125.f, 75.f, 115.f, 0.6f, animMasterChefIdle);
		FRAME_ADD_OFFSETX(frame, 190.f, 1125.f, 75.f, 115.f, 0.6f, animMasterChefIdle);
		FRAME_ADD_OFFSETX(frame, 265.f, 1125.f, 75.f, 115.f, 0.6f, animMasterChefIdle);
		FRAME_ADD_OFFSETX(frame, 340.f, 1125.f, 75.f, 110.f, 0.6f, animMasterChefIdle);
		FRAME_ADD_OFFSETX(frame, 415.f, 1125.f, 75.f, 110.f, 0.6f, animMasterChefIdle);

		CREATE_SHEET(animMasterChefDie);
		FRAME_ADD_OFFSETX(frame, 0.f, 1125.f, 115.f, 60.f, 0.478261f, animMasterChefDie);

		CREATE_SHEET(animMasterChefJump);
		FRAME_ADD_OFFSETX(frame, 490.f, 1125.f, 75.f, 110.f, 0.6f, animMasterChefJump);

		CREATE_SHEET(animMasterChefRun);
		frame.duration = 1.f / 16.f;
		FRAME_ADD_OFFSETX(frame, 565.f, 1125.f, 75.f, 105.f, 0.533333f, animMasterChefRun);
		FRAME_ADD_OFFSETX(frame, 640.f, 1125.f, 75.f, 110.f, 0.533333f, animMasterChefRun);
		FRAME_ADD_OFFSETX(frame, 715.f, 1125.f, 75.f, 110.f, 0.533333f, animMasterChefRun);
		FRAME_ADD_OFFSETX(frame, 790.f, 1125.f, 75.f, 105.f, 0.533333f, animMasterChefRun);
		FRAME_ADD_OFFSETX(frame, 865.f, 1125.f, 75.f, 105.f, 0.533333f, animMasterChefRun);
		FRAME_ADD_OFFSETX(frame, 940.f, 1125.f, 75.f, 110.f, 0.533333f, animMasterChefRun);
		FRAME_ADD_OFFSETX(frame, 0.f, 1240.f, 75.f, 110.f, 0.533333f, animMasterChefRun);
		FRAME_ADD_OFFSETX(frame, 75.f, 1240.f, 75.f, 105.f, 0.533333f, animMasterChefRun);

		AUTO_OFFSET_CALC_Y(animMasterChefIdle);
		AUTO_OFFSET_CALC_Y(animMasterChefDie);
		AUTO_OFFSET_CALC_Y(animMasterChefJump);
		AUTO_OFFSET_CALC_Y(animMasterChefRun);

		std::wstring costumeString;
		UINT costumeNumber = static_cast<UINT>(ePlayerCostume::MasterChef);
		GetCostumeString(costumeNumber, costumeString);

		Costume* masterChefCostume = new Costume();

		std::wstring idleAnimWstr = masterChefCostume->costumeIdleAnim = costumeString + WIDE("_Idle");
		std::wstring dieAnimWstr = masterChefCostume->costumeDieAnim = costumeString + WIDE("_Die");
		std::wstring jumpAnimWstr = masterChefCostume->costumeJumpAnim = costumeString + WIDE("_Jump");
		std::wstring runAnimWstr = masterChefCostume->costumeRunAnim = costumeString + WIDE("_Run");

		masterChefCostume->costumeSize = canvasSize;

		mAnimator->Create(idleAnimWstr, texture, animMasterChefIdle, canvasSize, false);
		mAnimator->Create(dieAnimWstr, texture, animMasterChefDie, canvasSize, false);
		mAnimator->Create(jumpAnimWstr, texture, animMasterChefJump, canvasSize, false);
		mAnimator->Create(runAnimWstr, texture, animMasterChefRun, canvasSize, false);

		mCostume.push_back(masterChefCostume);
	}

	void Player::CreateRiderHAnimation(std::shared_ptr<hj::graphics::Material> material, std::shared_ptr<hj::graphics::Texture> texture, const Vector2& atlasTexSize)
	{
		float duration = 0.f;
		duration = 1.f / 10.f;
		CREATE_ANIM(animRiderHIdle, frame, atlasTexSize, duration);
		FRAME_ADD_OFFSETX(frame, 245.f, 1240.f, 120.f, 95.f,  0.666667f, animRiderHIdle);
		FRAME_ADD_OFFSETX(frame, 365.f, 1240.f, 120.f, 95.f,  0.666667f, animRiderHIdle);
		FRAME_ADD_OFFSETX(frame, 485.f, 1240.f, 120.f, 95.f,  0.666667f, animRiderHIdle);
		FRAME_ADD_OFFSETX(frame, 605.f, 1240.f, 120.f, 90.f,  0.666667f, animRiderHIdle);
		FRAME_ADD_OFFSETX(frame, 725.f, 1240.f, 120.f, 90.f,  0.666667f, animRiderHIdle);

		CREATE_SHEET(animRiderHDie);
		FRAME_ADD_OFFSETX(frame, 150.f, 1240.f, 95.f, 70.f, 0.526316f, animRiderHDie);

		CREATE_SHEET(animRiderHRun);
		frame.duration = 1.f / 12.f;
		FRAME_ADD_OFFSETX(frame, 845.f, 1240.f, 115.f, 90.f, 0.652174f, animRiderHRun);
		FRAME_ADD_OFFSETX(frame, 0.f, 1350.f, 110.f, 90.f, 0.636364f, animRiderHRun);
		FRAME_ADD_OFFSETX(frame, 110.f, 1350.f, 115.f, 90.f, 0.652174f, animRiderHRun);
		FRAME_ADD_OFFSETX(frame, 225.f, 1350.f, 110.f, 90.f, 0.636364f, animRiderHRun);
		FRAME_ADD_OFFSETX(frame, 335.f, 1350.f, 115.f, 90.f, 0.652174f, animRiderHRun);
		FRAME_ADD_OFFSETX(frame, 450.f, 1350.f, 110.f, 90.f, 0.636364f, animRiderHRun);

		AUTO_OFFSET_CALC_Y(animRiderHIdle);
		AUTO_OFFSET_CALC_Y(animRiderHDie);
		AUTO_OFFSET_CALC_Y(animRiderHRun);

		std::wstring costumeString;
		UINT costumeNumber = static_cast<UINT>(ePlayerCostume::RiderH);
		GetCostumeString(costumeNumber, costumeString);

		Costume* riderHCostume = new Costume();

		std::wstring idleAnimWstr = riderHCostume->costumeIdleAnim = costumeString + WIDE("_Idle");
		std::wstring dieAnimWstr = riderHCostume->costumeDieAnim = costumeString + WIDE("_Die");
		std::wstring jumpAnimWstr = riderHCostume->costumeJumpAnim = costumeString + WIDE("_Jump");
		std::wstring runAnimWstr = riderHCostume->costumeRunAnim = costumeString + WIDE("_Run");

		canvasSize = Vector2(128.f, canvasSize.y);
		riderHCostume->costumeSize = canvasSize;

		mAnimator->Create(idleAnimWstr, texture, animRiderHIdle, canvasSize, false);
		mAnimator->Create(dieAnimWstr, texture, animRiderHDie, canvasSize, false);
		// RiderH는 Jump 애니메이션과 Run 애니메이션이 동일.
		mAnimator->Create(jumpAnimWstr, texture, animRiderHRun, canvasSize, false);
		mAnimator->Create(runAnimWstr, texture, animRiderHRun, canvasSize, false);

		mCostume.push_back(riderHCostume);
	}

	void Player::CreateSunsetGunmanAnimation(std::shared_ptr<hj::graphics::Material> material, std::shared_ptr<hj::graphics::Texture> texture, const Vector2& atlasTexSize)
	{
		float duration = 0.f;
		duration = 1.f / 10.f;
		CREATE_ANIM(animSunsetGunmanIdle, frame, atlasTexSize, duration);
		FRAME_ADD_OFFSETX(frame, 665.f, 1350.f, 85.f, 105.f, 0.470588f, animSunsetGunmanIdle);
		FRAME_ADD_OFFSETX(frame, 750.f, 1350.f, 85.f, 105.f, 0.470588f, animSunsetGunmanIdle);
		FRAME_ADD_OFFSETX(frame, 835.f, 1350.f, 85.f, 105.f, 0.470588f, animSunsetGunmanIdle);
		FRAME_ADD_OFFSETX(frame, 920.f, 1350.f, 85.f, 100.f, 0.470588f, animSunsetGunmanIdle);
		FRAME_ADD_OFFSETX(frame, 0.f, 1455.f, 85.f, 100.f, 0.470588f, animSunsetGunmanIdle);

		CREATE_SHEET(animSunsetGunmanDie);
		FRAME_ADD_OFFSETX(frame, 60.f, 1350.f, 105.f, 65.f, 0.47619f, animSunsetGunmanDie);

		CREATE_SHEET(animSunsetGunmanJump);
		FRAME_ADD_OFFSETX(frame, 85.f, 1455.f, 85.f, 105.f, 0.470588f, animSunsetGunmanJump);

		CREATE_SHEET(animSunsetGunmanRun);
		frame.duration = 1.f / 16.f;
		FRAME_ADD_OFFSETX(frame, 170.f, 1455.f, 85.f, 100.f, 0.470588f, animSunsetGunmanRun);
		FRAME_ADD_OFFSETX(frame, 255.f, 1455.f, 85.f, 105.f, 0.470588f, animSunsetGunmanRun);
		FRAME_ADD_OFFSETX(frame, 340.f, 1455.f, 85.f, 105.f, 0.470588f, animSunsetGunmanRun);
		FRAME_ADD_OFFSETX(frame, 425.f, 1455.f, 85.f, 100.f, 0.470588f, animSunsetGunmanRun);
		FRAME_ADD_OFFSETX(frame, 510.f, 1455.f, 85.f, 100.f, 0.470588f, animSunsetGunmanRun);
		FRAME_ADD_OFFSETX(frame, 595.f, 1455.f, 85.f, 105.f, 0.470588f, animSunsetGunmanRun);
		FRAME_ADD_OFFSETX(frame, 680.f, 1455.f, 85.f, 105.f, 0.470588f, animSunsetGunmanRun);
		FRAME_ADD_OFFSETX(frame, 765.f, 1455.f, 85.f, 100.f, 0.470588f, animSunsetGunmanRun);

		AUTO_OFFSET_CALC_Y(animSunsetGunmanIdle);
		AUTO_OFFSET_CALC_Y(animSunsetGunmanDie);
		AUTO_OFFSET_CALC_Y(animSunsetGunmanJump);
		AUTO_OFFSET_CALC_Y(animSunsetGunmanRun);

		std::wstring costumeString;
		UINT costumeNumber = static_cast<UINT>(ePlayerCostume::SunsetGunman);
		GetCostumeString(costumeNumber, costumeString);

		Costume* sunsetGunmanCostume = new Costume();

		std::wstring idleAnimWstr = sunsetGunmanCostume->costumeIdleAnim = costumeString + WIDE("_Idle");
		std::wstring dieAnimWstr = sunsetGunmanCostume->costumeDieAnim = costumeString + WIDE("_Die");
		std::wstring jumpAnimWstr = sunsetGunmanCostume->costumeJumpAnim = costumeString + WIDE("_Jump");
		std::wstring runAnimWstr = sunsetGunmanCostume->costumeRunAnim = costumeString + WIDE("_Run");

		sunsetGunmanCostume->costumeSize = canvasSize;

		mAnimator->Create(idleAnimWstr, texture, animSunsetGunmanIdle, canvasSize, false);
		mAnimator->Create(dieAnimWstr, texture, animSunsetGunmanDie, canvasSize, false);
		mAnimator->Create(jumpAnimWstr, texture, animSunsetGunmanJump, canvasSize, false);
		mAnimator->Create(runAnimWstr, texture, animSunsetGunmanRun, canvasSize, false);

		mCostume.push_back(sunsetGunmanCostume);
	}
#pragma endregion
}