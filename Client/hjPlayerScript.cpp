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

extern hj::Application application;
namespace hj
{
	PlayerScript::PlayerScript()
		: Script()
		, mLimitJumpKeyInput(0.1f)
	{
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
		Transform* tr = GetOwner()->GetComponent<Transform>();

		if (Input::GetKeyState(eKeyCode::R) == eKeyState::PRESSED)
		{
			Vector3 rot = tr->GetRotation();
			rot.z -= 10.f * Time::DeltaTime();
			tr->SetRotation(rot);
		}
		
		Vector3 pos = tr->GetPosition();

		if (Input::GetKeyState(eKeyCode::D) == eKeyState::PRESSED)
		{
			//pos.x += 100.f * Time::DeltaTime();
			mOwnerRigid->AddForce(Vector2(2000.f, 0.f));
		}
		if (Input::GetKeyState(eKeyCode::A) == eKeyState::PRESSED)
		{
			mOwnerRigid->AddForce(Vector2(-2000.f, 0.f));
		}



		bool isGround = mOwnerRigid->IsGround();

		Vector2 JumpForce = Vector2(0.f, 25000.f);
		if (isGround && Input::GetKeyDown(eKeyCode::W))
		{
			mOwnerRigid->AddForce(JumpForce);
			mOwnerRigid->SetGround(false);
			mOwnerRigid->SetJumpStartPos(pos.y);
			mJumpStartTime = Time::AccTime();
			mbIsJump = true;
		}
		if (mbIsJump && Input::GetKeyPressed(eKeyCode::W))
		{
			float accTime = Time::AccTime();
			if (mJumpStartTime + mLimitJumpKeyInput >= accTime)
				mOwnerRigid->AddForce(JumpForce);
			else
				mbIsJump = false;
		}
		if (mbIsJump && Input::GetKeyUp(eKeyCode::W))
		{
			mbIsJump = false;
		}
		if (!mbIsJump && !isGround && Input::GetKeyDown(eKeyCode::W))
		{
			mOwnerRigid->ClearVelocityY();
			mOwnerRigid->AddForce(JumpForce * 10.f);
			mOwnerRigid->SetJumpStartPos(pos.y);
			mDoubleJumpStartTime = Time::AccTime();
			mbIsDoubleJump = true;
		}
		if(!mbIsJump && !isGround && mbIsDoubleJump)
		{
			float accTime = Time::AccTime();
			if (mDoubleJumpStartTime + mLimitJumpKeyInput >= accTime)
				mOwnerRigid->AddForce(JumpForce);
			else
				mbIsDoubleJump = false;
		}

		if (Input::GetKeyState(eKeyCode::S) == eKeyState::PRESSED)
		{
			pos.y -= 100.f * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::Q) == eKeyState::PRESSED)
		{
			pos.z += 3.0f * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::E) == eKeyState::PRESSED)
		{
			pos.z -= 3.0f * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::NUM_3) == eKeyState::PRESSED)
		{
			mOwnerRigid->SetGround(true);
			mbIsJump = false;
			mbCanDoubleJump = false;
			mbIsDoubleJump = false;
			mJumpStartTime = 0.f;
		}
		if (Input::GetKeyState(eKeyCode::NUM_6) == eKeyState::DOWN)
		{
			Player* player = dynamic_cast<Player*>(GetOwner());
			if (player)
			{
				ePlayerCostume nextCostume = player->GetCurrentCostume();
				static_cast<UINT>(nextCostume) == static_cast<UINT>(ePlayerCostume::SunsetGunman) ?	nextCostume = ePlayerCostume::Adventurer : nextCostume = static_cast<ePlayerCostume>(static_cast<UINT>(nextCostume) + 1);
				player->ChangeCostume(nextCostume);
			}
		}
		if (Input::GetKeyState(eKeyCode::NUM_4) == eKeyState::DOWN)
		{
			Player* player = dynamic_cast<Player*>(GetOwner());
			if (player)
			{
				ePlayerCostume nextCostume = player->GetCurrentCostume();
				static_cast<UINT>(nextCostume) == static_cast<UINT>(ePlayerCostume::Adventurer) ? nextCostume = ePlayerCostume::SunsetGunman : nextCostume = static_cast<ePlayerCostume>(static_cast<UINT>(nextCostume) - 1);
				player->ChangeCostume(nextCostume);
			}
		}

		tr->SetPosition(pos);
	}

	void PlayerScript::FixedUpdate()
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
			bool isGround= mOwnerRigid->IsGround();
			ePlayerState state = ePlayerState::End;
			Player* player = dynamic_cast<Player*>(GetOwner());
				
			if (player)
			{
				if (isGround)
				{
					if (eMoveDir::End == moveDir)
						state = ePlayerState::Idle;
					else if(eMoveDir::Left == moveDir || eMoveDir::Right == moveDir)
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
		else
		{
			return Vector2(FLT_MAX, FLT_MAX);
		}
	}
}