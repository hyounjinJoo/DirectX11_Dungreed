#include "hjTime.h"
#include "hjApplication.h"

extern hj::Application application;

namespace hj
{
    LARGE_INTEGER	Time::mCpuFrequency = {};
    LARGE_INTEGER   Time::mPrevFrequency = {};
    LARGE_INTEGER	Time::mCurFrequency = {};
	float			Time::mFixedDeltaTime = 0.0f;
	float			Time::mActualDeltaTime = 0.0f;
    float           Time::mAccTime = 0.0f;
    float			Time::mOneSecond = 0.0f;
    static int test = 0;

    void Time::Initialize()
    {
        //CPU 의 초당 반복되는 주파수를 얻어온다.
        QueryPerformanceFrequency(&mCpuFrequency);

        //프로그램을 시작했을때의 CPU 클럭 수
        QueryPerformanceCounter(&mPrevFrequency);
    }

    void Time::Update()
    {
        static const float targetFrameRate = 144.f;
        static const float targetDeltaTime = 1.f / targetFrameRate;
        static const float targetFixedFPS = 1.f / targetDeltaTime;
        HWND activeWindow = GetFocus();
        //if (activeWindow == application.GetHwnd())
        //{
            QueryPerformanceCounter(&mCurFrequency);

            float differenceInFrequancy
                = static_cast<float>((mCurFrequency.QuadPart - mPrevFrequency.QuadPart));

            mActualDeltaTime = differenceInFrequancy / static_cast<float>(mCpuFrequency.QuadPart);
            if (mActualDeltaTime > 1.f / targetFrameRate * 2.f)
                mActualDeltaTime = 1.f / targetFrameRate * 2.f;
			float FPS = 1.f / mActualDeltaTime;
            float FPSRatio = targetFixedFPS / FPS;
			//mFixedDeltaTime = mActualDeltaTime / FPSRatio;
			mFixedDeltaTime = targetDeltaTime;
            if (mFixedDeltaTime > mActualDeltaTime)
                mFixedDeltaTime = 1.f / 144.f * static_cast<float>((mActualDeltaTime / targetDeltaTime));
            mAccTime += mActualDeltaTime; 
            mPrevFrequency.QuadPart = mCurFrequency.QuadPart;
        //}
        //else
        //{
        //    mActualDeltaTime = 0.f;
        //    mFixedDeltaTime = 0.f;
        //}
    }

    void Time::Render(HDC hdc)
    {
        static int iCount = 0;
        ++iCount;


        // 1 초에 한번
        mOneSecond += mActualDeltaTime;
        if (1.0f < mOneSecond)
        {
            HWND hWnd = application.GetHwnd();

            wchar_t szFloat[50] = {};
			float FPS = 1.f / mActualDeltaTime;
			float FixedFPS = 1.f / mFixedDeltaTime;
            swprintf_s(szFloat, 50, L"DeltaTime [Actual : %d], [Fixed : %d]", std::lround(FPS), std::lround(FixedFPS));
            int iLen = static_cast<int>(wcsnlen_s(szFloat, 50));
            //TextOut(_dc, 10, 10, szFloat, iLen);

            SetWindowText(hWnd, szFloat);

            // 누적시간, 카운트 초기화
            mOneSecond = 0.f;
            iCount = 0;
        }
    }
}
