#include "hjTitleCloud.h"
#include "hjSpriteRenderer.h"
#include "hjResources.h"
#include "hjMaterial.h"
#include "hjMesh.h"
#include "hjTransform.h"

namespace hj
{
	TitleCloud::TitleCloud()
		: GameObject()
	{
		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = MTRL_FIND("MTRL_Title_Object_MidCloud");
		std::shared_ptr<Mesh> mesh = MESH_FIND("Mesh_Rect");
		sr->SetMaterial(material);
		sr->SetMesh(mesh);

		Vector2 texSize = material->GetTexture()->GetTexSize();
		Transform* tr = static_cast<Transform*>(mComponents[(UINT)eComponentType::Transform]);
		tr->SetScale(Vector3(texSize.x, texSize.y, 1.f));
	}

	TitleCloud::~TitleCloud()
	{
	}

	void TitleCloud::Initialize()
	{
	}

	void TitleCloud::Update()
	{
	}

	void TitleCloud::FixedUpdate()
	{
	}

	void TitleCloud::Render()
	{
	}
}