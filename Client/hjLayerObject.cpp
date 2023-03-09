#include "hjLayerObject.h"
#include "hjSpriteRenderer.h"
#include "hjResources.h"
#include "hjTime.h"

namespace hj
{
	LayerObject::LayerObject()
		: GameObject()
		, mTotalMove(0.f)
		, mMoveSpeed(0.f)
		, mMoveDir(static_cast<UINT>(MoveDir::Left))
		, mMove(false)
	{
		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		sr->SetMesh(MESH_FIND("Mesh_Rect"));
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

		if (false == mMove)
			return;

		SpriteRenderer* sr = GetComponent<SpriteRenderer>();

		if (sr)
		{
			std::shared_ptr<Material> material = sr->GetMaterial();

			if (material)
			{
				mTotalMove += Time::DeltaTime() * mMoveSpeed;
				if (mTotalMove >= 1.f)
					mTotalMove = 0.f;

				material->SetData(eGPUParam::Float_1, &mTotalMove);
				int moveDir = static_cast<int>(mMoveDir);
				material->SetData(eGPUParam::Int_1, &moveDir);
			}
		}
	}
	
	void LayerObject::Render()
	{
		GameObject::Render();
	}
}