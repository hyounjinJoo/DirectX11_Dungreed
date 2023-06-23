#include "hjPlayerLifeBarWaveUI.h"
#include "hjAnimator.h"

namespace hj
{
	PlayerLifeBarWaveUI::PlayerLifeBarWaveUI()
		: Actor()
	{
		CreateAnimation();
	}
	PlayerLifeBarWaveUI::~PlayerLifeBarWaveUI()
	{
	}

	void PlayerLifeBarWaveUI::CreateAnimation()
	{
		SetNameAndCreateSpriteRenderer(WIDE("PlayerHBBarWave"), WIDE("MTRL_UI_HUD_PlayerHUD"), WIDE("Mesh_Rect"));
		CheckHasMaterialAndTexture(eTextureSlot::T0);
		AddComponent<Animator>();

		std::wstring path = WIDE("03_UI/01_HUD/PlayerHUD.xml");
		std::wstring searchWstr;
		std::wstring animWstr;
		searchWstr = WIDE("LifeWave");
		animWstr = WIDE("LifeWave");

		float frameDuration = 1.f / 10.f;
		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration, true);
		Actor::InsertAnimationToAnimator(eTextureSlot::T0);

		Animator* animator = GetComponent<Animator>();
		animator->Play(WIDE("LifeWave"));
	}
}