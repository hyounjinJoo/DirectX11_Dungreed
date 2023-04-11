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

		END,
	};

	class PlayerScript : public Script
	{
	public:
		PlayerScript();
		virtual ~PlayerScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		Vector2 GetOwnerScreenPos();

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
		void FakeGroundApply();

		void ActionMouseLBTN();
		void ActionMouseRBTN();

		void Dash();
		void InitialDashTrail();
		void ActiveDashTrail();

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
	};
}