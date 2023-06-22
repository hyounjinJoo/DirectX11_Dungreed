#include "hjSwingFx.h"
#include "hjCollider2D.h"
#include "hjAttackScript.h"
#include "hjAnimator.h"
#include "hjInput.h"
#include "hjSpriteRenderer.h"

namespace hj
{
	SwingFx::SwingFx()
		: Actor()
		, mSpawnDistance(100.f)
	{
		SetNameAndCreateSpriteRenderer(WIDE("SwingFx"),
			WIDE("MTRL_FX_OPAQUE"), WIDE("Mesh_Rect"));

		AddComponent<Collider2D>();
		// 2. Animator 생성 및 Animation 추가
		Animator* animator = AddComponent<Animator>();
		if (animator)
		{
			CreateAnimation();
		}

		mAttackScript = AddComponent<AttackScript>();
		SetPositionZ(-1.f);
		
		Pause();
	}

	SwingFx::~SwingFx()
	{
	}

	void SwingFx::PlayFx()
	{
		Animation* animation = GetComponent<Animator>()->GetCurrentAnimation();
		if (animation && !animation->IsComplete())
			return;

		Vector2 ownerPos = mOwnerActor->GetWorldPositionXY();
		Vector2 mousePos = Input::GetMouseWorldPosition();

		Vector2 dir = mousePos - ownerPos;
		dir.Normalize();

		float dot = Vector2::Right.Dot(dir);
		float angle = acosf(dot);
		Vector3 cross = Vector3::Right.Cross(dir);
		angle = (cross.z < 0.f) ? XM_2PI - angle : angle;

		Vector2 newPos = dir * mSpawnDistance;
		newPos.Rotate(angle);
		newPos += ownerPos;
		SetPositionXY(newPos);
		SetRotationZ(angle - XM_PI * 0.5f);
		GetTransform()->FixedUpdate();

		Activate();

		GetComponent<Animator>()->Play(WIDE("SwingFX"), false);

		if (mAttackScript)
		{
			mAttackScript->SetDamageOn(true);
		}
	}

	void SwingFx::StopFx()
	{
		if (mAttackScript)
		{
			mAttackScript->ClearDamagedObjects();
		}

		Pause();
	}

	void SwingFx::SetDamageRange(int minDamage, int maxDamage)
	{
		if (mAttackScript)
		{
			mAttackScript->SetDamageRange(minDamage, maxDamage);
		}
	}

	void SwingFx::CreateAnimation()
	{
		CheckHasMaterialAndTexture(eTextureSlot::T0);

		std::wstring path = WIDE("08_FX/08_FX.xml");
		std::wstring searchWstr = WIDE("SwingFX/SwingFX0");
		std::wstring animWstr = WIDE("SwingFX");
		float frameDuration = 1.f / 15.f;

		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD_OFFSET_TRIM_OFFSET, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration, true, 0);
		CalcOffsetAutoY(animWstr);
		Actor::InsertAnimationToAnimator(eTextureSlot::T0);

		Animator* animator = GetComponent<Animator>();
		animator->GetCompleteEvent(animWstr) = std::bind(&SwingFx::StopFx, this);

		Animation* animation = animator->FindAnimation(animWstr);
		//float smallestX = animation->GetSpriteSize(0).x;
		//float biggestY = animation->GetSpriteSize(2).y;
		//Vector2 canvasSize = animation->GetCanvasSize();
		//
		//GetComponent<Collider2D>()->SetSize(Vector2(smallestX, biggestY) / canvasSize);
	}

}