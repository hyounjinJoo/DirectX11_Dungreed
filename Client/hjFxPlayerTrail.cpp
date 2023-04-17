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
		, mActivateTime(0.f)
		, mActivateTimer(1.f)
		, mAnimator(nullptr)
	{
		SetName(WIDE("플레이어 대쉬잔상"));
	}

	FxPlayerTrail::~FxPlayerTrail()
	{
	}

	void FxPlayerTrail::Initialize()
	{
		assert(mOwner);

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
			mActivateTime += Time::FixedDeltaTime();
			if (mActivateTime >= mActivateTimer)
			{
				mActivateTime = 0.f;
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
			mMaterial.get()->SetData(eGPUParam::Int_4, &mbActivateTrail);
			Vector4 test = Vector4(1.f, 1.f, 1.f, 1.f);
			mMaterial.get()->SetData(eGPUParam::Vector4_1, &test);
	
			GameObject::Render();

			bool initialFalse = false;
			mMaterial.get()->SetData(eGPUParam::Int_4, &initialFalse);
			test = Vector4::Zero;
			mMaterial.get()->SetData(eGPUParam::Vector4_1, &test);
		}
	}

	void FxPlayerTrail::SetOwner(class Player* owner)
	{
		mOwner = owner;

		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		mMaterial = MTRL_FIND("MTRL_Char_Adventurer");
		std::shared_ptr<Mesh> mesh = MESH_FIND("Mesh_Rect");
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