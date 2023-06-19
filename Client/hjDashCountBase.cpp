#include "hjDashCountBase.h"
#include "hjObject.h"
#include "hjAnimator.h"

namespace hj
{
	DashCountBase::DashCountBase()
		: Actor()
		, mbIsActivateDashCount(false)
		, mDashCount(nullptr)
	{
		SetNameAndCreateSpriteRenderer(WIDE("DashCountBase"), WIDE("MTRL_UI_HUD_PlayerHUD"), WIDE("Mesh_Rect"));
		CreateBaseUIAnimation();

		mDashCount = object::Instantiate<Actor>(eLayerType::UI);
		CreateDashCountUIAnimation();

		mDashCount->GetTransform()->SetParent(this->GetTransform());
		mDashCount->AddPositionX(4.f);
		mDashCount->AddPositionZ(-0.1f);
	}
	
	DashCountBase::~DashCountBase()
	{
		mDashCount = nullptr;
	}

	void DashCountBase::ActivateDashCount()
	{
		mbIsActivateDashCount = true;

		if (mDashCount)
		{
			mDashCount->Activate();
		}
	}
	void DashCountBase::DeactivateDashCount()
	{
		mbIsActivateDashCount = false;

		if (mDashCount)
		{
			mDashCount->Pause();
		}
	}

	void DashCountBase::CreateBaseUIAnimation()
	{
		CheckHasMaterialAndTexture(eTextureSlot::T0);
		Animator* animator = AddComponent<Animator>();

		std::wstring path = WIDE("03_UI/01_HUD/PlayerHUD.xml");
		std::wstring searchWstr;
		std::wstring animWstr;
		searchWstr = WIDE("DashCountBaseSlot");
		animWstr = WIDE("DashCountBaseSlot");

		float frameDuration = 1.f;
		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration, false, 0, false);
		this->InsertAnimationToAnimator(eTextureSlot::T0, true);


		
		animator->Play(animWstr);
	}

	void DashCountBase::CreateDashCountUIAnimation()
	{
		mDashCount->SetNameAndCreateSpriteRenderer(WIDE("DashCountBase"), WIDE("MTRL_UI_HUD_PlayerHUD"), WIDE("Mesh_Rect"));
		mDashCount->CheckHasMaterialAndTexture(eTextureSlot::T0);
		Animator* animator = mDashCount->AddComponent<Animator>();

		std::wstring path = WIDE("03_UI/01_HUD/PlayerHUD.xml");
		std::wstring searchWstr;
		std::wstring animWstr;
		searchWstr = WIDE("DashCountActivate");
		animWstr = WIDE("DashCountActivate");

		float frameDuration = 1.f;
		mDashCount->LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration, false, 0, false);
		mDashCount->InsertAnimationToAnimator(eTextureSlot::T0, true);

		mDashCount->GetComponent<Animator>()->Play(animWstr);
	}

}