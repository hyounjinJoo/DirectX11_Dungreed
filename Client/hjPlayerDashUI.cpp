#include "hjPlayerDashUI.h"
#include "hjDashCountBase.h"
#include "hjObject.h"
#include "hjAnimator.h"
#include "hjApplication.h"

extern hj::Application application;

namespace hj
{
	PlayerDashUI::PlayerDashUI()
		: Actor()
		, mDashBaseMaxCount(2)
		, mCurrentDashCanCount(2)
		, mDashBaseUIRightEnd(nullptr)
	{
		size_t Size = 8;
		for (size_t index = 0; index < Size; ++index)
		{
			DashCountBase* base = object::Instantiate<DashCountBase>(eLayerType::UI);
			base->GetTransform()->SetParent(this->GetTransform());
			base->SetPositionX(40.f * static_cast<float>(index));
			base->SetPositionZ(0.1f * static_cast<float>(index));
			base->GetTransform()->FixedUpdate();

			mDashUIBases.push_back(base);

			if (index >= mDashBaseMaxCount)
			{
				base->Pause();
				base->DeactivateDashCount();
			}
		}

		mDashBaseUIRightEnd = object::Instantiate<Actor>(eLayerType::UI);
		mDashBaseUIRightEnd->GetTransform()->SetParent(this->GetTransform());
		CreateRightEndUIAnimation();
		mDashBaseUIRightEnd->SetPositionX(mDashUIBases[mDashBaseMaxCount - 1]->GetPositionX() + 26.f);
		
		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);

		float halfWidth = static_cast<float>(winRect.right - winRect.left) * 0.5f;
		float halfHeight = static_cast<float>(winRect.bottom - winRect.top) * 0.5f;

		Vector2 pos = Vector2(-halfWidth, halfHeight);
		pos += Vector2(42.f, -104.f);

		SetPositionXY(pos);

		SetPositionZ(-2.f);
	}

	PlayerDashUI::~PlayerDashUI()
	{
	}
	
	void PlayerDashUI::Initialize()
	{
		Actor::Initialize();
	}
	
	void PlayerDashUI::Update()
	{
		Actor::Update();
	}
	
	void PlayerDashUI::FixedUpdate()
	{
		Actor::FixedUpdate();
	}

	void PlayerDashUI::SetMaxDashCount(UINT count)
	{
		if(0 >= count || count > 8)
		{
			return;
		}
		else
		{
			mDashBaseMaxCount = count;
			if (mDashBaseUIRightEnd)
			{
				float rightEndPosX = 0.f;
				rightEndPosX = mDashBaseMaxCount * 22.f;

				mDashBaseUIRightEnd->SetPositionX(mDashUIBases[mDashBaseMaxCount - 1]->GetPositionX() + 26.f);
			}

			size_t Size = mDashUIBases.size();
			for (size_t index = 0; index < Size; ++index)
			{
				if (mDashUIBases[index])
				{
					mDashUIBases[index]->GetTransform()->SetParent(this->GetTransform());
					mDashUIBases[index]->SetPositionX(index * 40.f);

					if (mDashBaseMaxCount <= index)
					{
						mDashUIBases[index]->Pause();
						mDashUIBases[index]->DeactivateDashCount();
					}
					else
					{
						mDashUIBases[index]->Activate();
						mDashUIBases[index]->ActivateDashCount();
					}
				}
			}
		}

		mCurrentDashCanCount = mDashBaseMaxCount;
	}

	void PlayerDashUI::DecreaseCurCount()
	{
		DeactivateDashCount();
		if (mCurrentDashCanCount > 0)
		{
			mCurrentDashCanCount -= 1;
		}
	}

	void PlayerDashUI::IncreaseCurCount()
	{
		if (mDashBaseMaxCount < mCurrentDashCanCount)
		{
			return;
		}

		ActivateDashCount();
		mCurrentDashCanCount += 1;
	}

	void PlayerDashUI::ActivateDashCount()
	{
		if (mCurrentDashCanCount >= mDashUIBases.size())
		{
			return;
		}

		mDashUIBases[mCurrentDashCanCount]->ActivateDashCount();
	}

	void PlayerDashUI::DeactivateDashCount()
	{
		if (mCurrentDashCanCount <= 0 || mCurrentDashCanCount >= mDashUIBases.size())
		{
			return;
		}

		mDashUIBases[mCurrentDashCanCount - 1]->DeactivateDashCount();
	}

	void PlayerDashUI::CreateRightEndUIAnimation()
	{
		mDashBaseUIRightEnd->SetNameAndCreateSpriteRenderer(WIDE("DashCountBase"), WIDE("MTRL_UI_HUD_PlayerHUD"), WIDE("Mesh_Rect"));
		mDashBaseUIRightEnd->CheckHasMaterialAndTexture(eTextureSlot::T0);

		Animator* animator = mDashBaseUIRightEnd->AddComponent<Animator>();

		std::wstring path = WIDE("03_UI/01_HUD/PlayerHUD.xml");
		std::wstring searchWstr;
		std::wstring animWstr;
		searchWstr = WIDE("DashCountBase_RightEnd");
		animWstr = WIDE("DashCountBaseSlot_RightEnd");

		float frameDuration = 1.f;
		mDashBaseUIRightEnd->LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration, false, 0, false);
		mDashBaseUIRightEnd->InsertAnimationToAnimator(eTextureSlot::T0, true);

		animator->Play(animWstr);
	}

}