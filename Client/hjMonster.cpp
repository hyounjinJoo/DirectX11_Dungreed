#include "hjMonster.h"

namespace hj
{
	Monster::Monster()
		: mDamagedEffectTimer(0.1f)
	{
	}

	Monster::~Monster()
	{
	}

	void Monster::Initialize()
	{
		GameObject::Initialize();
	}

	void Monster::Update()
	{
		GameObject::Update();
	}

	void Monster::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void Monster::Render()
	{
		GameObject::Render();
	}
}