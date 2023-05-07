#include "hjFxPlayerDust.h"
#include "hjPlayer.h"
#include "hjSpriteRenderer.h"
#include "hjResources.h"
#include "hjMaterial.h"
#include "hjTexture.h"
#include "hjAnimator.h"
#include "hjRigidBody.h"


namespace hj
{
	FxPlayerDust::FxPlayerDust()
		: mOwner(nullptr)
		, mAnimator(nullptr)
		, mbActivateEffect(false)
		, mOffsetPos(Vector2::Zero)
	{
		SetName(WIDE("플레이어 더스트"));

		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = MTRL_FIND("MTRL_FX");
		std::shared_ptr<Mesh> mesh = MESH_FIND("Mesh_Rect");
		sr->SetMaterial(material);
		sr->SetMesh(mesh);

		mAnimator = AddComponent<Animator>();
		std::shared_ptr<Texture> texture = material->GetTexture(eTextureSlot::T0);
		Vector2 atlasTexSize = texture->GetTexSize();		
		
		float duration = 0.f;
		duration = 0.25f / 6.f;
		CREATE_ANIM(animPlayerDust, frame, atlasTexSize, duration);

		FRAME_ADD_OFFSETX(frame, 5672.f, 20.f, 40.f, 40.f, 0.5f, animPlayerDust);
		FRAME_ADD_OFFSETX(frame, 6504.f, 20.f, 48.f, 44.f, 0.5f, animPlayerDust);
		FRAME_ADD_OFFSETX(frame, 6920.f, 20.f, 52.f, 48.f, 0.538462f, animPlayerDust);
		FRAME_ADD_OFFSETX(frame, 412.f, 72.f, 52.f, 52.f, 0.615385f, animPlayerDust);
		FRAME_ADD_OFFSETX(frame, 464.f, 72.f, 44.f, 52.f, 0.727273f, animPlayerDust);
		FRAME_ADD_OFFSETX(frame, 0.f, 60.f, 1.f, 1.f, 0.5f, animPlayerDust);
		
		AUTO_OFFSET_CALC_Y(animPlayerDust);
		
		mAnimator->Create(WIDE("FX_PlayerDust"), texture, animPlayerDust, canvasSize, false);
		
		SetScaleXY(canvasSize);
		mOffsetPos.y = canvasSize.y * 0.5f;
	}
	FxPlayerDust::~FxPlayerDust()
	{
	}
	void FxPlayerDust::Initialize()
	{
		GameObject::Initialize();
	}

	void FxPlayerDust::Update()
	{
		GameObject::Update(); 
		if (!mbActivateEffect)
		{
			if (mOwner)
			{
				SetPositionXY(mOwner->GetWorldCenterBottom() + mOffsetPos);
				eMoveDir moveDir = mOwner->GetComponent<RigidBody>()->GetMoveDir();
				bool isFlip = moveDir == eMoveDir::Left ? true : false;
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
			if (mAnimator->GetCurrentAnimation()->IsComplete())
				mbActivateEffect = false;
		}
	}

	void FxPlayerDust::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void FxPlayerDust::Render()
	{
		if(mbActivateEffect)
			GameObject::Render();
	}

	void FxPlayerDust::SetOwner(class Player* owner)
	{
		mOwner = owner;
	}

	void FxPlayerDust::ActivateEffect()
	{
		if (mbActivateEffect)
			return;

		mbActivateEffect = true;
		mAnimator->Play(L"FX_PlayerDust", false);
	}

	void FxPlayerDust::ChangeAnimationDuration(float wholePlayTime)
	{
		Animation* dustAnim = mAnimator->FindAnimation(L"FX_PlayerDust");
		if (!dustAnim)
			return;

		dustAnim->ChangePlayDuration(wholePlayTime / 6.f);
	}

}