#include "hjSwordOfExplorer.h"
#include "hjAnimator.h"
#include "hjCollider2D.h"
#include "hjSwingFx.h"
#include "hjObject.h"
#include "hjPlayerHand.h"
#include "hjArmRotatorScript.h"
#include "hjTime.h"
#include "hjCameraScript.h"
#include "hjAudioClip.h"
#include "hjResourceManager.h"
#include "hjAudioSource.h"

namespace hj::object::item::weapon
{
	SwordOfExplorer::SwordOfExplorer()
		: Actor()
		, Weapon(eWeaponType::Melee, eItemClass::Common)
		, mAttackFx(nullptr)
	{
		SetName(WIDE("WEAPON_MEELEE_01_모험가의 검"));

		mItemName = std::string("모험가의 검");
		mItemDescription = std::string("잘 손질된 모험가의 검");
		mWeaponInfo.mMinAttack = 10;
		mWeaponInfo.mMaxAttack = 11;
		mWeaponInfo.mAttackPerSec = 3.3f;
		mWeaponInfo.mbUseManualDistance = false;
		mWeaponInfo.mManualDistance = 0.f;
		mWeaponInfo.mFirstOffsetAngle = 20.f;
		mWeaponInfo.mSecondOffsetAngle = -200.f;
		mWeaponInfo.mFirstMinDistance = Vector2(-10.f, 10.f);
		mWeaponInfo.mFirstMaxDistance = Vector2(35.f, 45.f);

		mWeaponInfo.mSecondMinDistance = Vector2(25.f, -40.f);
		mWeaponInfo.mSecondMaxDistance = Vector2(2.f, 25.f);
		mWeaponInfo.mbUseOriginAngle = false;

		SetNameAndCreateSpriteRenderer(GetName(),
			WIDE("MTRL_Weapon_Common"), WIDE("Mesh_Rect"));

		// 2. Animator 생성 및 Animation 추가
		Animator* animator = AddComponent<Animator>();
		if (animator)
		{
			CreateAnimation();
		}

		AddComponent<Collider2D>();

		mAttackFx = object::Instantiate<SwingFx>(eLayerType::PlayerAttack_NotForeGround);
		mAttackFx->SetDamageRange(mWeaponInfo.mMinAttack, mWeaponInfo.mMaxAttack);

		mAttackSoundObj = object::Instantiate<GameObject>(eLayerType::UI);
		std::shared_ptr<AudioClip> clip = ResourceManager::Load<AudioClip>(WIDE("Swing"), WIDE("swing1.mp3"));
		AudioSource* audioSrc = mAttackSoundObj->AddComponent<AudioSource>();
		clip->SetLoop(false);
		audioSrc->SetClip(clip);
	}

	SwordOfExplorer::~SwordOfExplorer()
	{
		if (mAttackSoundObj)
		{
			mAttackSoundObj->Death();
		}
	}
	
	void SwordOfExplorer::Initialize()
	{
		Actor::Initialize();
	}
	
	void SwordOfExplorer::Update()
	{
		if (mAttackCoolTimer > 0.f)
		{
			mAttackCoolTimer -= Time::FixedDeltaTime();

			if (mAttackCoolTimer < 0.f)
				mAttackCoolTimer = 0.f;
		}

		Actor::Update();
	}
	
	void SwordOfExplorer::FixedUpdate()
	{
		Actor::FixedUpdate();
	}
	
	void SwordOfExplorer::Render()
	{
		Actor::Render();
	}

	void SwordOfExplorer::Attack()
	{
		if (mAttackCoolTimer > 0.f)
		{
			return;
		}
		mAttackSoundObj->GetComponent<AudioSource>()->Play();

		mAttackCoolTimer = 1.f / mWeaponInfo.mAttackPerSec;

		Camera* camera = renderer::mainCamera;
		GameObject* cameraObj = camera->GetOwner();
		std::vector<Script*> scripts = cameraObj->GetScripts();

		for (auto iter : scripts)
		{
			if (dynamic_cast<CameraScript*>(iter))
			{
				CameraScript* script = static_cast<CameraScript*>(iter);
				script->Shake();
			}
		}

		mAttackFx->PlayFx();

		PlayerHand* playerHand = reinterpret_cast<PlayerHand*>(mHand);

		if (!playerHand)
		{
			return;
		}

		if (mWeaponInfo.mbIsNeedToRotX)
		{
			float nextRotX = playerHand->GetRotationX();
			nextRotX -= XM_PI;
			while (nextRotX < 0.f)
			{
				nextRotX += XM_2PI;
			}

			playerHand->SetRotationX(nextRotX);

			nextRotX = playerHand->GetHandOwnerTR()->GetRotationX();
			nextRotX -= XM_PI;
			while (nextRotX < 0.f)
			{
				nextRotX += XM_2PI;
			}

			playerHand->GetHandOwnerTR()->SetRotationX(nextRotX);
		}

		ArmRotatorScript* rotatorScript = playerHand->GetRotatorScript();

		if (!rotatorScript)
			return;

		float firstOffsetAngle = mWeaponInfo.mFirstOffsetAngle;
		float secondOffsetAngle = mWeaponInfo.mSecondOffsetAngle;

		float rotatorOffsetAngle = rotatorScript->GetOffsetAngle();
		if (firstOffsetAngle == rotatorOffsetAngle)
		{
			rotatorScript->SetOffsetAngle(secondOffsetAngle);

			Vector2 nextMinDistance = mWeaponInfo.mSecondMinDistance;
			Vector2 nextMaxDistance = mWeaponInfo.mSecondMaxDistance;
			rotatorScript->SetDistanceInfo(nextMinDistance, nextMaxDistance);
		}
		else
		{
			rotatorScript->SetOffsetAngle(firstOffsetAngle);

			Vector2 nextMinDistance = mWeaponInfo.mFirstMinDistance;
			Vector2 nextMaxDistance = mWeaponInfo.mFirstMaxDistance;
			rotatorScript->SetDistanceInfo(nextMinDistance, nextMaxDistance);
		}
	}

	void SwordOfExplorer::CreateAnimation()
	{
		CheckHasMaterialAndTexture(eTextureSlot::T0);

		std::wstring path = WIDE("02_Object/03_Weapon/Weapon_Common.xml");
		std::wstring searchWstr = WIDE("SwordOfExplorer");
		std::wstring animWstr = WIDE("SwordOfExplorer");
		float frameDuration = 1.f / (mWeaponInfo.mAttackPerSec);

		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD_OFFSET, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration);
		Actor::InsertAnimationToAnimator(eTextureSlot::T0);

		GetComponent<Animator>()->Play(animWstr);
	}

	hj::Actor* SwordOfExplorer::GetFxActor()
	{
		Actor* result = nullptr;

		if (mAttackFx)
		{
			result = mAttackFx;
		}

		return result;
	}

	void SwordOfExplorer::SetOwnerActor(Actor* owner)
	{
		Actor::SetOwnerActor(owner);

		if (mAttackFx)
		{
			mAttackFx->SetOwnerActor(owner);
		}
	}

}