#include "hjPlayerScript.h"
#include "hjTransform.h"
#include "hjGameObject.h"
#include "hjInput.h"
#include "hjTime.h"
#include "hjSpriteRenderer.h"
#include "hjBaseRenderer.h"
#include "hjRigidBody.h"
#include "hjApplication.h"
#include "CommonInclude.h"
#include "hjArmRotatorScript.h"
#include "hjObject.h"
#include "hjFxPlayerTrail.h"
#include "hjFxPlayerJump.h"
#include "hjResourceManager.h"
#include "hjCameraScript.h"
#include "hjMonster.h"
#include "hjPlayerDashUI.h"
#include "hjActor.h"
#include "hjPlayerHand.h"
#include "hjAttackScript.h"
#include "hjPlayerLifeBarUI.h"

extern hj::Application application;
namespace hj
{
	PlayerScript::PlayerScript()
		: Script()
		, mRunForce(Vector2(2000.f, 0.f))
		, mbDash(false)
		, mDashStartedTime(0.f)
		, mDashInputCoolTime(0.3f)
		, mMaxDashTime(0.1f)
		, mDashTrailCount(5)
		, mDashDir(Vector3::Zero)
		, mDashPower(3400.f)
		, mLastDashTrailTime(0.f)
		, mCurActivatedTrailIndex(0)
		, mJumpRatio(0.05f)
		, mJumpingRatio(0.025f)
		, mJumpForce(Vector2(0.f, 20000.f))
		, mbCanInputSingleJump(true)
		, mbCanInputDoubleJump(false)
		, mbSingleJumping(false)
		, mbDoubleJumping(false)
		, mSingleJumpInputedTime(0.f)
		, mMaxJumpInputTime(0.2f)
		, mDashUI(nullptr)
		, mMaxDashCount(2)
		, mCurDashCanCount(2)
		, mDashChargeTimer(0.f)
		, mDashChargeTime(1.f)
		, mbActiveInput(true)
	{
		mDashTrailRenderTimer = 0.1f;
		mDashTrailCreateInterval = mMaxDashTime / mDashTrailCount;
		InitialKeyBind();

		CreateDamageWarningObject();
		CreateDashUIObject();
		CreateHPBarUIObject();
	}

	PlayerScript::~PlayerScript()
	{
	}

	void PlayerScript::Initialize()
	{
		if (GetOwner()->GetComponent<RigidBody>())
			mOwnerRigid = GetOwner()->GetComponent<RigidBody>();		

		InitialDashTrail();
	}

	void PlayerScript::Update()
	{
		if (Time::IsStopped())
			return;

		HandleMovementInput();
		HandleJumpInput();
		HandleDownInput();
		HandleMouseInput(); 
		HandleTestInput();

		HandleOtherInput();
		Dash();

		HandleDamageWarningObject();
		HandleDashUIObject();
	}

	void PlayerScript::FixedUpdate()
	{
		if (Time::IsStopped())
			return;

		GroundMove();
	}

	void PlayerScript::Render()
	{
	}

	void PlayerScript::SetDashAttackActor(Actor* actor)
	{
		if (actor)
		{
			mDashAttackColliderActor = actor;
		}
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

	void PlayerScript::CreateDamageWarningObject()
	{
		mDamageWarningObject = object::Instantiate<GameObject>(eLayerType::UI);
		SpriteRenderer* sr = mDamageWarningObject->AddComponent<SpriteRenderer>();
		sr->SetMesh(MESH_FIND_STR("Mesh_Rect"));
		std::shared_ptr<Material> WarningMaterial = MTRL_FIND_STR("MTRL_UI_HUD_RedWarningOnHit");
		sr->SetMaterial(WarningMaterial);


		int useUV = 1;
		Vector2 startUV = Vector2::Zero;
		Vector2 endUV = Vector2::One;

		WarningMaterial->SetData(eGPUParam::Int_1, &useUV);
		WarningMaterial->SetData(eGPUParam::Int_2, &useUV);
		WarningMaterial->SetData(eGPUParam::Int_3, &useUV);
		WarningMaterial->SetData(eGPUParam::Vector2_1, &startUV);

		Vector2 canvasSize = WarningMaterial->GetTexture(eTextureSlot::T0)->GetTexSize();
		WarningMaterial->SetData(eGPUParam::Vector2_2, &canvasSize);
		WarningMaterial->SetData(eGPUParam::Vector2_3, &canvasSize);
		WarningMaterial->SetData(eGPUParam::Vector2_4, &canvasSize);

		Vector4 addedColor = Vector4(1.f, -1.f, -1.f, 0.f);
		WarningMaterial->SetData(eGPUParam::Vector4_1, &addedColor);

		mDamageWarningObject->SetScaleXY(canvasSize);
		mDamageWarningObject->Pause();
	}

	void PlayerScript::HandleDamageWarningObject()
	{
		if (!mDamageWarningObject || mDamageWarningObject->IsPause())
			return;

		if (mActivateDamageWarningTimer < mActivateDamageWarningLimitTime)
		{
			mActivateDamageWarningTimer += Time::ActualDeltaTime();

			return;
		}

		mDamageWarningObject->Pause();
	}

	void PlayerScript::CreateDashUIObject()
	{
		mDashUI = object::Instantiate<PlayerDashUI>(eLayerType::UI);
	}

	void PlayerScript::HandleDashUIObject()
	{
		if (!mDashUI)
		{
			return;
		}

		UINT dashSlotCount = mDashUI->GetMaxDashCount();
		if (mMaxDashCount != dashSlotCount)
		{
			mDashUI->SetMaxDashCount(mMaxDashCount);
		}

		UINT uiCurCanDashCount = mDashUI->GetCurDashCount();
		if (uiCurCanDashCount != dashSlotCount)
		{
			if (mDashChargeTimer > 0.f)
			{
				mDashChargeTimer -= Time::FixedDeltaTime();
			}
			else if (mDashChargeTimer == 0.f)
			{
				mDashChargeTimer = mDashChargeTime;
			}
		}
		
		if (mDashChargeTimer < 0.f)
		{
			mDashChargeTimer = 0.f;
			mCurDashCanCount += 1;
			if (mMaxDashCount < mCurDashCanCount)
			{
				mCurDashCanCount = mMaxDashCount;
			}
			mDashUI->IncreaseCurCount();
		}

	}

	void PlayerScript::CreateHPBarUIObject()
	{
		mLifeBarUI = object::Instantiate<PlayerLifeBarUI>(eLayerType::UI);
	}

	void PlayerScript::UpdateHPBarUI()
	{
		if (!GetOwner() || !mLifeBarUI)
		{
			return;
		}

		Player* ownerPlayer = dynamic_cast<Player*>(GetOwner());

		if (!ownerPlayer)
		{
			return;
		}

		int currentPlayerHP = ownerPlayer->GetCurrentHP();
		int maxPlayerHP = ownerPlayer->GetMaxHP();

		mLifeBarUI->SetMaxHPValue(static_cast<float>(maxPlayerHP));
		mLifeBarUI->SetCurrentHPValue(static_cast<float>(currentPlayerHP));
	}

	void PlayerScript::HandleMovementInput()
	{
		if (!mbActiveInput)
			return;

		if (!mOwnerRigid)
			return;

		if (mbDash)
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
		//if (Input::GetKeyPressed(mKeyBindings[(UINT)playerKeyAction::MOVE_DOWN]))
		//{
		//	GetOwner()->SetPositionY(-300.f * Time::ActualDeltaTime());
		//}
	}

	void PlayerScript::HandleMouseInput()
	{
		if (!mbActiveInput)
			return;

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
				ChangeTrailCostume(nextCostume);
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
				ChangeTrailCostume(nextCostume);
			}
		}

	}

	void PlayerScript::ChangeTrailCostume(ePlayerCostume nextCostume)
	{
		auto iter = mDashTrailObj.begin();
		auto iterEnd = mDashTrailObj.end();

		for (; iter != iterEnd; ++iter)
			(*iter)->ChangeCostume(static_cast<UINT>(nextCostume));
	}

	void PlayerScript::GroundMove()
	{
		if (mOwnerRigid)
		{			
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
		if (mHand)
		{
			mHand->Attack();
		}
	}

	void PlayerScript::ActionMouseRBTN()
	{
		bool isNeedToReturn = false;
		if (0 == mCurDashCanCount)
		{
			isNeedToReturn = true;
		}

		if (Time::AccTime() - mDashStartedTime < mDashInputCoolTime)
		{
			isNeedToReturn = true;
		}

		if (isNeedToReturn)
		{
			return;
		}

		// Dash 중이지 않은 경우에만 동작시켜준다.
		if (!mbDash)
		{
			mDashStartedTime = Time::AccTime();
			mLastDashTrailTime = mDashStartedTime;
			mbDash = true;

			Vector3 bodyPos = GetOwner()->GetWorldPosition();
			Vector2 mousePos = Input::GetMouseWorldPosition();

			mDashDir = Vector3(mousePos.x, mousePos.y, 0.f) - bodyPos;
			mDashDir.Normalize();

			mDashUI->DecreaseCurCount();
			mCurDashCanCount -= 1;
		}
	}

	void PlayerScript::Dash()
	{
		if (!mbDash)
		{
			return;
		}

		float curTime = Time::AccTime();

		if (curTime > mDashStartedTime + mMaxDashTime)
		{
			mbDash = false;
			return;
		}

		bool isNeedToActiveTrail = false;
		if (mLastDashTrailTime == mDashStartedTime)
		{
			isNeedToActiveTrail = true;
			mLastDashTrailTime += mDashTrailCreateInterval;
		}

		if (curTime > mLastDashTrailTime)
		{
			isNeedToActiveTrail = true;
			mLastDashTrailTime += mDashTrailCreateInterval;
		}
		
		if (isNeedToActiveTrail)
		{
			ActiveDashTrail();
		}

		// 대쉬 로직
		float fixedDelta = Time::FixedDeltaTime();
		Vector2 nextPos = Vector2(mDashDir.x * mDashPower * fixedDelta, mDashDir.y * mDashPower * fixedDelta);

		GetOwner()->AddPositionXY(nextPos);
		mOwnerRigid->ClearVelocityY();
		mOwnerRigid->SetVelocity(Vector2(mDashDir.x, mDashDir.y));
		mOwnerRigid->AddForce(Vector2(mDashDir.x, mDashDir.y));
		mOwnerRigid->SetGround(false);
		mOwnerRigid->SetPathThroughFloor(false);
	}

	void PlayerScript::ActiveDashTrail()
	{
		if (1 == mCurActivatedTrailIndex)
		{
			std::vector<Script*> scripts = mDashAttackColliderActor->GetScripts();
			for (Script* script : scripts)
			{
				if (dynamic_cast<AttackScript*>(script))
				{
					dynamic_cast<AttackScript*>(script)->SetDamageOn(true);
				}
			}
		}
		Vector3 bodyPos = GetOwner()->GetWorldPosition();
		mDashTrailObj[mCurActivatedTrailIndex]->SetPositionXY(Vector2(bodyPos.x, bodyPos.y));
		mDashTrailObj[mCurActivatedTrailIndex++]->ActivateTrail(true);
		
		if (mCurActivatedTrailIndex >= mDashTrailCount)
		{
			mCurActivatedTrailIndex = 0;

			std::vector<Script*> scripts = mDashAttackColliderActor->GetScripts();
			for (Script* script : scripts)
			{
				if (dynamic_cast<AttackScript*>(script))
				{
					dynamic_cast<AttackScript*>(script)->ClearDamagedObjects();
				}
			}
		}
	}

	void PlayerScript::HandleJumpInput()
	{
		if (!mbActiveInput)
			return;

		if (!mOwnerRigid)
			return;

		eKeyState firstJumpKeyState = Input::GetKeyState(mKeyBindings[(UINT)playerKeyAction::MOVE_JUMP]);
		eKeyState secondJumpKeyState = Input::GetKeyState(mKeyBindings[(UINT)playerKeyAction::MOVE_SECONDJUMP]);
		
		eKeyState finalJumpKeyState = eKeyState::NONE;
		if (firstJumpKeyState == eKeyState::DOWN || secondJumpKeyState == eKeyState::DOWN)
		{
			finalJumpKeyState = eKeyState::DOWN;
		}
		else if (firstJumpKeyState == eKeyState::PRESSED || secondJumpKeyState == eKeyState::PRESSED)
		{
			finalJumpKeyState = eKeyState::PRESSED;
		}
		else if(firstJumpKeyState == eKeyState::UP || secondJumpKeyState == eKeyState::UP)
		{
			finalJumpKeyState = eKeyState::UP;
		}

		ProcessJumpKey(finalJumpKeyState);
	}

	void PlayerScript::ProcessJumpKey(eKeyState jumpKeyState)
	{
		bool bIsGround = mOwnerRigid->IsGround();

		if (bIsGround)
		{
			bool bIsOnPathThrough = mOwnerRigid->IsOnPathThroughFloor();
			bool bDownKeyClicked = Input::GetKeyDown(mKeyBindings[(UINT)playerKeyAction::MOVE_DOWN]) || Input::GetKeyPressed(mKeyBindings[(UINT)playerKeyAction::MOVE_DOWN]);
			bool bJumpKeyClickAndPress = (jumpKeyState == eKeyState::DOWN || jumpKeyState == eKeyState::PRESSED);

			// 바닥이 pathThrough인 경우에만 동작할 것.
			// Down과 jumpKey가 동시에 눌린 경우
			// 처리하고 탈출
			if (bIsOnPathThrough && bDownKeyClicked && bJumpKeyClickAndPress)
			{
				mOwnerRigid->SetGround(false);
				mOwnerRigid->SetPathThroughFloor(false);
				GetOwner()->AddPositionY(-13.f);
				return;
			}

		}

		switch (jumpKeyState)
		{
		case DOWN:
		{
			if (bIsGround && mbCanInputSingleJump)
			{
				mSingleJumpInputedTime = Time::AccTime();
				mbCanInputSingleJump = false;
				mbSingleJumping = true;
				JumpStart();
			}
			else if (!bIsGround && mbCanInputDoubleJump)
			{
				mbCanInputDoubleJump = false;
				mbDoubleJumping = true;
				DoubleJumpStart();
			}
		}
		break;
		// When Down Jump button is still Pushed
		case PRESSED:
		{
			if (!mbSingleJumping)
				break;

			float curAccTime = Time::AccTime();
			if (mSingleJumpInputedTime + mMaxJumpInputTime > curAccTime)
			{
				Jumping();
			}
			else
			{
				mbSingleJumping = false;
			}
		}
		break;
		// When Pressed Jump button released
		case UP:
		{
			if (mbSingleJumping)
			{
				mbCanInputSingleJump = bIsGround;
				mbCanInputDoubleJump = !bIsGround;
				mbSingleJumping = false;
				mbDoubleJumping = false;
				mSingleJumpInputedTime = 0.f;
			}
			else if (!mbSingleJumping)
			{
				if (!mbDoubleJumping)
					mbCanInputDoubleJump = true;

			}
			else if (mbDoubleJumping)
			{
				mbDoubleJumping = false;
			}
		}
		break;
		// After Key Up
		case NONE:
			if (bIsGround && !mbSingleJumping)
			{
				mbDoubleJumping = false;
				mbCanInputSingleJump = true;
				mbCanInputDoubleJump = false;
				mSingleJumpInputedTime = 0.f;
			}
			break;
		default:
			break;
		}
	}

	void PlayerScript::JumpStart()
	{
		mOwnerRigid->SetGround(false);
		GetOwner()->AddPositionY(1.f);
		mOwnerRigid->AddForce(mJumpForce);
		Vector2 curVel = mOwnerRigid->GetVelocity();
		mOwnerRigid->SetVelocity(Vector2(curVel.x, mJumpForce.y * mJumpingRatio));
		Player* owner = dynamic_cast<Player*>(GetOwner());
		if (owner)
		{
			owner->JumpEffectActive(JumpEffect::Single);
		}
	}

	void PlayerScript::DoubleJumpStart()
	{
		mOwnerRigid->AddForce(mJumpForce);
		Vector2 curVel = mOwnerRigid->GetVelocity();
		mOwnerRigid->SetVelocity(Vector2(curVel.x, mJumpForce.y * mJumpRatio));
		Player* owner = dynamic_cast<Player*>(GetOwner());
		if (owner)
		{
			owner->JumpEffectActive(JumpEffect::Double);
		}
	}

	void PlayerScript::Jumping()
	{
		mOwnerRigid->AddForce(Vector2(mJumpForce.x, mJumpForce.y * mJumpingRatio * 7.f));
	}

	void PlayerScript::ResetJump()
	{
	}

	void PlayerScript::DashTrailTransformFixedUpdate()
	{
		if (1 <= mDashTrailObj.size())
		{
			size_t Size = mDashTrailObj.size();

			for (size_t iter = 0; iter < Size; ++iter)
			{
				if (iter <= mCurActivatedTrailIndex)
				{
					mDashTrailObj[iter]->ActivateTrail(false);
				}
				mDashTrailObj[iter]->GetTransform()->FixedUpdate();
			}
		}
	}

	void PlayerScript::DamageWarningActivate()
	{
		if (!mDamageWarningObject)
			return;

		mActivateDamageWarningTimer = 0.f;
		mDamageWarningObject->Activate();

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

	void PlayerScript::InitialDashTrail()
	{
		if (1 <= mDashTrailObj.size())
		{
			std::vector<FxPlayerTrail*>::iterator iter = mDashTrailObj.begin();
			std::vector<FxPlayerTrail*>::iterator iterEnd = mDashTrailObj.end();
			while (iter != iterEnd)
			{
				iter = mDashTrailObj.erase(iter);
			}
		}

		for (int index = 0; index < mDashTrailCount; ++index)
		{
			FxPlayerTrail* trail = object::Instantiate<FxPlayerTrail>(eLayerType::Player);
			Player* owner = dynamic_cast<Player*>(GetOwner());
			trail->SetOwner(owner);
			trail->SetActivateTimer(mDashTrailRenderTimer);
			mDashTrailObj.push_back(trail);
		}
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

		key = eKeyCode::SPACE;

		mKeyBindings.push_back(key);
		
	}

	void PlayerScript::ChangeKeyBind(playerKeyAction targetAction, eKeyCode bindedKey)
	{
		if (mKeyBindings.size() <= static_cast<size_t>(targetAction) || mKeyBindings[static_cast<UINT>(targetAction)] == bindedKey)
			return;

		mKeyBindings[static_cast<UINT>(targetAction)] = bindedKey;
	}

}