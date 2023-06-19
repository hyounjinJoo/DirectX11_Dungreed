#include "hjBoss1SwordCreateDestroyFx.h"
#include "hjSpriteRenderer.h"
#include "hjResources.h"
#include "hjAnimator.h"

namespace hj
{
	Boss1SwordCreateDestroyFx::Boss1SwordCreateDestroyFx()
	{
		SetNameAndCreateSpriteRenderer(WIDE("Effect_Boss1_SwordCreateDestroyFX"),
										WIDE("MTRL_FX"), WIDE("Mesh_Rect"));


		// 2. Animator 생성 및 Animation 추가
		Animator* animator = AddComponent<Animator>();
		if (animator)
		{	
			CreateAnimation();
		}

		std::shared_ptr<Material> material = GetComponent<SpriteRenderer>()->GetMaterial();
		int useNewAlpha = static_cast<int>(true);
		material->SetData(eGPUParam::Int_3, &useNewAlpha);
		float newAlpha = 0.85f;
		material->SetData(eGPUParam::Float_1, &newAlpha);
	}

	Boss1SwordCreateDestroyFx::Boss1SwordCreateDestroyFx(const Boss1SwordCreateDestroyFx& fx)
		: Actor(fx)
	{
		Animator* animator = GetComponent<Animator>();
		assert(animator);

		std::wstring animWstr = WIDE("SwordCreateDestroyFX");
		animator->GetCompleteEvent(animWstr) = std::bind(&Boss1SwordCreateDestroyFx::FxEnd, this);

		std::shared_ptr<Material> material = GetComponent<SpriteRenderer>()->GetMaterial();
		int useNewAlpha = static_cast<int>(true);
		material->SetData(eGPUParam::Int_3, &useNewAlpha);
		float newAlpha = 0.85f;
		material->SetData(eGPUParam::Float_1, &newAlpha);
	}

	Boss1SwordCreateDestroyFx::~Boss1SwordCreateDestroyFx()
	{
	}

	hj::GameObject* Boss1SwordCreateDestroyFx::Clone()
	{
		return new Boss1SwordCreateDestroyFx(*this);
	}

	void Boss1SwordCreateDestroyFx::FxEnd()
	{
		Pause();
	}

	void Boss1SwordCreateDestroyFx::PlayFx()
	{
		Animator* animator = GetComponent<Animator>();
		std::wstring animWstr = WIDE("SwordCreateDestroyFX");
		animator->Play(animWstr, false);
	}

	void Boss1SwordCreateDestroyFx::CreateAnimation()
	{
		CheckHasMaterialAndTexture(eTextureSlot::T0);

		std::wstring path = WIDE("08_FX/08_FX.xml");
		std::wstring searchWstr = WIDE("IcicleDestroyFX");
		std::wstring animWstr = WIDE("SwordCreateDestroyFX");
		float frameDuration = 0.5f / 3.f;

		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration, true, 0, false);
		Actor::InsertAnimationToAnimator(eTextureSlot::T0, true);

		Animator* animator = GetComponent<Animator>();
		animator->GetCompleteEvent(animWstr) = std::bind(&Boss1SwordCreateDestroyFx::FxEnd, this);		
	}
}