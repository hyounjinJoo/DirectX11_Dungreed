#pragma once
#include "hjMath.h"


namespace hj
{
	class Application
	{
	public:
#pragma region Setter
		void SetHwnd(HWND hwnd) { mHwnd = hwnd; }
#pragma endregion

#pragma region Getter
		HWND GetHwnd() { return mHwnd; }
#pragma endregion;

#pragma region MemberVariable
	private:
		HWND mHwnd;
#pragma endregion

	};
}
