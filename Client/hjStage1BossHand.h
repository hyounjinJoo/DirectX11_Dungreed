#pragma once
#include <hjGameObject.h>
namespace hj
{
	class Stage1Boss;

	enum class Boss1HandState
	{
		Idle,
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

		void SetOwner(Stage1Boss* owner);
		void ChangeHandState(Boss1HandState nextState);
		void ChangeHandType(Boss1HandType handType);

		void ShotLaser();
	private:
		void CreateAnimation();

	private:
		Stage1Boss* mOwner;
		Boss1HandType mHandType;
		Boss1HandState mHandState;
		class Boss1Laser* mLaser;
		GameObject* mAttackEffect;

    };
}
