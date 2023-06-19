#pragma once
#include "hjGameObject.h"
namespace hj
{
	class PlayerHand :
		public GameObject
	{
		enum class handState
		{
			Normal,
			Inverse,
			End
		};

	public:
		PlayerHand();
		virtual ~PlayerHand();

		void Initialize() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() override;

	public:
		void SetHandOwner(class Player* owner);
		void InverseHandPosZ(bool inverse);

		Transform* GetWeaponTR();
		
	public:
		void Attack();

	private:
		void CreateAnimation();

	private:
		class Actor* mWeapon;
		Player* mHandOwner;
		Transform* mHandOwnerTR;
		Transform* mHandTransform;
		handState mHandState;
		class Animator* mAnimator;
		class ArmRotatorScript* mRotatorScript;
	};
}

