#include "hjTitleBird.h"
#include "hjSpriteRenderer.h"
#include "hjResources.h"
#include "hjMaterial.h"
#include "hjTransform.h"
#include "hjApplication.h"
#include "hjTime.h"
#include <random>

extern hj::Application application;
namespace hj
{
	TitleBird::TitleBird()
		: GameObject()
		, mMoveDir(static_cast<UINT>(eMoveDir::Right))
		, mMoveSpeed(100.f)
		, mbMove(false)
		, mPosY(0.f)
	{
		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = MTRL_FIND("MTRL_Title_Object_Bird_0");
		std::shared_ptr<Mesh> mesh = MESH_FIND("Mesh_Rect");
		sr->SetMaterial(material);
		sr->SetMesh(mesh);

		Vector2 texSize = material->GetTexture()->GetTexSize();
		Transform* tr = static_cast<Transform*>(mComponents[(UINT)eComponentType::Transform]);
		tr->SetScale(Vector3(texSize.x, texSize.y, 1.f));

		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);

		float width = static_cast<float>(winRect.right - winRect.left);
		float height = static_cast<float>(winRect.bottom - winRect.top);

		mStartX = -width * 0.5f - texSize.x;
		mEndX = width * 0.5f + texSize.x;

		SetPositionX(mStartX);
	}
	
	TitleBird::~TitleBird()
	{
	}
	
	void TitleBird::Initialize()
	{
		GameObject::Initialize();

		SetPositionY(mPosY);
		mbMove = true;
	}

	void TitleBird::Update()
	{
		GameObject::Update();

		if (mbMove)
		{
			Transform* tr = static_cast<Transform*>(mComponents[(UINT)eComponentType::Transform]);

			if (tr)
			{
				Vector3 position = tr->GetPosition();
				if (mEndX < position.x)
				{
					mbMove = false;
				}
				else
				{
					float deltaTime = Time::DeltaTime();
					switch (static_cast<eMoveDir>(mMoveDir))
					{
					case enums::eMoveDir::Left:
						position.x += mMoveSpeed * deltaTime;
						break;
					case enums::eMoveDir::Top:
						position.y += -mMoveSpeed * deltaTime;
						break;
					case enums::eMoveDir::Right:
						position.x += mMoveSpeed * deltaTime;
						break;
					case enums::eMoveDir::Bottom:
						position.y += -mMoveSpeed * deltaTime;
						break;
					case enums::eMoveDir::End:
					default:
						break;
					}

					tr->SetPosition(position);
				}
			}
		}
	}

	void TitleBird::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void TitleBird::Render()
	{
		GameObject::Render();
	}

	void TitleBird::Reset()
	{
		//RECT winRect;
		//GetClientRect(application.GetHwnd(), &winRect);
		//
		//float width = static_cast<float>(winRect.right - winRect.left);
		//float height = static_cast<float>(winRect.bottom - winRect.top);
		//
		//auto curTime = std::chrono::system_clock::now();
		//auto duration = curTime.time_since_epoch();
		//auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
		//std::mt19937 randomSeed(static_cast<UINT>(millis));
		//
		//std::uniform_real_distribution<float> valueRange(-height + GetScaleY(), height - GetScaleY());
		//SetPositionX(mStartX);
		//SetPositionY(valueRange(randomSeed));
	}
}