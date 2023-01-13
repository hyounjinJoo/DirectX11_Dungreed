#pragma once
#include "CommonInclude.h"
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
		// Running main engine loop
		void Run();

		// 초기화
		virtual void Initialize();

		// 게임 로직 캐릭터 이동 등등의 CPU Update
		virtual void Update();

		// CPU Update 된 정보들을 GPU에 Update
		virtual void FixedUpdate();

		// 렌더링 수행
		virtual void Render();
#pragma endregion

#pragma region Setter
		void SetWindow(HWND hwnd, UINT width, UINT height);
		void SetHwnd(HWND hwnd) { mHwnd = hwnd; }
#pragma endregion

#pragma region Getter
		HWND GetHwnd() { return mHwnd; }
		UINT GetWidth() { return mWidth; }
		UINT GetHeight() { return mHeight; }
#pragma endregion;

#pragma region MemberVariable
	private:
		bool initialized = false;
		std::unique_ptr<graphics::GraphicDevice_DX11> graphicDevice;

		HWND mHwnd;
		UINT mHeight;
		UINT mWidth;
#pragma endregion

	};
}
