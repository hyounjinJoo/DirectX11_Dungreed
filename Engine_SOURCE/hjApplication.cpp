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
    
    // �ʱ�ȭ
    void Application::Initialize()
    {
        Time::Initialize();
        Input::Initialize();

        renderer::Initialize();
    }

    // ���� ���� ĳ���� �̵� ����� CPU Update
    void Application::Update()
    {
        Time::Update();
        Input::Update();
    }

    // CPU Update �� �������� GPU�� Update
    void Application::FixedUpdate()
    {
    }

    // ������ ����
    void Application::Render()
    {
        Time::Render(mHdc);
        graphicDevice->Render();
    }
    
    void Application::SetWindow(HWND hwnd, UINT width, UINT height)
    {
        // �׷��� ��ġ�� �������� ������ �޸𸮿� �Ҵ� ����
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

        // ������ ũ�� ����
        RECT rt = { 0, 0, (LONG)width, (LONG)height };
        AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, false);
        SetWindowPos(mHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
        ShowWindow(mHwnd, true);
        UpdateWindow(mHwnd);
    }
}
