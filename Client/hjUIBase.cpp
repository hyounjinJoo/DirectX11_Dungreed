#include "hjUIBase.h"
#include "hjSpriteRenderer.h"
#include "hjResourceManager.h"
#include "hjMesh.h"

namespace hj
{
	UIBase::UIBase()
		: Actor()
		, mbUseUV(true)
		, mbUseCanvas(true)
		, mStartUV(Vector2::Zero)
		, mEndUV(Vector2::One)
		, mCanvasSize(Vector2::One)
		, mAtlasSize(Vector2::One)
	{
		SetName(WIDE("UI Base Obj"));

		mUIRenderer = AddComponent<SpriteRenderer>();
		mMaterial = MTRL_FIND_STR("MTRL_Title_UI");
		std::shared_ptr<Mesh> mesh = MESH_FIND_STR("Mesh_Rect");

		if (!mUIRenderer || !mMaterial || !mesh)
			return;

		mUIRenderer->SetMaterial(mMaterial);
		mUIRenderer->SetMesh(mesh);

		std::shared_ptr<Texture> texture = mMaterial->GetTexture(eTextureSlot::T0);

		if (!texture)
			return;

		mAtlasSize = texture->GetTexSize();
		mCanvasSize = mAtlasSize;

		SetPositionZ(-1.f);
		Vector3 scale = Vector3(mCanvasSize, 1.f);
		SetScale(scale);
	}

	UIBase::~UIBase()
	{
	}
	
	void UIBase::Initialize()
	{
		GameObject::Initialize();
	}
	
	void UIBase::Update()
	{
		GameObject::Update();
	}
	
	void UIBase::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}
	
	void UIBase::Render()
	{
		if (mMaterial)
		{
			int inputInt1 = mbUseUV ? 1 : 0;
			mMaterial->SetData(eGPUParam::Int_1, &inputInt1);
			mMaterial->SetData(eGPUParam::Vector2_1, &mStartUV);
			mMaterial->SetData(eGPUParam::Vector2_2, &mEndUV);
			int inputInt2 = mbUseCanvas ? 1 : 0;
			mMaterial->SetData(eGPUParam::Int_2, &inputInt2);
			mMaterial->SetData(eGPUParam::Vector2_3, &mCanvasSize);
			mMaterial->SetData(eGPUParam::Vector2_4, &mAtlasSize);
		}
		GameObject::Render();
		//if (mMaterial)
		//{
		//	mMaterial->Clear();
		//}
	}
	
	void UIBase::SetRenderer(SpriteRenderer* renderer)
	{
		if (!renderer)
			return;

		SpriteRenderer* curRenderer = GetComponent<SpriteRenderer>();
		if (curRenderer)
		{
			mComponents[static_cast<UINT>(eComponentType::MeshRenderer)] = nullptr;
			delete curRenderer;
			AddComponent(renderer);
			mUIRenderer = renderer;
		}
	}
	
	void UIBase::SetMaterial(std::shared_ptr<Material> material)
	{
		if (!material)
			return;

		SpriteRenderer* curRenderer = GetComponent<SpriteRenderer>();
		if (curRenderer)
		{
			curRenderer->SetMaterial(material);
			mMaterial = material;
		}
	}
}