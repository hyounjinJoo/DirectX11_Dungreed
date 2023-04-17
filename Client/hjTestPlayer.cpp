#include "hjTestPlayer.h"

namespace hj
{
	TestPlayer::TestPlayer()
		: GameObject()
	{
		SetName(WIDE("Test Player Obj"));
	}
	TestPlayer::~TestPlayer()
	{
	}
	void TestPlayer::Initialize()
	{
		GameObject::Initialize();
	}
	void TestPlayer::Update()
	{
		GameObject::Update();
	}
	void TestPlayer::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}
	void TestPlayer::Render()
	{
		GameObject::Render();
	}
}