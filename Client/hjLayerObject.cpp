#include "hjLayerObject.h"
#include "hjSpriteRenderer.h"
#include "hjResourceManager.h"
#include "hjTime.h"

namespace hj
{
	LayerObject::LayerObject()
		: GameObject()
		, mTotalMove(0.f)
		, mMoveSpeed(0.f)
		, mMoveDir(static_cast<UINT>(eMoveDir::Left))
		, mMove(false)
	{
		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		sr->SetMesh(MESH_FIND_STR("Mesh_Rect"));
	}

	LayerObject::~LayerObject()
	{
	}

	void LayerObject::Initialize()
	{
		GameObject::Initialize();
	}
	
	void LayerObject::Update()
	{
		GameObject::Update();
	}
	
	void LayerObject::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}
	
	void LayerObject::Render()
	{
		SpriteRenderer* sr = GetComponent<SpriteRenderer>();

		if (sr)
		{
			std::shared_ptr<Material> material = sr->GetMaterial();

			if (material)
			{

				if (mMove)
				{
					mTotalMove += Time::FixedDeltaTime() * mMoveSpeed;
					if (mTotalMove >= 1.f)
						mTotalMove = 0.f;

					material->SetData(eGPUParam::Float_1, &mTotalMove);
					int moveDir = static_cast<int>(mMoveDir);
					material->SetData(eGPUParam::Int_1, &moveDir);
				}

				material->SetData(eGPUParam::Vector2_1, &mStartUV);
				material->SetData(eGPUParam::Vector2_2, &mEndUV);
			}
		}

		GameObject::Render();
	}
}