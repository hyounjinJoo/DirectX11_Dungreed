#pragma once
#include <hjGameObject.h>
#include "hjMaterial.h"

namespace hj
{
	class FxPlayerTrail :
		public GameObject
	{
	public:
		FxPlayerTrail();
		virtual ~FxPlayerTrail();

		void Initialize() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() override;

		void SetOwner(class Player* owner);
		void ChangeCostume(UINT costumeNumber);

		void ActivateTrail(bool active) { mbActivateTrail = active; }
		void SetActivateTimer(float time) { mActivateTimer = time; }

	private:
		Player* mOwner;
		class Animator* mAnimator; 
		std::shared_ptr<Material> mMaterial;
		
		int			mbActivateTrail;
		float		mActivateTimer;
		float		mActivateTime;
	};
}