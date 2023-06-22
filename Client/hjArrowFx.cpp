#include "hjArrowFx.h"
#include "hjAnimator.h"
#include "hjCameraScript.h"

namespace hj
{
	ArrowFx::ArrowFx()
		: Actor()
	{
		SetNameAndCreateSpriteRenderer(WIDE("ArrowFx"),
			WIDE("MTRL_FX_OPAQUE"), WIDE("Mesh_Rect"));

		// 2. Animator 생성 및 Animation 추가
		Animator* animator = AddComponent<Animator>();
		if (animator)
		{
			CreateAnimation();
		}

		//SetRotationZ(XM_PI * 0.5f);
	}

	ArrowFx::~ArrowFx()
	{
	}
	
	void ArrowFx::PlayFx()
	{
		Activate();

		GetComponent<Animator>()->Play(WIDE("ArrowFX"), false);

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
	}
	
	void ArrowFx::StopFx()
	{
		Death();
	}
	
	void ArrowFx::CreateAnimation()
	{
		CheckHasMaterialAndTexture(eTextureSlot::T0);

		std::wstring path = WIDE("08_FX/08_FX.xml");
		std::wstring searchWstr = WIDE("ArrowFX");
		std::wstring animWstr = WIDE("ArrowFX");
		float frameDuration = 1.f / 15.f;

		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD_OFFSET_TRIM_OFFSET, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration, true, 0);
		CalcOffsetAutoY(animWstr);
		Actor::InsertAnimationToAnimator(eTextureSlot::T0);

		Animator* animator = GetComponent<Animator>();
		animator->GetCompleteEvent(animWstr) = std::bind(&ArrowFx::StopFx, this);
	}
}