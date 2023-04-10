#pragma once
#include <hjGameObject.h>
#include "hjMaterial.h"

namespace hj
{
	class PlayerTrail :
		public GameObject
	{
	public:
		PlayerTrail();
		virtual ~PlayerTrail();

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
		
		bool		mbActivateTrail;
		float		mActivateTimer;
		float		mActivateTime;


	};
}