#include "hjPlayerScript.h"
#include "hjTransform.h"
#include "hjGameObject.h"
#include "hjInput.h"
#include "hjTime.h"
#include "hjSpriteRenderer.h"
#include "hjBaseRenderer.h"
#include "hjRigidBody.h"
#include "hjPlayer.h"

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
		if (GetOwner()->GetComponent<RigidBody>())
			mOwnerRigid = GetOwner()->GetComponent<RigidBody>();			
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
			//pos.x += 100.f * Time::DeltaTime();
			mOwnerRigid->AddForce(Vector2(1000.f, 0.f));
		}
		if (Input::GetKeyState(eKeyCode::A) == eKeyState::PRESSED)
		{
			mOwnerRigid->AddForce(Vector2(-1000.f, 0.f));
		}
		if (Input::GetKeyState(eKeyCode::W) == eKeyState::PRESSED)
		{
			//mOwnerRigid->AddForce(Vector2(0.f, 500.f));
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
		if (mOwnerRigid)
		{
			if (GetOwner()->GetPositionY() <= 0.f)
			{
				GetOwner()->SetPositionY(0.f);
				mOwnerRigid->SetGround(true);
			}
			float velocityX = mOwnerRigid->GetVelocity().x;
			bool isGround= mOwnerRigid->IsGround();
			ePlayerState state = ePlayerState::End;
			Player* player = dynamic_cast<Player*>(GetOwner());
				
			if (player)
			{
				if (isGround)
				{
					if (velocityX == 0.f)
						state = ePlayerState::Idle;
					else
						state = ePlayerState::Run;
				}
				else
				{
					state = ePlayerState::Jump;
				}

				player->ChangeState(state);
			}
		}
	}

	void PlayerScript::Render()
	{
	}
}