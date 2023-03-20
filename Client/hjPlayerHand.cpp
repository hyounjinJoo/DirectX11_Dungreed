#include "hjPlayerHand.h"
#include "hjPlayer.h"

namespace hj
{
	PlayerHand::PlayerHand()
		: GameObject()
		, mHandOwner(nullptr)
		, mHandOwnerTR(nullptr)
		, mHandTransform(nullptr)
		, mHandState(handState::Normal)
	{
	}

	PlayerHand::~PlayerHand()
	{
	}
	
	void PlayerHand::Initialize()
	{
		GameObject::Initialize();
	}
	
	void PlayerHand::Update()
	{
		GameObject::Update();
	}
	
	void PlayerHand::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}
	
	void PlayerHand::Render()
	{
		GameObject::Render();
	}

	void PlayerHand::SetHandOwner(Player* owner)
	{
		Player* player = owner;
		if (!player)
			return;

		mHandOwner = player;
		mHandOwnerTR = player->GetTransform();
	}
}