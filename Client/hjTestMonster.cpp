#include "hjTestMonster.h"
#include "hjInput.h"
#include "hjTime.h"

namespace hj
{
	TestMonster::TestMonster()
		: GameObject()
	{
	}

	TestMonster::~TestMonster()
	{
	}

	void TestMonster::Initialize()
	{
		GameObject::Initialize();
	}

	void TestMonster::Update()
	{
		GameObject::Update();
	}

	void TestMonster::FixedUpdate()
	{
		GameObject::FixedUpdate();

		if (Input::GetKeyState(eKeyCode::Y) == eKeyState::PRESSED)
		{
			Vector3 rot = GetTransform()->GetRotation();
			rot.z += 10.f * Time::FixedDeltaTime();
			GetTransform()->SetRotation(rot);
		}
	}

	void TestMonster::Render()
	{
		GameObject::Render();
	}

}