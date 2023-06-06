#include "hjMainLogo.h"
#include "hjSpriteRenderer.h"
#include "hjResources.h"
#include "hjCollider2D.h"
#include "hjXmlParser.h"

namespace hj
{
	MainLogo::MainLogo()
		: UIBase()
	{
		std::shared_ptr<Material> material = MTRL_FIND_STR("MTRL_Title_UI");
		mUIRenderer->SetMaterial(material);

		std::shared_ptr<Texture> texture = material->GetTexture(eTextureSlot::T0);
		
		if (texture)
		{
			XmlParser* parser = new XmlParser;
			std::wstring path = WIDE("01_Scene/00_TitleScene/TitleScene_01.xml");
			bool parseResult = parser->LoadFile(path);

			if (!parseResult)
			{
				delete parser;
				return;
			}

			parseResult = parser->FindElem(WIDE("TextureAtlas"));
			parseResult = parser->IntoElem();

			std::wstring nameWstr;
			bool findResult = false;
			while (!findResult)
			{
				parseResult = parser->FindElem(WIDE("sprite"));
				nameWstr = parser->GetWstringAttribute(WIDE("n"));
				if (0 == nameWstr.compare(WIDE("Logo/MainLogo")))
					findResult = true;
			}

			if (findResult)
			{
				mCanvasSize.x = static_cast<float>(parser->GetIntAttribute(WIDE("w")));
				mCanvasSize.y = static_cast<float>(parser->GetIntAttribute(WIDE("h")));

				
					mAtlasSize = texture->GetTexSize();
					Vector2 startPos = Vector2::Zero;
					startPos.x = static_cast<float>(parser->GetIntAttribute(WIDE("x")));
					startPos.y = static_cast<float>(parser->GetIntAttribute(WIDE("y")));
					Vector2 endPos = Vector2(startPos.x + mCanvasSize.x, startPos.y + mCanvasSize.y);
					mStartUV = startPos;
					mEndUV = endPos;
			}			
			delete parser;
		}

		
		Vector3 scale = Vector3(mCanvasSize, 1.f);
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