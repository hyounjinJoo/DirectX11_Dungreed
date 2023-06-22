#include "hjArrow.h"
#include "hjAnimator.h"
#include "hjCollider2D.h"
#include "hjAttackScript.h"
#include "hjArrowFx.h"
#include "hjTime.h"
#include "hjObject.h"
#include "hjRoomPassThrough.h"

namespace hj
{
	Arrow::Arrow()
		: Actor()
		, mMoveSpeed(1600.f)
		, mActivateLimitTime(0.3f)
		, mActivateTimer(0.f)
	{

		SetNameAndCreateSpriteRenderer(GetName(),
			WIDE("MTRL_Stage1"), WIDE("Mesh_Rect"));

		Animator* animator = AddComponent<Animator>();
		if (animator)
		{
			CreateAnimation();
		}
		AddComponent<Collider2D>();
		AddComponent<AttackScript>()->SetDamageRange(20, 35);

		ArrowScript* arrowScript = AddComponent<ArrowScript>();

		Vector2 scale = GetScaleXY();
		scale *= 0.8f;
		SetScaleXY(scale);
	}
	
	Arrow::~Arrow()
	{
	}

	void Arrow::Update()
	{
		mActivateTimer += Time::ActualDeltaTime();
		if (mActivateTimer > mActivateLimitTime)
		{
			ArrowScript* arrowScript = GetScript<ArrowScript>();
			if (arrowScript)
			{
				arrowScript->Hit();
			}
		}
		else
		{
			Vector2 moveDir = Vector2(WorldUp().x, WorldUp().y);
			AddPositionXY(moveDir * mMoveSpeed * Time::FixedDeltaTime());	
		}

		Actor::Update();
	}

	void Arrow::DamageOn()
	{
		AttackScript* script = GetScript<AttackScript>();
		if (script)
		{
			script->SetDamageOn(true);
		}
	}

	void Arrow::CreateAnimation()
	{
		CheckHasMaterialAndTexture(eTextureSlot::T0);

		std::wstring path = WIDE("02_Object/02_Monster/00_Normal/Stage01.xml");
		std::wstring searchWstr = WIDE("Arrow00");
		std::wstring animWstr = WIDE("Arrow");
		float frameDuration = 1.f;

		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD_OFFSET, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration);
		Actor::InsertAnimationToAnimator(eTextureSlot::T0);

		GetComponent<Animator>()->Play(animWstr);
	}
}

namespace hj
{
	ArrowScript::ArrowScript()
		: Script()
		, mArrowFx(nullptr)
	{
	}

	ArrowScript::~ArrowScript()
	{
		mArrowFx = nullptr;
	}

	void ArrowScript::OnCollisionStay(Collider* collider)
	{
		if (!collider)
		{
			return;
		}

		if (dynamic_cast<RoomPassThrough*>(collider->GetOwner()))
		{
			return;
		}

		Hit();
	}

	void ArrowScript::SetArrowFx(ArrowFx* fx)
	{
		if (fx)
		{
			mArrowFx = fx;
		}
	}

	void ArrowScript::Hit()
	{
		GetOwner()->GetScript<AttackScript>()->ClearDamagedObjects();

		ArrowFx* fx = object::Instantiate<ArrowFx>(eLayerType::ForeGround);
		fx->GetTransform()->SetPositionXY(GetOwner()->GetWorldPositionXY());
		fx->AddPosition(GetOwner()->Up() * GetOwner()->GetScaleY() * 2.f);
		fx->SetPositionZ(-1.f);
		fx->SetRotationZ(GetOwner()->GetWorldRotationZ());
		fx->PlayFx();

		GetOwner()->Death();
	}

}