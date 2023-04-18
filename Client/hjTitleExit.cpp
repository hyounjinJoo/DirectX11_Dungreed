#include "hjTitleExit.h"
#include "hjResources.h"
#include "hjMaterial.h"
#include "hjSpriteRenderer.h"
#include "hjApplication.h"

extern hj::Application application;

namespace hj
{
	TitleExit::TitleExit()
		: UIButton()
	{
		SetName(WIDE("Title Exit Button"));
		std::shared_ptr<Material> material = MTRL_FIND("MTRL_Title_UI_Text");
		SetMaterial(material);

		std::shared_ptr<Texture> texture = material->GetTexture();

		mAtlasSize = texture->GetTexSize();

		if (texture)
		{
			//<sprite n = "ExitOff" x = "0" y = "0" w = "101" h = "62" / >
			//<sprite n = "ExitOn" x = "101" y = "0" w = "101" h = "62" / >

			SetSameSizeAll(Vector2(101.f, 62.f));
			SetUVIdle(Vector2(0.f, 0.f), mIdleSize);
			SetUVHoverPressedDown(Vector2(101.f, 0.f), mHoverSize);

			mStartUV = mButtonUVInfo.idleStartUV;
			mEndUV = mButtonUVInfo.idleEndUV;

			mIdleSize = Vector2(76.f, 52.f);
			mHoverSize = Vector2(76.f, 52.f);
			mPressedSize = Vector2(76.f, 52.f);
			mDownSize = Vector2(76.f, 52.f);
		}
		SetScale(Vector2(84.f, 52.f));
		SetPositionY(-255.f);

		SetClickCallback(this, &TitleExit::ExitGame);
	}

	TitleExit::~TitleExit()
	{
	}

	void TitleExit::Initialize()
	{
		UIButton::Initialize();
	}

	void TitleExit::Update()
	{
		UIButton::Update();
	}

	void TitleExit::FixedUpdate()
	{
		UIButton::FixedUpdate();
	}

	void TitleExit::Render()
	{
		UIButton::Render();
	}

	void TitleExit::ExitGame()
	{
		HWND hWnd = application.GetHwnd();
		if (hWnd)
			DestroyWindow(hWnd);
	}
}