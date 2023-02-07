#include "hjApplication.h"
#include "hjRenderer.h"
#include "hjTime.h"
#include "hjInput.h"
#include "hjSceneManager.h"
#include "hjResources.h"

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
    // �ʱ�ȭ
    void Application::Initialize()
    {
        Time::Initialize();
        Input::Initialize();

        renderer::Initialize();
        SceneManager::Initialize();
    }

    // ���� ���� ĳ���� �̵� ����� CPU Update
    void Application::Update()
    {
        Time::Update();
        Input::Update();

        SceneManager::Update();
    }

    // CPU Update �� �������� GPU�� Update
    void Application::FixedUpdate()
    {
        SceneManager::FixedUpdate();
    }

    // ������ ����
    void Application::Render()
    {
        Time::Render(mHdc);

        graphicDevice->Clear();
        graphicDevice->AdjustViewPorts();

        SceneManager::Render();

        //graphicDevice->Render();
        graphicDevice->Present();
    }
    
    // Running main engine loop
    void Application::Run()
    {
        Update();
        FixedUpdate();
        Render();
    }

    void Application::Release()
    {
        Resources::Release();
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
