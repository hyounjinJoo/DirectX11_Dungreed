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

		void SetHandOwner(Player* owner);
	private:
		Player* mHandOwner;
		Transform* mHandOwnerTR;
		Transform* mHandTransform;
		handState mHandState;
	};
}
