#pragma once
#include "hjEngine.h"
#include "hjGraphics.h"
#include "hjGraphicDevice_DX11.h"

namespace hj
{
	class Application
	{
	public:
		Application();
		~Application();

#pragma region Main Application Logic
		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();
		virtual void Destroy();

		// Running main engine loop
		void Run();
		void Present();
		void Release();
#pragma endregion

#pragma region Setter
		void SetWindow(HWND hwnd, UINT width, UINT height);
		void SetHwnd(HWND hwnd) { mHwnd = hwnd; }
#pragma endregion

#pragma region Getter
		HWND GetHwnd() { return mHwnd; }
		UINT GetWidth() { return mWidth; }
		UINT GetHeight() { return mHeight; }
		math::Vector2 GetWindowSize() { return math::Vector2(static_cast<float>(mWidth), static_cast<float>(mHeight)); }
		math::Vector2 GetResolutionRatio();
#pragma endregion;

#pragma region MemberVariable
	private:
		bool	initialized = false;
		std::unique_ptr<graphics::GraphicDevice_DX11> graphicDevice;

		HWND	mHwnd;
		HDC		mHdc;
		UINT	mHeight;
		UINT	mWidth;
#pragma endregion

	};
}
