#include "hjPlayer.h"
#include "hjPlayerHand.h"
#include "hjSpriteRenderer.h"
#include "hjResources.h"
#include "hjRigidBody.h"
#include "hjPlayerScript.h"
#include "hjInput.h"
#include "hjCollider2D.h"

namespace hj
{
	Player::Player()
		: GameObject()
		, mHand(nullptr)
		, mState(ePlayerState::End)
	{
		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = MTRL_FIND("MTRL_Char_Adventurer");
		std::shared_ptr<Mesh> mesh = MESH_FIND("Mesh_Rect");
		sr->SetMaterial(material);
		sr->SetMesh(mesh);

		mAnimator = AddComponent<Animator>();
		if (material && mAnimator)
		{
			std::shared_ptr<Texture> texture = material->GetTexture();
			if (texture)
			{
				CreateAnimation();
			}
		}

		mRigidBody = AddComponent<RigidBody>();
		mPlayerScript = AddComponent<PlayerScript>();
		AddComponent<Collider2D>();
	}

	Player::~Player()
	{
	}
	
	void Player::Initialize()
	{
		GameObject::Initialize();
	}

	void Player::Update()
	{
		GameObject::Update();

	}

	void Player::FixedUpdate()
	{
		GameObject::FixedUpdate();

		float posX = mPlayerScript->GetOwnerScreenPos().x;
		float mouseScreenPosX = Input::GetMousePosition().x;

		if (posX < mouseScreenPosX)
			mAnimator->Inverse(false);
		else
			mAnimator->Inverse(true);
	}

	void Player::Render()
	{
		GameObject::Render();
	}

	void Player::CreateAnimation()
	{
		std::shared_ptr<Material> material = MTRL_FIND("MTRL_Char_Adventurer");
		std::shared_ptr<Texture> texture = material->GetTexture();
		Vector2 atlasTexSize = texture->GetTexSize();

		float duration = 0.f;
		duration = 1.f / 10.f;
		CREATE_ANIM(animAdventurerIdle, frame, atlasTexSize, duration);

		FRAME_ADD(frame, 128.f, 0.f, 52.f, 76.f, animAdventurerIdle);
		FRAME_ADD(frame, 180.f, 0.f, 52.f, 80.f, animAdventurerIdle);
		FRAME_ADD(frame, 232.f, 0.f, 56.f, 80.f, animAdventurerIdle);
		FRAME_ADD(frame, 288.f, 0.f, 60.f, 76.f, animAdventurerIdle);
		FRAME_ADD(frame, 348.f, 0.f, 56.f, 76.f, animAdventurerIdle);

		CREATE_SHEET(animAdventurerDie);
		FRAME_ADD(frame, 52.f, 0.f, 76.f, 48.f, animAdventurerDie);
		
		CREATE_SHEET(animAdventurerJump);
		FRAME_ADD(frame, 404.f, 0.f, 52.f, 76.f, animAdventurerJump);
		
		CREATE_SHEET(animAdventurerRun);
		frame.duration = 1.f / 15.f;
		FRAME_ADD(frame, 456.f, 0.f, 68.f, 76.f, animAdventurerRun);
		FRAME_ADD(frame, 524.f, 0.f, 56.f, 80.f, animAdventurerRun);
		FRAME_ADD(frame, 580.f, 0.f, 56.f, 80.f, animAdventurerRun);
		FRAME_ADD(frame, 636.f, 0.f, 60.f, 76.f, animAdventurerRun);
		FRAME_ADD(frame, 696.f, 0.f, 60.f, 76.f, animAdventurerRun);
		FRAME_ADD(frame, 756.f, 0.f, 52.f, 80.f, animAdventurerRun);
		FRAME_ADD(frame, 808.f, 0.f, 56.f, 80.f, animAdventurerRun);
		FRAME_ADD(frame, 864.f, 0.f, 64.f, 76.f, animAdventurerRun);
		
		AUTO_OFFSET_CALC(animAdventurerIdle)
		AUTO_OFFSET_CALC(animAdventurerDie)
		AUTO_OFFSET_CALC(animAdventurerJump)
		AUTO_OFFSET_CALC(animAdventurerRun)

		mAnimator->Create(WIDE("Adeventurer_Idle"), texture, animAdventurerIdle, canvasSize, false);
		mAnimator->Create(WIDE("Adeventurer_Die"), texture, animAdventurerDie, canvasSize, false);
		mAnimator->Create(WIDE("Adeventurer_Jump"), texture, animAdventurerJump, canvasSize, false);
		mAnimator->Create(WIDE("Adeventurer_Run"), texture, animAdventurerRun, canvasSize, false);
		mAnimator->Play(WIDE("Adeventurer_Idle"), true);
		GetTransform()->SetScaleXY(canvasSize);
	}

	void Player::ChangeState(ePlayerState state)
	{
		if (mState == state)
			return;

		if (mState == ePlayerState::Die)
			return;

		mState = state;

		switch (state)
		{
		case ePlayerState::Idle:
			Idle();
			break;
		case ePlayerState::Jump:
			Jump();
			break;
		case ePlayerState::Run:
			Run();
			break;
		case ePlayerState::Die:
			break;
		case ePlayerState::End:
		default:
			break;
		}
	}

	void Player::Idle()
	{
		mAnimator->Play(WIDE("Adeventurer_Idle"), true);
	}

	void Player::Run()
	{
		mAnimator->Play(WIDE("Adeventurer_Run"), true);
	}

	void Player::Jump()
	{
		mAnimator->Play(WIDE("Adeventurer_Jump"), true);
	}

}