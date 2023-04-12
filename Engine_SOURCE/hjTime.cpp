#include "hjTime.h"
#include "hjApplication.h"

extern hj::Application application;

namespace hj
{
    LARGE_INTEGER	Time::mCpuFrequency = {};
    LARGE_INTEGER   Time::mPrevFrequency = {};
    LARGE_INTEGER	Time::mCurFrequency = {};
    float			Time::mDeltaTime = 0.0f;
    float           Time::mAccTime = 0.0f;
    float			Time::mOneSecond = 0.0f;

    void Time::Initialize()
    {
        //CPU 의 초당 반복되는 주파수를 얻어온다.
        QueryPerformanceFrequency(&mCpuFrequency);

        //프로그램을 시작했을때의 CPU 클럭 수
        QueryPerformanceCounter(&mPrevFrequency);
    }

    void Time::Update()
    {
        HWND activeWindow = GetFocus();
        if (activeWindow == application.GetHwnd())
        {

            QueryPerformanceCounter(&mCurFrequency);

            float differenceInFrequancy
                = static_cast<float>((mCurFrequency.QuadPart - mPrevFrequency.QuadPart));

            mDeltaTime = differenceInFrequancy / static_cast<float>(mCpuFrequency.QuadPart);
            mAccTime += mDeltaTime;
            mPrevFrequency.QuadPart = mCurFrequency.QuadPart;

            float deviceNumerator = static_cast<float>(hj::graphics::GetDevice()->GetSwapChainNumerator());

            if (deviceNumerator <= 0.f)
                deviceNumerator = 144.f;
            if (mDeltaTime >= 1.f / deviceNumerator)
            {
                mDeltaTime = 1.f / deviceNumerator;
            }
        }
        else
        {
            mDeltaTime = 0.f;
        }
    }

    void Time::Render(HDC hdc)
    {
        static int iCount = 0;
        ++iCount;


        // 1 초에 한번
        mOneSecond += mDeltaTime;
        if (1.0f < mOneSecond)
        {
            HWND hWnd = application.GetHwnd();

            wchar_t szFloat[50] = {};
            float FPS = 1.f / mDeltaTime;
            swprintf_s(szFloat, 50, L"DeltaTime : %d", iCount);
            int iLen = static_cast<int>(wcsnlen_s(szFloat, 50));
            //TextOut(_dc, 10, 10, szFloat, iLen);

            SetWindowText(hWnd, szFloat);

            // 누적시간, 카운트 초기화
            mOneSecond = 0.f;
            iCount = 0;
        }
    }
}
