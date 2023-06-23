#include "hjBossLifeBarUI.h"
#include "hjObject.h"
#include "hjAnimator.h"
#include "hjApplication.h"
#include "hjUIProgressBar.h"
#include "hjStage1Boss.h"

extern hj::Application application;

namespace hj
{
	BossLifeBarUI::BossLifeBarUI()
		: Actor()
		, mLifeBarGauge(nullptr)
		, mBossLifeBarImage(nullptr)
	{
		CreateHPBarAnimation();
		CreateHPBarBack();
		CreateHPBarGauge();
		CreateBossLifeBarImage();
		InitializeUIPose();
	}

	BossLifeBarUI::~BossLifeBarUI()
	{
	}

	void BossLifeBarUI::Initialize()
	{
		Actor::Initialize();
	}

	void BossLifeBarUI::Update()
	{
		Actor::Update();
	}

	void BossLifeBarUI::FixedUpdate()
	{
		Actor::FixedUpdate();
	}

	void BossLifeBarUI::Render()
	{
		Actor::Render();
	}

	void BossLifeBarUI::Death()
	{
		if (mLifeBarGauge)
		{
			mLifeBarGauge->Death();
		}

		if (mBossLifeBarImage)
		{
			mBossLifeBarImage->Death();
		}

		if (mBackground)
		{
			mBackground->Death();
		}

		Actor::Death();
	}

	void BossLifeBarUI::SetMaxHPValue(float value)
	{
		if (!mLifeBarGauge)
			return;

		mLifeBarGauge->SetMaxPercentValue(value);
	}

	void BossLifeBarUI::SetCurrentHPValue(float value)
	{
		if (!mLifeBarGauge)
			return;

		mLifeBarGauge->SetCurrentPercentValue(value);

		float CurMaxValue = mLifeBarGauge->GetMaxPercentValue();
		float lifeBarGaugeScaleX = mLifeBarGauge->GetScaleX();

		float rangedValue = (value / CurMaxValue) - 0.5f;

		if (rangedValue < -0.5f)
		{
			rangedValue = -0.5f;
		}
	}

	void BossLifeBarUI::CreateHPBarAnimation()
	{
		// slot base
		SetNameAndCreateSpriteRenderer(WIDE("BossLifeBase"), WIDE("MTRL_UI_HUD_PlayerHUD"), WIDE("Mesh_Rect"));
		CheckHasMaterialAndTexture(eTextureSlot::T0);

		Animator* animator = AddComponent<Animator>();

		std::wstring path = WIDE("03_UI/01_HUD/PlayerHUD.xml");
		std::wstring searchWstr;
		std::wstring animWstr;
		searchWstr = WIDE("BossLifeBase");
		animWstr = WIDE("BossLifeBase");

		float frameDuration = 1.f;
		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration);
		InsertAnimationToAnimator(eTextureSlot::T0, true);

		animator->Play(animWstr);
		SetScaleXY(GetScaleXY() * 1.5f);
	}

	void BossLifeBarUI::CreateHPBarBack()
	{
		mBackground = object::Instantiate<Actor>(eLayerType::UI);

		mBackground->SetNameAndCreateSpriteRenderer(WIDE("BossLifeBack"), WIDE("MTRL_UI_HUD_PlayerHUD"), WIDE("Mesh_Rect"));
		mBackground->CheckHasMaterialAndTexture(eTextureSlot::T0);

		Animator* animator = mBackground->AddComponent<Animator>();

		std::wstring path = WIDE("03_UI/01_HUD/PlayerHUD.xml");
		std::wstring searchWstr;
		std::wstring animWstr;
		searchWstr = WIDE("BossLifeBack");
		animWstr = WIDE("BossLifeBack");

		float frameDuration = 1.f;
		mBackground->LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration);
		mBackground->InsertAnimationToAnimator(eTextureSlot::T0, true);

		animator->Play(animWstr);

		mBackground->GetTransform()->SetParent(this->GetTransform());
		mBackground->SetPositionZ(1.f);
		mBackground->SetScaleXY(mBackground->GetScaleXY() * 1.5f);
	}

	void BossLifeBarUI::CreateHPBarGauge()
	{
		mLifeBarGauge = object::Instantiate<UIProgressBar>(eLayerType::UI);

		mLifeBarGauge->GetTransform()->SetParent(this->GetTransform());
		mLifeBarGauge->SetPositionZ(0.5f);
		mLifeBarGauge->SetScaleX(600.f);
		mLifeBarGauge->SetScaleY(90.f);
		mLifeBarGauge->SetPositionX(57.f);
	}

	void BossLifeBarUI::CreateBossLifeBarImage()
	{
		mBossLifeBarImage = object::Instantiate<Actor>(eLayerType::UI);

		mBossLifeBarImage->SetNameAndCreateSpriteRenderer(WIDE("BossSkellPortrait"), WIDE("MTRL_UI_HUD_PlayerHUD"), WIDE("Mesh_Rect"));
		mBossLifeBarImage->CheckHasMaterialAndTexture(eTextureSlot::T0);

		Animator* animator = mBossLifeBarImage->AddComponent<Animator>();

		std::wstring path = WIDE("03_UI/01_HUD/PlayerHUD.xml");
		std::wstring searchWstr;
		std::wstring animWstr;
		searchWstr = WIDE("BossSkellPortrait");
		animWstr = WIDE("BossSkellPortrait");

		float frameDuration = 1.f;
		mBossLifeBarImage->LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration);
		mBossLifeBarImage->InsertAnimationToAnimator(eTextureSlot::T0, true);

		animator->Play(animWstr);

		mBossLifeBarImage->GetTransform()->SetParent(this->GetTransform());
		mBossLifeBarImage->SetScaleXY(mBossLifeBarImage->GetScaleXY() * 1.5f);
		mBossLifeBarImage->SetPositionX(GetScaleX() * -0.4075f);
		mBossLifeBarImage->SetPositionZ(0.5f);
	}

	void BossLifeBarUI::InitializeUIPose()
	{
		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);

		Vector2 scale = GetScaleXY();

		Vector3 pos = Vector3(0.f, -winRect.bottom * 0.5f + scale.y * 0.6f, -10.f);

		SetPosition(pos);
	}
}