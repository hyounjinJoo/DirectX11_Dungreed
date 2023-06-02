#pragma once
#include <hjGameObject.h>
namespace hj
{
	class Stage1Boss;

	enum class Boss1HandState
	{
		Idle,
		MoveToAttack,
		MoveToIdle,
		Attack,
		End,
	};

	enum class Boss1HandType
	{
		Left,
		Right,
		End,
	};

    class Stage1BossHand :
        public GameObject
    {
	public:
		Stage1BossHand();
		virtual ~Stage1BossHand();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	public:
		void SetOwner(Stage1Boss* owner);
		void ChangeHandState(Boss1HandState nextState);
		void ChangeHandType(Boss1HandType handType);
		Boss1HandState GetHandState() { return mHandState; }
		void SetInitialHandPosY(float posY) { mInitialHandPosY = posY; }
		void SetMoveTargetHandPosY(float posY) { mTargetMoveHandPosY = posY; }

	public:
		void AttackStart(float PosY);
		void ShotLaser();
		void EndAttack(bool bNeedToMoveIdlePos);
		bool IsAttackEnd();
		bool IsAttackStartEnd() { return mbAttackStart; }

	private:
		void CreateAnimation();
		void MoveHand();

	private:
		Stage1Boss* mOwner;
		Boss1HandType mHandType;
		Boss1HandState mHandState;
		class Boss1Laser* mLaser;
		GameObject* mAttackEffect;
		bool mbAttackStart;

		float mMoveTimer = 0.f;
		float mMoveLimitTime = 0.5f;

		float mInitialHandPosY;
		float mTargetMoveHandPosY;
		Vector2 mCanMoveRangeY = Vector2(-590.f, 600.f);
		bool mbCanMove;


    };
}
