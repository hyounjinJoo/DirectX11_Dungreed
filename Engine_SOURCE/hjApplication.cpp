#include "hjApplication.h"
#include "hjRenderer.h"
#include "hjTime.h"
#include "hjInput.h"

namespace hj
{
    using namespace graphics;

    Application::Application()
        : mHwnd(nullptr)
        , mHdc(nullptr)
        , mHeight(1600)
        , mWidth(900)
    {
    }

    Application::~Application()
    {
    }
    
    // Running main engine loop
    void Application::Run()
    {
        Update();
        FixedUpdate();
        Render();
    }
    
    // 초기화
    void Application::Initialize()
    {
        Time::Initialize();
        Input::Initialize();

        renderer::Initialize();
    }

    // 게임 로직 캐릭터 이동 등등의 CPU Update
    void Application::Update()
    {
        Time::Update();
        Input::Update();
    }

    // CPU Update 된 정보들을 GPU에 Update
    void Application::FixedUpdate()
    {
    }

    // 렌더링 수행
    void Application::Render()
    {
        Time::Render(mHdc);
        graphicDevice->Render();
    }
    
    void Application::SetWindow(HWND hwnd, UINT width, UINT height)
    {
        // 그래픽 장치가 존재하지 않으면 메모리에 할당 수행
        if (graphicDevice == nullptr)
        {
            mHwnd = hwnd;
            mHdc = GetDC(mHwnd);
            mWidth = width;
            mHeight = height;

            ValidationMode validationMode = ValidationMode::Disabled;
            graphicDevice = std::make_unique<GraphicDevice_DX11>();
            graphics::GetDevice() = graphicDevice.get();
        }

        // 윈도우 크기 조정
        RECT rt = { 0, 0, (LONG)width, (LONG)height };
        AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
        SetWindowPos(mHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
        ShowWindow(mHwnd, true);
        UpdateWindow(mHwnd);
    }
}
