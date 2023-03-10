#include "hjCameraScript.h"
#include "hjTransform.h"
#include "hjGameObject.h"
#include "hjInput.h"
#include "hjTime.h"

namespace hj
{
	CameraScript::CameraScript()
		: Script()
	{
	}

	CameraScript::~CameraScript()
	{
	}

	void CameraScript::Initialize()
	{
	}

	void CameraScript::Update()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();

		Vector3 pos = tr->GetPosition();

		if (Input::GetKeyState(eKeyCode::D) == eKeyState::PRESSED)
		{
			pos += 3000.f * tr->Right() * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::A) == eKeyState::PRESSED)
		{
			pos += 3000.f * -tr->Right() * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::W) == eKeyState::PRESSED)
		{
			pos += 3000.f * tr->Forward() * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::S) == eKeyState::PRESSED)
		{
			pos += 3000.f * -tr->Forward() * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::Q) == eKeyState::PRESSED)
		{
			pos += 3000.f * tr->Up() * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::E) == eKeyState::PRESSED)
		{
			pos += 3000.f * -tr->Up() * Time::DeltaTime();
		}

		tr->SetPosition(pos);
	}

	void CameraScript::Render()
	{
	}
}