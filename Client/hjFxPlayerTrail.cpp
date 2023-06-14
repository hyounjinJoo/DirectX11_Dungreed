#include "hjFxPlayerTrail.h"
#include "hjTime.h"
#include "hjSpriteRenderer.h"
#include "hjResources.h"
#include "hjAnimator.h"
#include "hjPlayer.h"

namespace hj
{
	FxPlayerTrail::FxPlayerTrail()
		: mOwner(nullptr)
		, mMaterial(nullptr)
		, mbActivateTrail(false)
		, mActivateTimer(0.f)
		, mActivateLimitTime(1.f)
		, mAnimator(nullptr)
	{
		SetName(WIDE("플레이어 대쉬잔상"));
	}

	FxPlayerTrail::~FxPlayerTrail()
	{
		mOwner = nullptr;
		mMaterial = nullptr;
	}

	void FxPlayerTrail::Initialize()
	{
		GameObject::Initialize();
	}

	void FxPlayerTrail::Update()
	{
		GameObject::Update();

		if (!mbActivateTrail)
		{
			if (mOwner)
			{
				SetPositionXY(mOwner->GetWorldPositionXY());
				Vector3 ownerScale = mOwner->GetWorldScale();
				SetScaleXY(Vector2(ownerScale.x, ownerScale.y));
				bool isFlip = mOwner->IsFlip();
				if (isFlip)
				{
					SetRotationY(PI);
				}
				else
				{
					SetRotationY(0.f);
				}
			}
		}
		else
		{
			mActivateTimer += Time::FixedDeltaTime();
			if (mActivateTimer >= mActivateLimitTime)
			{
				mActivateTimer = 0.f;
				mbActivateTrail = false;
			}
		}
	}

	void FxPlayerTrail::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void FxPlayerTrail::Render()
	{
		if (mbActivateTrail)
		{
			mMaterial->SetData(eGPUParam::Int_4, &mbActivateTrail);
			Vector4 meshColor = Vector4::One;
			meshColor.w = (mActivateLimitTime - mActivateTimer) / mActivateLimitTime;
			mMaterial->SetData(eGPUParam::Vector4_1, &meshColor);
	
			GameObject::Render();

			bool initialFalse = false;
			mMaterial->SetData(eGPUParam::Int_4, &initialFalse);
			meshColor = Vector4::Zero;
			mMaterial->SetData(eGPUParam::Vector4_1, &meshColor);
		}
	}

	void FxPlayerTrail::SetOwner(class Player* owner)
	{
		mOwner = owner;

		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		mMaterial = MTRL_FIND_STR("MTRL_Char_Adventurer");
		std::shared_ptr<Mesh> mesh = MESH_FIND_STR("Mesh_Rect");
		sr->SetMaterial(mMaterial);
		sr->SetMesh(mesh);

		mAnimator = AddComponent<Animator>();
		if (mMaterial && mAnimator)
		{
			std::wstring costumeString;
			std::wstring jumpAnimWstr;
			Animation* ownersJumpAnim = nullptr;
			UINT maxCostume = static_cast<UINT>(ePlayerCostume::End);
			for (UINT costumeNumber = 0; costumeNumber < maxCostume; ++costumeNumber)
			{
				owner->GetCostumeString(costumeNumber, costumeString);
				jumpAnimWstr = costumeString + WIDE("_Jump");
				ownersJumpAnim = owner->GetComponent<Animator>()->FindAnimation(jumpAnimWstr)->Clone();
				if(ownersJumpAnim)
					mAnimator->AddAnimation(ownersJumpAnim);
			}

			owner->GetCurrentCostumeString(costumeString);
			jumpAnimWstr = costumeString + WIDE("_Jump");
			mAnimator->Play(jumpAnimWstr, true);
		}
	}

	void FxPlayerTrail::ChangeCostume(UINT costumeNumber)
	{
		if (!mOwner)
			return;

		std::wstring costumeString;
		mOwner->GetCurrentCostumeString(costumeString);
		std::wstring jumpAnimWstr = costumeString + WIDE("_Jump");
		mAnimator->Play(jumpAnimWstr, true);
	}
}