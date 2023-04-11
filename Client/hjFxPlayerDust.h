#pragma once
#include <hjGameObject.h>
#include "hjAnimator.h"

namespace hj
{
	class FxPlayerDust :
		public GameObject
	{
	public:
		FxPlayerDust();
		virtual ~FxPlayerDust();
		
		void Initialize() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() override;

		void SetOwner(class Player* owner);
		void ActivateEffect();
		void ChangeAnimationDuration(float wholePlayTime);

	private:
		class Player* mOwner;
		Animator* mAnimator;
		bool mbActivateEffect;

		Vector2 mOffsetPos;
	};
}