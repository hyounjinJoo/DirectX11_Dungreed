#include "hjTitleGameStart.h"
#include "hjResources.h"
#include "hjMaterial.h"
#include "hjSpriteRenderer.h"
#include "hjSceneManager.h"

namespace hj
{
	TitleGameStart::TitleGameStart()
		: UIButton()
	{
		SetName(WIDE("Title Start Button"));
		std::shared_ptr<Material> material = MTRL_FIND("MTRL_Title_UI_Text");
		SetMaterial(material);

		std::shared_ptr<Texture> texture = material->GetTexture();

		mAtlasSize = texture->GetTexSize();

		if (texture)
		{
			//    <sprite n="PlayOff" x="384" y="0" w="211" h="62"/>
			//    <sprite n="PlayOn" x="595" y="0" w="211" h="62"/>

			SetSameSizeAll(Vector2(211.f, 62.f));
			SetUVIdle(Vector2(384.f, 0.f), mIdleSize);
			SetUVHoverPressedDown(Vector2(595.f, 0.f), mHoverSize);

			mStartUV = mButtonUVInfo.idleStartUV;
			mEndUV = mButtonUVInfo.idleEndUV;

			mIdleSize = Vector2(176.f, 52.f);
			mHoverSize = Vector2(176.f, 52.f);
			mPressedSize = Vector2(176.f, 52.f);
			mDownSize = Vector2(176.f, 52.f);
		}
		SetScale(Vector2(176.f, 52.f));
		SetPositionY(-135.f);

		SetClickCallback(this, &TitleGameStart::StartGame);
	}

	TitleGameStart::~TitleGameStart()
	{
	}

	void TitleGameStart::Initialize()
	{
		UIButton::Initialize();
	}

	void TitleGameStart::Update()
	{
		UIButton::Update();
	}

	void TitleGameStart::FixedUpdate()
	{
		UIButton::FixedUpdate();
	}

	void TitleGameStart::Render()
	{
		UIButton::Render();
	}

	void TitleGameStart::StartGame()
	{
		SceneManager::NeedToLoad(eSceneType::Test);
	}
}