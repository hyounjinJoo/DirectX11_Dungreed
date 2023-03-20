#include "hjPlayer.h"
#include "hjPlayerHand.h"
#include "hjSpriteRenderer.h"
#include "hjResources.h"
#include "hjRigidBody.h"
#include "hjPlayerScript.h"
#include "hjInput.h"

namespace hj
{
	Player::Player()
		: GameObject()
		, mHand(nullptr)
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
		AddComponent<PlayerScript>();
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

		float posX = GetPositionX();
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
		CREATE_ANIM(animAdventurerIdle, frame, atlasTexSize, duration, Vector2(0.f, 0.f));

		frame.offset.x = -4.f;
		frame.offset.y = 2.f;
		FRAME_ADD(frame, 128.f, 0.f, 52.f, 76.f, animAdventurerIdle);

		frame.offset.x = -4.f;
		frame.offset.y = 0.f;
		FRAME_ADD(frame, 180.f, 0.f, 52.f, 80.f, animAdventurerIdle);
		frame.offset.x = -2.f;
		FRAME_ADD(frame, 232.f, 0.f, 56.f, 80.f, animAdventurerIdle);

		frame.offset.x = 0.f;
		frame.offset.y = 2.f;
		FRAME_ADD(frame, 288.f, 0.f, 60.f, 76.f, animAdventurerIdle);

		frame.offset.x = -2.f;
		FRAME_ADD(frame, 348.f, 0.f, 56.f, 76.f, animAdventurerIdle);

#define AUTO_OFFSET_CALC(spriteSheetVarName) \
		for (auto spriteFrame : spriteSheetVarName)\
		{\
			if (spriteFrame.size.x != canvasSize.x)\
				spriteFrame.offset.x = (canvasSize.x - spriteFrame.size.x) / -2.f;\
			else\
				spriteFrame.offset.x = 0.f;\
			if (spriteFrame.size.y != canvasSize.y)\
				spriteFrame.offset.y = (canvasSize.x - spriteFrame.size.x) / 2.f;\
			else\
				spriteFrame.offset.y = 0.f;\
		}\


		CREATE_SHEET(animAdventurerDie);
		FRAME_ADD(frame, 52.f, 0.f, 76.f, 48.f, animAdventurerDie);
		
		CREATE_SHEET(animAdventurerJump);
		FRAME_ADD(frame, 404.f, 0.f, 52.f, 76.f, animAdventurerJump);
		
		CREATE_SHEET(animAdventurerRun);
		FRAME_ADD(frame, 456.f, 0.f, 68.f, 76.f, animAdventurerRun);
		FRAME_ADD(frame, 524.f, 0.f, 56.f, 80.f, animAdventurerRun);
		FRAME_ADD(frame, 580.f, 0.f, 56.f, 80.f, animAdventurerRun);
		FRAME_ADD(frame, 636.f, 0.f, 60.f, 76.f, animAdventurerRun);
		FRAME_ADD(frame, 696.f, 0.f, 60.f, 76.f, animAdventurerRun);
		FRAME_ADD(frame, 756.f, 0.f, 52.f, 80.f, animAdventurerRun);
		FRAME_ADD(frame, 808.f, 0.f, 56.f, 80.f, animAdventurerRun);
		FRAME_ADD(frame, 864.f, 0.f, 64.f, 76.f, animAdventurerRun);
		
		AUTO_OFFSET_CALC(animAdventurerIdle);
		AUTO_OFFSET_CALC(animAdventurerDie);
		AUTO_OFFSET_CALC(animAdventurerJump);
		AUTO_OFFSET_CALC(animAdventurerRun);

		mAnimator->Create(WIDE("Adeventurer_Idle"), texture, animAdventurerIdle, canvasSize, false);
		mAnimator->Create(WIDE("Adeventurer_Die"), texture, animAdventurerDie, Vector2(76.f, 80.f), false);
		mAnimator->Create(WIDE("Adeventurer_Jump"), texture, animAdventurerJump, Vector2(76.f, 80.f), false);
		mAnimator->Create(WIDE("Adeventurer_Run"), texture, animAdventurerRun, Vector2(76.f, 80.f), false);
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