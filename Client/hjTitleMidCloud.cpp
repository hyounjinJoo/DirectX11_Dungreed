#include "hjTitleMidCloud.h"
#include "hjSpriteRenderer.h"
#include "hjResources.h"
#include "hjMaterial.h"
#include "hjMesh.h"
#include "hjTransform.h"
#include "hjApplication.h"

extern hj::Application application;

namespace hj
{
	TitleMidCloud::TitleMidCloud()
		: GameObject()
	{
		SetName(WIDE("Title Mid Cloud Obj"));

		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = MTRL_FIND_STR("MTRL_Title_Layer_Cloud");
		std::shared_ptr<Mesh> mesh = MESH_FIND_STR("Mesh_Rect");
		sr->SetMaterial(material);
		sr->SetMesh(mesh);

		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);

		Vector2 texSize = material->GetTexture(eTextureSlot::T0)->GetTexSize();
		Vector3 scale = Vector3(texSize.x, texSize.y * 0.5f, 1.f);
		float height = static_cast<float>(winRect.bottom - winRect.top);
		float ratio = height / texSize.y;
		scale *= ratio;
		SetScale(scale);



	}

	TitleMidCloud::~TitleMidCloud()
	{
	}

	void TitleMidCloud::Initialize()
	{
	}

	void TitleMidCloud::Update()
	{
	}

	void TitleMidCloud::FixedUpdate()
	{
	}

	void TitleMidCloud::Render()
	{
	}
}