#pragma once
#include <hjGameObject.h>

namespace hj
{
	enum class JumpEffect
	{
		Single,
		Double,
		End,
	};


	class FxPlayerJump :
		public GameObject
	{
	public:
		FxPlayerJump();
		virtual ~FxPlayerJump();

		void Initialize() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() override;

		void SetOwner(class Player* owner);
		void ActivateEffect(JumpEffect jumpEffect);

	private:
		class Player* mOwner;
		class Animator* mAnimator;
		JumpEffect mJumpEffect;
		bool mbActivateEffect;

		Vector2 mOffsetPos;
	};
}