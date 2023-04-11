#include "hjFxPlayerJump.h"
#include "hjSpriteRenderer.h"
#include "hjResources.h"
#include "hjMaterial.h"
#include "hjMesh.h"
#include "hjTexture.h"
#include "hjAnimation.h"
#include "hjAnimator.h"
#include "hjPlayer.h"

namespace hj
{
	FxPlayerJump::FxPlayerJump()
		: mOwner(nullptr)
		, mJumpEffect(JumpEffect::Single)
		, mbActivateEffect(false)
		, mOffsetPos(Vector2::Zero)
		, mAnimator(nullptr)
	{
		SetName(WIDE("플레이어 점프"));

		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = MTRL_FIND("MTRL_FX");
		std::shared_ptr<Mesh> mesh = MESH_FIND("Mesh_Rect");
		sr->SetMaterial(material);
		sr->SetMesh(mesh);

		mAnimator = AddComponent<Animator>();
		std::shared_ptr<Texture> texture = material->GetTexture();
		Vector2 atlasTexSize = texture->GetTexSize();

		float duration = 0.f;
		duration = 1.f / 120.f;
		CREATE_ANIM(animPlayerJump, frame, atlasTexSize, duration);
		FRAME_ADD_OFFSETX(frame, 3781.f, 0.f, 36.f, 8.f, 0.5f, animPlayerJump);
		FRAME_ADD_OFFSETX(frame, 440.f, 20.f, 24.f, 20.f, 0.583333f, animPlayerJump);
		FRAME_ADD_OFFSETX(frame, 2096.f, 20.f, 20.f, 24.f, 0.5f, animPlayerJump);
		FRAME_ADD_OFFSETX(frame, 2116.f, 20.f, 20.f, 24.f, 0.5f, animPlayerJump);
		FRAME_ADD_OFFSETX(frame, 3817.f, 0.f, 4.f, 8.f, 0.5f, animPlayerJump);


		CREATE_SHEET(animPlayerDoubleJump);

		FRAME_ADD_OFFSETX(frame, 6697.f, 0.f, 20.f, 16.f, 0.5f, animPlayerDoubleJump);
		FRAME_ADD_OFFSETX(frame, 3644.f, 20.f, 36.f, 32.f, 0.5f, animPlayerDoubleJump);
		FRAME_ADD_OFFSETX(frame, 2048.f, 20.f, 28.f, 24.f, 0.5f, animPlayerDoubleJump);
		FRAME_ADD_OFFSETX(frame, 3680.f, 20.f, 32.f, 32.f, 0.5625f, animPlayerDoubleJump);
		FRAME_ADD_OFFSETX(frame, 3712.f, 20.f, 32.f, 32.f, 0.5625f, animPlayerDoubleJump);
		FRAME_ADD_OFFSETX(frame, 404.f, 20.f, 36.f, 20.f, 0.5f, animPlayerDoubleJump);

		AUTO_OFFSET_CALC_Y(animPlayerJump);
		AUTO_OFFSET_CALC_Y(animPlayerDoubleJump);

		mAnimator->Create(WIDE("FX_PlayerJump"), texture, animPlayerJump, canvasSize, false);
		mAnimator->Create(WIDE("FX_PlayerDoubleJump"), texture, animPlayerDoubleJump, canvasSize, false);

		SetScaleXY(canvasSize);
		mOffsetPos.x = canvasSize.x * 0.5f;
		mOffsetPos.y = canvasSize.y * 0.5f;
	}
	FxPlayerJump::~FxPlayerJump()
	{
	}
	void FxPlayerJump::Initialize()
	{
		GameObject::Initialize();
	}
	void FxPlayerJump::Update()
	{
		GameObject::Update();

		if (!mbActivateEffect)
		{
			if (mOwner)
			{
				if(mOwner->IsFlip())
					SetPositionXY(mOwner->GetWorldRightBottom() + Vector2(-mOffsetPos.x, mOffsetPos.y));
				else
					SetPositionXY(mOwner->GetWorldLeftBottom() + mOffsetPos);
			}
		}
		else
		{
			if (mAnimator->GetCurrentAnimation()->IsComplete())
				mbActivateEffect = false;
		}
	}

	void FxPlayerJump::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void FxPlayerJump::Render()
	{
		if (mbActivateEffect)
			GameObject::Render();
	}

	void FxPlayerJump::SetOwner(Player* owner)
	{
		mOwner = owner;
	}

	void FxPlayerJump::ActivateEffect(JumpEffect jumpEffect)
	{
		if (mbActivateEffect)
			return;

		mbActivateEffect = true;
		switch (jumpEffect)
		{
		case hj::JumpEffect::Single:
			mAnimator->Play(L"FX_PlayerJump", false);
			break;
		case hj::JumpEffect::Double:
			mAnimator->Play(L"FX_PlayerDoubleJump", false);
			break;
		case hj::JumpEffect::End:
		default:
			break;
		}
	}
}