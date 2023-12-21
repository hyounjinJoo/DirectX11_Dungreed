#include "hjUIProgressBar.h"
#include "hjResourceManager.h"
#include "hjSpriteRenderer.h"
#include "hjAnimator.h"

namespace hj
{
	UIProgressBar::UIProgressBar()
		: UIBase()
		, mType(progressBarType::RightToLeft)
		, mMaxPercentValue(1.f)
		, mCurrentPercentValue(0.5f)
		, mbIsNeedToUpdate(false)
	{
		SetName(WIDE("UIBaseProgressBar"));

		mMaterial = MTRL_FIND_STR("MTRL_UI_HUD_PlayerHUD_USE_UIShader");
		std::shared_ptr<Mesh> mesh = MESH_FIND_STR("Mesh_Rect");

		if (!mUIRenderer || !mMaterial || !mesh)
			return;

		mUIRenderer->SetMaterial(mMaterial);

		CreateAnimation();

		mAtlasSize = mMaterial->GetTexture(eTextureSlot::T0)->GetTexSize();
		UIBase::mCanvasSize = Actor::mCanvasSize;

		Vector3 scale = Vector3(Actor::mCanvasSize, 1.f);
		SetScale(scale);

		mbUseUV = true;
		mbUseCanvas = false;
		mMaxPercentUV = mEndUV;
		mCurrentPercentUV = mMaxPercentUV;
	}

	UIProgressBar::~UIProgressBar()
	{
	}

	void UIProgressBar::Initialize()
	{
		UIBase::Initialize();
	}

	void UIProgressBar::Update()
	{
		UIBase::Update();
	}

	void UIProgressBar::FixedUpdate()
	{
		if (mbIsNeedToUpdate)
		{
			mbIsNeedToUpdate = false;

			switch (mType)
			{
			case hj::progressBarType::RightToLeft:
			{
				mCurrentPercentUV.x = mMaxPercentUV.x * (mCurrentPercentValue / mMaxPercentValue);
				mCurrentPercentUV.y = mMaxPercentUV.y;
			}
			break;
			case hj::progressBarType::DownToUp:
				break;
			case hj::progressBarType::End:
				break;
			default:
				break;
			}

			mEndUV = mCurrentPercentUV;
		}

		UIBase::FixedUpdate();
	}

	void UIProgressBar::Render()
	{
		UIBase::Render();
	}

	void UIProgressBar::SetScale(const Vector3& scale)
	{
		Actor::SetScale(scale);
		//if (mCanvasSize.x != 0.f || mCanvasSize.y != 0.f)
		//{
		//	Vector2 scaleXY = Vector2(scale.x, scale.y);
		//
		//	UIBase::mCanvasSize = mAtlasSize / (scaleXY / Actor::mCanvasSize);
		//}
	}

	void UIProgressBar::SetScaleXY(const Vector2& scaleXY)
	{
		//Actor::SetScaleXY(scaleXY);
		//if (mCanvasSize.x != 0.f || mCanvasSize.y != 0.f)
		//{
		//	UIBase::mCanvasSize = mAtlasSize / (scaleXY / Actor::mCanvasSize);
		//}
	}

	void UIProgressBar::SetScaleX(const float scaleX)
	{
		Actor::SetScaleX(scaleX);
		//if (mCanvasSize.x != 0.f)
		//{
		//	UIBase::mCanvasSize.x = mAtlasSize.x / (scaleX / Actor::mCanvasSize.x);
		//}
	}

	void UIProgressBar::SetScaleY(const float scaleY)
	{
		Actor::SetScaleY(scaleY);
		//if (mCanvasSize.y != 0.f)
		//{
		//	UIBase::mCanvasSize.y = mAtlasSize.y / (scaleY / Actor::mCanvasSize.y);
		//}
	}

	void UIProgressBar::CreateAnimation()
	{
		CheckHasMaterialAndTexture(eTextureSlot::T0);

		Animator* animator = AddComponent<Animator>();

		std::wstring path = WIDE("03_UI/01_HUD/PlayerHUD.xml");
		std::wstring searchWstr;
		std::wstring animWstr;
		searchWstr = WIDE("LifeBar_LifeWave/LifeBar");
		animWstr = WIDE("LifeBar");

		float frameDuration = 1.f;
		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration);
	}

}