#include "hjTitleOption.h"
#include "hjResources.h"
#include "hjMaterial.h"
#include "hjSpriteRenderer.h"

namespace hj
{
	TitleOption::TitleOption()
		: UIButton()
	{
		SetName(WIDE("Title Option Button"));
		std::shared_ptr<Material> material = MTRL_FIND("MTRL_Title_UI_Text");
		SetMaterial(material);

		std::shared_ptr<Texture> texture = material->GetTexture();

		mAtlasSize = texture->GetTexSize();

		if (texture)
		{
			//<sprite n="OptionOff" x="202" y="0" w="91" h="62"/>
			//<sprite n="OptionOn" x="293" y="0" w="91" h="62"/>

			SetSameSizeAll(Vector2(91.f, 62.f));
			SetUVIdle(Vector2(202.f, 0.f), mIdleSize);
			SetUVHoverPressedDown(Vector2(293.f, 0.f), mHoverSize);

			mStartUV = mButtonUVInfo.idleStartUV;
			mEndUV = mButtonUVInfo.idleEndUV;

			mIdleSize = Vector2(76.f, 52.f);
			mHoverSize = Vector2(76.f, 52.f);
			mPressedSize = Vector2(76.f, 52.f);
			mDownSize = Vector2(76.f, 52.f);
		}
		SetScale(Vector2(76.f, 52.f));
		SetPositionY(-195.f);
	}

	TitleOption::~TitleOption()
	{
	}

	void TitleOption::Initialize()
	{
		UIButton::Initialize();
	}

	void TitleOption::Update()
	{
		UIButton::Update();
	}

	void TitleOption::FixedUpdate()
	{
		UIButton::FixedUpdate();
	}

	void TitleOption::Render()
	{
		UIButton::Render();
	}
}