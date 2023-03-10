#include "hjPlayerScript.h"
#include "hjTransform.h"
#include "hjGameObject.h"
#include "hjInput.h"
#include "hjTime.h"
#include "hjSpriteRenderer.h"
#include "hjBaseRenderer.h"

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

		if (Input::GetKeyState(eKeyCode::R) == eKeyState::PRESSED)
		{
			Vector3 rot = tr->GetRotation();
			rot.z += 10.f * Time::DeltaTime();
			tr->SetRotation(rot);
		}
		
		Vector3 pos = tr->GetPosition();

		if (Input::GetKeyState(eKeyCode::D) == eKeyState::PRESSED)
		{
			pos.x += 100.f * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::A) == eKeyState::PRESSED)
		{
			pos.x -= 100.f * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::W) == eKeyState::PRESSED)
		{
			pos.y += 100.f * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::S) == eKeyState::PRESSED)
		{
			pos.y -= 100.f * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::Q) == eKeyState::PRESSED)
		{
			pos.z += 3.0f * Time::DeltaTime();
		}
		if (Input::GetKeyState(eKeyCode::E) == eKeyState::PRESSED)
		{
			pos.z -= 3.0f * Time::DeltaTime();
		}

		tr->SetPosition(pos);


		BaseRenderer* baseRenderer = GetOwner()->GetComponent<BaseRenderer>();
		if (baseRenderer)
		{
			std::shared_ptr<Material> mtrl = baseRenderer->GetMaterial();
			if (mtrl)
			{

#define INVERSE -1
#define NORMAL 1

				Vector2 mousePos = Input::GetMousePosition();
				int isInverse = mousePos.x < pos.x ? INVERSE : NORMAL;

				mtrl->SetData(eGPUParam::Int_1, &isInverse);
			}
		}
	}

	void PlayerScript::FixedUpdate()
	{
	}

	void PlayerScript::Render()
	{
	}
}