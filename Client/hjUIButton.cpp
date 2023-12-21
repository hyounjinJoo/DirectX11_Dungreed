#include "hjUIButton.h"
#include "hjCollider2D.h"
#include "hjInput.h"
#include "hjResourceManager.h"

namespace hj
{
	UIButton::UIButton()
		: UIBase()
		, mUIButtonCollider(nullptr)
		, mIdleSize(Vector2::One)
		, mHoverSize(Vector2::One)
		, mPressedSize(Vector2::One)
		, mDownSize(Vector2::One)
		, mState(eUIButtonState::IDLE)
		, mButtonUVInfo{}
	{
		mUIButtonCollider = AddComponent<Collider2D>();
		mUIButtonCollider->UseMouseCollision(true);
	}

	UIButton::~UIButton()
	{
	}
	
	void UIButton::Initialize()
	{
		UIBase::Initialize();
	}
	
	void UIButton::Update()
	{
		UIBase::Update();
	}
	
	void UIButton::FixedUpdate()
	{
		UpdateState();
		ChangeUV();
		UpdateScale();

		UIBase::FixedUpdate();

		if (mState == eUIButtonState::DOWN)
		{
			if (mClickCallback)
				mClickCallback();
		}
	}
	
	void UIButton::Render()
	{
		UIBase::Render();
	}

	void UIButton::UpdateState()
	{
		eCollisionState collisionState = eCollisionState::End;

		if (mUIButtonCollider)
		{
			collisionState = mUIButtonCollider->GetMouseState();
			if (collisionState == eCollisionState::CollisionStay)
			{
				eKeyState mouseLBState = Input::GetKeyState(eKeyCode::LBTN);
				switch (mouseLBState)
				{
				case DOWN:
					mState = eUIButtonState::DOWN;
					break;
				case PRESSED:
					mState = eUIButtonState::PRESSED;
					break;
				case UP:
				case NONE:
				default:
					mState = eUIButtonState::HOVER;
					break;
				}
			}
			else if (collisionState == eCollisionState::CollisionNot)
			{
				mState = eUIButtonState::IDLE;
			}
		}
	}

	void UIButton::UpdateScale()
	{
		Vector2 nextScale = GetScaleXY();
		switch (mState)
		{
		case eUIButtonState::IDLE:
		case eUIButtonState::End:
		default:
			nextScale = mIdleSize;
			break;
		case eUIButtonState::HOVER:
			nextScale = mHoverSize;
			break;
		case eUIButtonState::PRESSED:
			nextScale = mPressedSize;
			break;
		case eUIButtonState::DOWN:
			nextScale = mDownSize;
			break;
		}
		SetScaleXY(nextScale);
	}

	void UIButton::ChangeUV()
	{
		switch (mState)
		{
		case eUIButtonState::IDLE:
		case eUIButtonState::End:
		default:
			mStartUV = mButtonUVInfo.idleStartUV;
			mEndUV = mButtonUVInfo.idleEndUV;
			break;
		case eUIButtonState::HOVER:
			mStartUV = mButtonUVInfo.hoverStartUV;
			mEndUV = mButtonUVInfo.hoverEndUV;
			break;
		case eUIButtonState::PRESSED:
			mStartUV = mButtonUVInfo.pressedStartUV;
			mEndUV = mButtonUVInfo.pressedEndUV;
			break;
		case eUIButtonState::DOWN:
			mStartUV = mButtonUVInfo.downStartUV;
			mEndUV = mButtonUVInfo.downEndUV;
			break;
		}
	}
	void UIButton::SetSameSizeAll(const Vector2& size)
	{
		mDownSize = mPressedSize = mHoverSize = mIdleSize = mCanvasSize = size;
	}

	void UIButton::SetUVIdle(const Vector2& startUV, const Vector2& size)
	{
		mButtonUVInfo.idleStartUV = startUV;
		mButtonUVInfo.idleEndUV = startUV + size;
	}

	void UIButton::SetUVHover(const Vector2& startUV, const Vector2& size)
	{
		mButtonUVInfo.hoverStartUV = startUV;
		mButtonUVInfo.hoverEndUV = startUV + size;
	}

	void UIButton::SetUVPressed(const Vector2& startUV, const Vector2& size)
	{
		mButtonUVInfo.pressedStartUV = startUV;
		mButtonUVInfo.pressedEndUV = startUV + size;
	}

	void UIButton::SetUVDown(const Vector2& startUV, const Vector2& size)
	{
		mButtonUVInfo.downStartUV = startUV;
		mButtonUVInfo.downEndUV = startUV + size;
	}

	void UIButton::SetUVHoverPressedDown(const Vector2& startUV, const Vector2& size)
	{
		mButtonUVInfo.downStartUV = mButtonUVInfo.pressedStartUV = mButtonUVInfo.hoverStartUV = startUV;
		mButtonUVInfo.downEndUV = mButtonUVInfo.pressedEndUV = mButtonUVInfo.hoverEndUV = startUV + size;
	}
}