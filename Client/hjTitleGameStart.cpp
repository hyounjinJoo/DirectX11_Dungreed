#include "hjTitleGameStart.h"
#include "hjResources.h"
#include "hjMaterial.h"
#include "hjSpriteRenderer.h"
#include "hjSceneManager.h"
#include "hjXmlParser.h"
#include "hjApplication.h"

extern hj::Application application;
namespace hj
{
	TitleGameStart::TitleGameStart()
		: UIButton()
	{
		SetName(WIDE("Title Start Button"));
		std::shared_ptr<Material> material = MTRL_FIND_STR("MTRL_Title_UI_Text");
		SetMaterial(material);

		std::shared_ptr<Texture> texture = material->GetTexture(eTextureSlot::T0);

		mAtlasSize = texture->GetTexSize();

		if (texture)
		{
			XmlParser* parser = new XmlParser;
			std::wstring path = WIDE("01_Scene/00_TitleScene/TitleScene_03.xml");
			bool parseResult = parser->LoadFile(path);

			if (!parseResult)
			{
				delete parser;
				return;
			}

			Vector2 size = Vector2::Zero;
			Vector2 idleStartPos = Vector2::Zero;
			Vector2 hoverStartPos = Vector2::Zero;
			if (parseResult)
			{
				parseResult = parser->FindElem(WIDE("TextureAtlas"));
				parseResult = parser->IntoElem();

				std::wstring nameWstr;
				while (parseResult)
				{
					parseResult = parser->FindElem(WIDE("sprite"));
					nameWstr = parser->GetWstringAttribute(WIDE("n"));
					if (nameWstr == WIDE("PlayOff"))
					{
						idleStartPos.x = static_cast<float>(parser->GetIntAttribute(WIDE("x")));
						idleStartPos.y = static_cast<float>(parser->GetIntAttribute(WIDE("y"))); 
						size.x = static_cast<float>(parser->GetIntAttribute(WIDE("w")));
						size.y = static_cast<float>(parser->GetIntAttribute(WIDE("h")));
					}
					else if (nameWstr == WIDE("PlayOn"))
					{
						hoverStartPos.x = static_cast<float>(parser->GetIntAttribute(WIDE("x")));
						hoverStartPos.y = static_cast<float>(parser->GetIntAttribute(WIDE("y")));
					}
				}
			}

			SetSameSizeAll(size);
			SetUVIdle(idleStartPos, size);
			SetUVHoverPressedDown(hoverStartPos, size);

			mStartUV = mButtonUVInfo.idleStartUV;
			mEndUV = mButtonUVInfo.idleEndUV;

			float width = static_cast<float>(application.GetWidth());
			size *= (width / 1920.f);
			size.x = std::roundf(size.x);
			size.y = std::roundf(size.y);
			
			mIdleSize = size;
			mHoverSize = size;
			mPressedSize = size;
			mDownSize = size;

			SetScale(Vector2(176.f, 52.f));
			delete parser;
		}

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