#include "hjTitleBird.h"
#include "hjSpriteRenderer.h"
#include "hjResources.h"
#include "hjMaterial.h"
#include "hjTransform.h"
#include "hjApplication.h"
#include "hjTime.h"

extern hj::Application application;
namespace hj
{
	TitleBird::TitleBird()
		: GameObject()
	{
		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = MTRL_FIND("MTRL_Title_Object_Bird_0");
		std::shared_ptr<Mesh> mesh = MESH_FIND("Mesh_Rect");
		sr->SetMaterial(material);
		sr->SetMesh(mesh);

		Vector2 texSize = material->GetTexture()->GetTexSize();
		Transform* tr = static_cast<Transform*>(mComponents[(UINT)eComponentType::Transform]);
		tr->SetScale(Vector3(texSize.x, texSize.y, 1.f));
	}
	
	TitleBird::~TitleBird()
	{
	}
	
	void TitleBird::Initialize()
	{
		GameObject::Initialize();
	}

	void TitleBird::Update()
	{
		GameObject::Update();
	}

	void TitleBird::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void TitleBird::Render()
	{
		GameObject::Render();
	}
}