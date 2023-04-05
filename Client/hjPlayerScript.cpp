#include "hjPlayerScript.h"
#include "hjTransform.h"
#include "hjGameObject.h"
#include "hjInput.h"
#include "hjTime.h"
#include "hjSpriteRenderer.h"
#include "hjBaseRenderer.h"
#include "hjRigidBody.h"
#include "hjPlayer.h"
#include "hjApplication.h"
#include "CommonInclude.h"
#include "hjArmRotatorScript.h"

extern hj::Application application;
namespace hj
{
	PlayerScript::PlayerScript()
		: Script()
		, mRunForce(Vector2(2000.f, 0.f))
		, mbDash(false)
		, mDashStartedTime(0.f)
		, mMaxDashTime(0.1f)
		, mDashPower(5000.f)
	{
		InitialKeyBind();
	}

	PlayerScript::~PlayerScript()
	{
	}

	void PlayerScript::Initialize()
	{
		if (GetOwner()->GetComponent<RigidBody>())
			mOwnerRigid = GetOwner()->GetComponent<RigidBody>();			
	}

	void PlayerScript::Update()
	{
		HandleMovementInput();
		HandleJumpInput();
		HandleDownInput();
		HandleMouseInput(); 
		HandleTestInput();

		HandleOtherInput();
	}

	void PlayerScript::FixedUpdate()
	{
		FakeGroundApply();
		Dash();
	}

	void PlayerScript::Render()
	{
	}

	Vector2 PlayerScript::GetOwnerScreenPos()
	{
		Camera* mainCamera = renderer::mainCamera;

		if (mainCamera)
		{
			Matrix viewMat = mainCamera->GetViewMatrix();
			Matrix projMat = mainCamera->GetProjectionMatrix();

			Vector4 objectPosInView = XMVectorSetW(Vector4(GetOwner()->GetPosition().x, GetOwner()->GetPosition().y, GetOwner()->GetPosition().z, 1.f), 1.f);
			objectPosInView = XMVector3TransformCoord(objectPosInView, viewMat);

			Vector4 objectPosInClip = XMVector4Transform(objectPosInView, projMat);

			Vector3 objectPosNDC = Vector3(objectPosInClip) / objectPosInClip.w;


			RECT winRect;
			GetClientRect(application.GetHwnd(), &winRect);

			float width = static_cast<float>(winRect.right - winRect.left);
			float height = static_cast<float>(winRect.bottom - winRect.top);
			
			float viewX = static_cast<float>(winRect.left);
			float viewY = static_cast<float>(winRect.top);

			Vector2 objectPosInScreen;
			objectPosInScreen.x = (objectPosNDC.x + 1.0f) * 0.5f * width - (width * 0.5f);
			objectPosInScreen.y = (height * 0.5f) - (1.0f - objectPosNDC.y) * 0.5f * height;


			return objectPosInScreen;
		}

		return Vector2(FLT_MAX, FLT_MAX);
	}

	void PlayerScript::HandleMovementInput()
	{
		if (!mOwnerRigid)
			return;


		if (Input::GetKeyPressed(mKeyBindings[(UINT)playerKeyAction::MOVE_RIGHT]))
		{
			mOwnerRigid->AddForce(mRunForce);
		}
		if (Input::GetKeyPressed(mKeyBindings[(UINT)playerKeyAction::MOVE_LEFT]))
		{
			mOwnerRigid->AddForce(-mRunForce);
		}
	}

	void PlayerScript::HandleDownInput()
	{
		if (Input::GetKeyPressed(mKeyBindings[(UINT)playerKeyAction::MOVE_DOWN]))
		{
			mOwnerRigid->AddForce(Vector2(-2000.f, 0.f));
		}

	}

	void PlayerScript::HandleMouseInput()
	{
		
		if (Input::GetKeyDown(mKeyBindings[(UINT)playerKeyAction::MOVE_LBTN]))
		{
			ActionMouseLBTN();
		}

		if (Input::GetKeyDown(mKeyBindings[(UINT)playerKeyAction::MOVE_RBTN]))
		{
			ActionMouseRBTN();
		}
	}

	void PlayerScript::HandleOtherInput()
	{
	}

	void PlayerScript::HandleTestInput()
	{
		if (Input::GetKeyDown(eKeyCode::NUM_6))
		{
			Player* player = dynamic_cast<Player*>(GetOwner());
			if (player)
			{
				ePlayerCostume nextCostume = player->GetCurrentCostume();
				static_cast<UINT>(nextCostume) == static_cast<UINT>(ePlayerCostume::SunsetGunman) ? nextCostume = ePlayerCostume::Adventurer : nextCostume = static_cast<ePlayerCostume>(static_cast<UINT>(nextCostume) + 1);
				player->ChangeCostume(nextCostume);
			}
		}
		if (Input::GetKeyDown(eKeyCode::NUM_4))
		{
			Player* player = dynamic_cast<Player*>(GetOwner());
			if (player)
			{
				ePlayerCostume nextCostume = player->GetCurrentCostume();
				static_cast<UINT>(nextCostume) == static_cast<UINT>(ePlayerCostume::Adventurer) ? nextCostume = ePlayerCostume::SunsetGunman : nextCostume = static_cast<ePlayerCostume>(static_cast<UINT>(nextCostume) - 1);
				player->ChangeCostume(nextCostume);
			}
		}

	}

	void PlayerScript::FakeGroundApply()
	{
		if (mOwnerRigid)
		{
			float halfScaleY = GetOwner()->GetScaleY() * 0.5f;
			float testY = GetOwner()->GetPositionY() - halfScaleY;
			if (testY < 0.f && mOwnerRigid->GetVelocity().y <= 0.f)
			{
				GetOwner()->SetPositionY(halfScaleY);
				mOwnerRigid->SetGround(true);
			}
			eMoveDir moveDir = mOwnerRigid->GetMoveDir();
			bool isGround = mOwnerRigid->IsGround();
			ePlayerState state = ePlayerState::End;
			Player* player = dynamic_cast<Player*>(GetOwner());

			if (player)
			{
				if (isGround)
				{
					if (eMoveDir::End == moveDir)
						state = ePlayerState::Idle;
					else if (eMoveDir::Left == moveDir || eMoveDir::Right == moveDir)
						state = ePlayerState::Run;
				}
				else
				{
					state = ePlayerState::Jump;
				}

				player->ChangeState(state);
			}
		}
	}

	void PlayerScript::ActionMouseLBTN()
	{
		Player* player = dynamic_cast<Player*>(GetOwner());

		if (!player)
			return;

		std::vector<Script*> scripts = player->GetCenter()->GetScripts();

		for (auto script : scripts)
		{
			if (dynamic_cast<ArmRotatorScript*>(script))
			{
				ArmRotatorScript* rotatorScript = dynamic_cast<ArmRotatorScript*>(script);
				rotatorScript->InverseArmAxis(Axis::X);
			}
		}
	}

	void PlayerScript::ActionMouseRBTN()
	{
		// Dash 중이지 않은 경우에만 동작시켜준다.
		if (!mbDash)
		{
			mDashStartedTime = Time::AccTime();
			mbDash = true;
		}
	}

	void PlayerScript::Dash()
	{
		if (!mbDash)
			return;

		if (mbDash)
		{
			float curTime = Time::AccTime();

			if (curTime > mDashStartedTime + mMaxDashTime)
			{
				mbDash = false;
				return;
			}
		}
		Vector3 bodyPos = GetOwner()->GetWorldPosition();
		Vector2 mousePos = Input::GetMouseWorldPosition();

		Vector3 dir = Vector3(mousePos.x, mousePos.y, 0.f) - bodyPos;
		dir.Normalize();

		// 대쉬 로직
		Vector2 nextPos = Vector2(dir.x * mDashPower * Time::DeltaTime(), dir.y * mDashPower * Time::DeltaTime());
		//Vector2 nextPos = Vector2(dir.x * mDashPower, dir.y * mDashPower);
		//
		//mOwnerRigid->SetGround(false);
		//Vector2 curVel = mOwnerRigid->GetVelocity();
		//mOwnerRigid->SetVelocity(Vector2(curVel.x, curVel.y + dir.y * mDashPower * 0.1f));
		//mOwnerRigid->AddForce(nextPos);

		GetOwner()->AddPositionXY(nextPos);
		mOwnerRigid->ClearVelocityY();
		mOwnerRigid->SetGround(false);
	}

	void PlayerScript::HandleJumpInput()
	{
		if (!mOwnerRigid)
			return;

		bool bIsGround = mOwnerRigid->IsGround();

		static bool bCanInputSingleJump = true;
		static bool bCanInputDoubleJump = false;
		static bool bSingleJumping = false;
		static bool bDoubleJumping = false;

		static float singleJumpInputedTime = 0.f;

		eKeyState jumpKeyState = Input::GetKeyState(mKeyBindings[(UINT)playerKeyAction::MOVE_JUMP]);

		static int count = 0;
		switch (jumpKeyState)
		{
		// when Jump key State is changed to None to Down.(push)
		case DOWN:
		{
			if (bIsGround && bCanInputSingleJump)
			{
				singleJumpInputedTime = Time::AccTime();
				bCanInputSingleJump = false;
				bSingleJumping = true;
				JumpStart();
			}
			else if (!bIsGround && bCanInputDoubleJump)
			{
				bCanInputDoubleJump = false;
				bDoubleJumping = true;
				DoubleJumpStart();				
			}
		}
		break;
		// When Down Jump button is still Pushed
		case PRESSED:
		{
			if (!bSingleJumping)
				break;

			static float maxJumpInputTime = 0.5f;
			float curAccTime = Time::AccTime();
			if (singleJumpInputedTime + maxJumpInputTime > curAccTime)
			{
				Jumping();
			}
			else
			{
				bSingleJumping = false;
			}
		}
		break;
		// When Pressed Jump button released
		case UP:
		{
			if (bSingleJumping)
			{
				bCanInputSingleJump = bIsGround;
				bCanInputDoubleJump = !bIsGround;
				bSingleJumping = false;
				bDoubleJumping = false;
				singleJumpInputedTime = 0.f;
			}
			else if (!bSingleJumping)
			{
				if (!bDoubleJumping) bCanInputDoubleJump = true;

			}
			else if (bDoubleJumping)
			{
				bDoubleJumping = false;
			}
		}
		break;
		// After Key Up
		case NONE:
			if (bIsGround && !bSingleJumping)
			{
				bDoubleJumping = false;
				bCanInputSingleJump = true;
				bCanInputDoubleJump = false;
				singleJumpInputedTime = 0.f;
			}
			break;
		default:
			break;
		}
	}

	void PlayerScript::JumpStart()
	{
		mOwnerRigid->SetGround(false);

		Vector2 jumpForce = Vector2(0.f, 30000.f);
		mOwnerRigid->AddForce(jumpForce);
		Vector2 curVel = mOwnerRigid->GetVelocity();
		mOwnerRigid->SetVelocity(Vector2(curVel.x, 300.f));
	}

	void PlayerScript::DoubleJumpStart()
	{
		Vector2 jumpForce = Vector2(0.f, 25000.f);
		mOwnerRigid->AddForce(jumpForce);
		Vector2 curVel = mOwnerRigid->GetVelocity();
		mOwnerRigid->SetVelocity(Vector2(curVel.x, 1000.f));
	}

	void PlayerScript::Jumping()
	{
		Vector2 jumpForce = Vector2(0.f, 5000.f);
		mOwnerRigid->AddForce(jumpForce);
	}

	void PlayerScript::ResetJump()
	{
	}

	void PlayerScript::InitialKeyBind()
	{
		// Move Left
		eKeyCode key = eKeyCode::A;

		mKeyBindings.push_back(key);

		// Move Right
		key = eKeyCode::D;

		mKeyBindings.push_back(key);

		// Move Down
		key = eKeyCode::S;

		mKeyBindings.push_back(key);

		// Move Up
		key = eKeyCode::W;

		mKeyBindings.push_back(key);

		// Mouse L Button
		key = eKeyCode::LBTN;

		mKeyBindings.push_back(key);

		// Mouse R Button
		key = eKeyCode::RBTN;

		mKeyBindings.push_back(key);
		
	}

	void PlayerScript::ChangeKeyBind(playerKeyAction targetAction, eKeyCode bindedKey)
	{
		if (mKeyBindings.size() <= static_cast<size_t>(targetAction) || mKeyBindings[static_cast<UINT>(targetAction)] == bindedKey)
			return;

		mKeyBindings[static_cast<UINT>(targetAction)] = bindedKey;
	}

}