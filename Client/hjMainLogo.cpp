#include "hjMainLogo.h"
#include "hjSpriteRenderer.h"
#include "hjResources.h"
#include "hjCollider2D.h"

namespace hj
{
	MainLogo::MainLogo()
		: UIBase()
	{
		std::shared_ptr<Material> material = MTRL_FIND("MTRL_Title_UI");
		mUIRenderer->SetMaterial(material);

		//<sprite n = "Logo/MainLogo" x = "180" y = "116" w = "624" h = "300" pX = "0.5" pY = "1" / >
		std::shared_ptr<Texture> texture = material->GetTexture();

		mCanvasSize = Vector2(624.f, 300.f);
		mAtlasSize = texture->GetTexSize();

		if (texture)
		{
			Vector2 startPos = Vector2(180.f, 116.f);
			Vector2 endPos = Vector2(startPos.x + mCanvasSize.x, startPos.y + mCanvasSize.y);
			mStartUV = startPos;
			mEndUV = endPos;
		}
		Vector3 scale = Vector3(mCanvasSize, 1.f);
		scale *= 1.25f;
		SetScale(scale);
		SetPositionY(133.f);
	}
	
	MainLogo::~MainLogo()
	{
	}

	void MainLogo::Initialize()
	{
		UIBase::Initialize();
	}

	void MainLogo::Update()
	{
		UIBase::Update();
	}

	void MainLogo::FixedUpdate()
	{
		UIBase::FixedUpdate();
	}

	void MainLogo::Render()
	{
		UIBase::Render();
	}

}