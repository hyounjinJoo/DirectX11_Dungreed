#include "hjPlayerLifeBarUI.h"
#include "hjObject.h"
#include "hjAnimator.h"
#include "hjApplication.h"
#include "hjUIProgressBar.h"
#include "hjPlayerLifeBarWaveUI.h"

extern hj::Application application;

namespace hj
{
	PlayerLifeBarUI::PlayerLifeBarUI()
		: Actor()
		, mLifeBarGauge(nullptr)
		, mLifeBarWave(nullptr)
	{
		CreateHPBarAnimation();
		CreateHPBarBack();
		CreateHPBarGauge();
		CreateHPBarWave();
		InitializeUIPose();
	}

	PlayerLifeBarUI::~PlayerLifeBarUI()
	{
	}

	void PlayerLifeBarUI::Initialize()
	{
		Actor::Initialize();
	}

	void PlayerLifeBarUI::Update()
	{
		Actor::Update();
	}

	void PlayerLifeBarUI::FixedUpdate()
	{
		Actor::FixedUpdate();
	}

	void PlayerLifeBarUI::Render()
	{
		Actor::Render();
	}

	void PlayerLifeBarUI::SetMaxHPValue(float value)
	{
		if (!mLifeBarGauge)
			return;

		mLifeBarGauge->SetMaxPercentValue(value);
	}

	void PlayerLifeBarUI::SetCurrentHPValue(float value)
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

		if (rangedValue == 0.5f || rangedValue == -0.5f)
		{
			mLifeBarWave->Pause();
		}
		else
		{
			mLifeBarWave->Activate();
		}

		mLifeBarWave->SetPositionX(lifeBarGaugeScaleX * rangedValue);
	}

	void PlayerLifeBarUI::CreateHPBarAnimation()
	{
		// slot base
		SetNameAndCreateSpriteRenderer(WIDE("PlayerHBBarBase"), WIDE("MTRL_UI_HUD_PlayerHUD"), WIDE("Mesh_Rect"));
		CheckHasMaterialAndTexture(eTextureSlot::T0);

		Animator* animator = AddComponent<Animator>();

		std::wstring path = WIDE("03_UI/01_HUD/PlayerHUD.xml");
		std::wstring searchWstr;
		std::wstring animWstr;
		searchWstr = WIDE("PlayerLifeBase");
		animWstr = WIDE("PlayerLifeBase");

		float frameDuration = 1.f;
		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration);
		InsertAnimationToAnimator(eTextureSlot::T0, true);

		animator->Play(animWstr);
	}

	void PlayerLifeBarUI::CreateHPBarBack()
	{
		Actor* Background = object::Instantiate<Actor>(eLayerType::UI);

		Background->SetNameAndCreateSpriteRenderer(WIDE("PlayerHBBarBack"), WIDE("MTRL_UI_HUD_PlayerHUD"), WIDE("Mesh_Rect"));
		Background->CheckHasMaterialAndTexture(eTextureSlot::T0);

		Animator* animator = Background->AddComponent<Animator>();

		std::wstring path = WIDE("03_UI/01_HUD/PlayerHUD.xml");
		std::wstring searchWstr;
		std::wstring animWstr;
		searchWstr = WIDE("PlayerLifeBack");
		animWstr = WIDE("PlayerLifeBack");

		float frameDuration = 1.f;
		Background->LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration);
		Background->InsertAnimationToAnimator(eTextureSlot::T0, true);

		animator->Play(animWstr);

		Background->GetTransform()->SetParent(this->GetTransform());
		Background->SetPositionZ(1.f);
	}

	void PlayerLifeBarUI::CreateHPBarGauge()
	{
		mLifeBarGauge = object::Instantiate<UIProgressBar>(eLayerType::UI);

		mLifeBarGauge->GetTransform()->SetParent(this->GetTransform());
		mLifeBarGauge->SetPositionZ(0.5f);
		mLifeBarGauge->SetScaleX(196.f);
		mLifeBarGauge->SetPositionX(38.f);
	}

	void PlayerLifeBarUI::CreateHPBarWave()
	{
		mLifeBarWave = object::Instantiate<PlayerLifeBarWaveUI>(eLayerType::UI);

		mLifeBarWave->GetTransform()->SetParent(mLifeBarGauge->GetTransform());
		mLifeBarWave->SetPositionZ(0.5f);

		float lifeBarGaugeScaleX = mLifeBarGauge->GetScaleX();
		mLifeBarWave->SetPositionX(lifeBarGaugeScaleX * 0.5f);
	}

	void PlayerLifeBarUI::InitializeUIPose()
	{
		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);

		float width = static_cast<float>(winRect.right - winRect.left);
		float height = static_cast<float>(winRect.bottom - winRect.top);
		
		Vector3 originLT = Vector3(0.f - width / 2.f, height / 2.f, 0.f);
		Vector2 scale = GetScaleXY();
		
		Vector3 pos = originLT + Vector3(scale.x / 2.f + 20.f, scale.y / -2.f - 20.f, -10.f);
		
		SetPosition(pos);
	}
}