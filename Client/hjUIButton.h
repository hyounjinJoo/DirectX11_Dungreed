#pragma once
#include "hjUIBase.h"

namespace hj
{
	enum class eUIButtonState
	{
		IDLE,
		HOVER,
		PRESSED,
		DOWN,
		End,
	};

	struct UIButtonUV
	{
		Vector2 idleStartUV;
		Vector2 idleEndUV;

		Vector2 hoverStartUV;
		Vector2 hoverEndUV;

		Vector2 pressedStartUV;
		Vector2 pressedEndUV;

		Vector2 downStartUV;
		Vector2 downEndUV;

		UIButtonUV()
			: idleStartUV(Vector2::Zero)
			, idleEndUV(Vector2::One)
			, hoverStartUV(Vector2::Zero)
			, hoverEndUV(Vector2::One)
			, pressedStartUV(Vector2::Zero)
			, pressedEndUV(Vector2::One)
			, downStartUV(Vector2::Zero)
			, downEndUV(Vector2::One)
		{
		}
	};

	class UIButton :
		public UIBase
	{
	public:
		UIButton();
		virtual ~UIButton();

		void Initialize() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() override;

		void UpdateState();
		void UpdateScale();
		void ChangeUV();

	protected:
		void SetSameSizeAll(const Vector2& size);
		void SetUVIdle(const Vector2& startUV, const Vector2& size);
		void SetUVHover(const Vector2& startUV, const Vector2& size);
		void SetUVPressed(const Vector2& startUV, const Vector2& size);
		void SetUVDown(const Vector2& startUV, const Vector2& size);
		void SetUVHoverPressedDown(const Vector2& startUV, const Vector2& size);


	protected:
		class Collider2D* mUIButtonCollider;

		Vector2 mIdleSize;
		Vector2 mHoverSize;
		Vector2 mPressedSize;
		Vector2 mDownSize;
		eUIButtonState mState;

		UIButtonUV mButtonUVInfo;

		std::function<void()> mClickCallback;

	public:
		template<typename T>
		void SetClickCallback(T* obj, void (T::* func)())
		{
			mClickCallback = std::bind(func, obj);
		}

		void SetClickCallback(void (*func)())
		{
			mClickCallback = std::bind(func);
		}
	};
}