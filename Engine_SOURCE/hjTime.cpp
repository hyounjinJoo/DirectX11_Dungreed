#include "pch.h"
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
    bool            Time::mbStopDeltaTimeUpdate = false;
    
    void Time::Initialize()
    {
        //CPU 의 초당 반복되는 주파수를 얻어온다.
        QueryPerformanceFrequency(&mCpuFrequency);

        //프로그램을 시작했을때의 CPU 클럭 수
        QueryPerformanceCounter(&mPrevFrequency);
    }

    void Time::Update()
    {
        static const float targetFrameRate = 60.f;
        static const float targetDeltaTime = 1.f / targetFrameRate;
        static const float targetFixedFPS = 1.f / targetDeltaTime;
        
        static bool isFocused = application.IsFocused();
		if (isFocused)
		{
            QueryPerformanceCounter(&mCurFrequency);

            float differenceInFrequancy
                = static_cast<float>((mCurFrequency.QuadPart - mPrevFrequency.QuadPart));

            // 현재 DeltaTime 계산
            mActualDeltaTime = differenceInFrequancy / static_cast<float>(mCpuFrequency.QuadPart);
            // FixedTime 보정을 위한 FPS 비율 계산
            float FPS = 1.f / mActualDeltaTime;
            float FPSRatio = targetFixedFPS / FPS;
            
            // 고정된 FixedTime 설정
			mFixedDeltaTime = targetDeltaTime;

            // 현재 ActualDeltaTime이 더 작은 경우 물리연산용인 FixedDeltaTime에 의한 FPS보다 빠른 경우이므로 보정이 필요하다. 
            // mActualDeltaTime에 계산된 비율을 곱하여 보정하여 처리
            if (mFixedDeltaTime > mActualDeltaTime)
            {
                mFixedDeltaTime *= FPSRatio;
            }
			mPrevFrequency.QuadPart = mCurFrequency.QuadPart;
            if (mbStopDeltaTimeUpdate)
            {
                mFixedDeltaTime = 0.f;
            }
            else
            {
                mAccTime += mActualDeltaTime; 
			}
		}
		else
		{
			mActualDeltaTime = 0.f;
			mFixedDeltaTime = 0.f;
		}
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
