#include "hjTitleBird.h"
#include "hjSpriteRenderer.h"
#include "hjResources.h"
#include "hjMaterial.h"
#include "hjTransform.h"

namespace hj
{
	TitleBird::TitleBird()
		: GameObject()
	{
		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = MTRL_FIND("MTRL_TitleScene_Bird");
		std::shared_ptr<Mesh> mesh = MESH_FIND("Mesh_Rect");
		sr->SetMaterial(material);
		sr->SetMesh(mesh);

		Vector2 texSize = material->GetTexture()->GetTexSize();
		texSize *= 4.f;
		Transform* tr = static_cast<Transform*>(mComponents[(UINT)eComponentType::Transform]);
		tr->SetScale(Vector3(texSize.x, texSize.y, 1.f));
	}
	
	TitleBird::~TitleBird()
	{
	}
	
	void TitleBird::Initialize()
	{
	}

	void TitleBird::Update()
	{
	}

	void TitleBird::FixedUpdate()
	{
	}

	void TitleBird::Render()
	{
	}

}