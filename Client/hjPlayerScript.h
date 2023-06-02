#pragma once
#include "hjScript.h"
#include "hjInput.h"
#include "hjPlayer.h"

namespace hj
{
	using namespace math;

	enum class playerKeyAction
	{
		MOVE_LEFT = 0,
		MOVE_RIGHT,
		MOVE_DOWN,
		MOVE_JUMP,
		MOVE_LBTN,
		MOVE_RBTN,
		MOVE_SECONDJUMP,

		END,
	};
	
	class GameObject;

	class PlayerScript : public Script
	{
	public:
		PlayerScript();
		virtual ~PlayerScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	public:
		Vector2 GetOwnerScreenPos();

		void DashTrailTransformFixedUpdate();

		bool IsDash() { return mbDash; }

		void DamageWarningActivate();

	private:
		void CreateDamageWarningObject();
		void HandleDamageWarningObject();

	private:
		void HandleMovementInput();
		void HandleJumpInput();
		void HandleDownInput();
		void HandleMouseInput();
		void HandleOtherInput();


		void InitialKeyBind();
		void ChangeKeyBind(playerKeyAction targetAction, eKeyCode bindedKey);

		void HandleTestInput();
		void ChangeTrailCostume(ePlayerCostume nextCostume);
		void GroundMove();

		void ActionMouseLBTN();
		void ActionMouseRBTN();

		void Dash();
		void InitialDashTrail();
		void ActiveDashTrail();

		void ProcessJumpKey(eKeyState jumpKeyState);
		void JumpStart();
		void DoubleJumpStart();
		void Jumping();
		void ResetJump();

	private:
		Vector2 mRunForce;
		class RigidBody* mOwnerRigid;
		std::vector<eKeyCode> mKeyBindings;

		bool mbDash;
		float mDashStartedTime;
		float mDashCoolTime;
		float mMaxDashTime;
		Vector3 mDashDir;
		float mDashPower;
		int mDashTrailCount;
		int mCurActivatedTrailIndex;
		float mDashTrailCreateInterval;
		float mDashTrailRenderTimer;
		float mLastDashTrailTime;
		std::vector<class FxPlayerTrail*> mDashTrailObj;

		Vector2 mJumpForce;
		float mJumpRatio;
		float mJumpingRatio;

		bool mbCanInputSingleJump;
		bool mbCanInputDoubleJump;
		bool mbSingleJumping;
		bool mbDoubleJumping;
		float mSingleJumpInputedTime; 
		float mMaxJumpInputTime;

		float mActivateDamageWarningTimer = 0.f;
		float mActivateDamageWarningLimitTime = 0.2f;
		GameObject* mDamageWarningObject;
	};
}