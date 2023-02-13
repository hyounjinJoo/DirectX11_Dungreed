#include "hjPlayerScript.h"
#include "hjTransform.h"
#include "hjGameObject.h"
#include "hjInput.h"
#include "hjTime.h"

namespace hj
{
	PlayerScript::PlayerScript()
		: Script()
	{
	}

	PlayerScript::~PlayerScript()
	{
	}

	void PlayerScript::Initialize()
	{
	}

	void PlayerScript::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();

		Vector3 pos = tr->GetPosition();

		if (Input::GetKeyState(eKeyCode::D) == eKeyState::PRESSED)
		{
			pos.x += 3.f * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::A) == eKeyState::PRESSED)
		{
			pos.x -= 3.f * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::W) == eKeyState::PRESSED)
		{
			pos.y += 3.f * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::S) == eKeyState::PRESSED)
		{
			pos.y -= 3.f * Time::DeltaTime();
		}

		tr->SetPosition(pos);
	}

	void PlayerScript::FixedUpdate()
	{
	}

	void PlayerScript::Render()
	{
	}
}