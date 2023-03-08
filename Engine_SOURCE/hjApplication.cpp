#include "hjApplication.h"
#include "hjRenderer.h"
#include "hjTime.h"
#include "hjInput.h"
#include "hjSceneManager.h"
#include "hjResources.h"
#include "hjCollisionManager.h"

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

    // 초기화
    void Application::Initialize()
    {
        Time::Initialize();
        Input::Initialize();
        CollisionManager::Initialize();
        renderer::Initialize();
        SceneManager::Initialize();
    }

    // 게임 로직 캐릭터 이동 등등의 CPU Update
    void Application::Update()
    {
        Time::Update();
        Input::Update();

		CollisionManager::Update();
        SceneManager::Update();
    }

    // CPU Update 된 정보들을 GPU에 Update
    void Application::FixedUpdate()
	{
		CollisionManager::FixedUpdate();
        SceneManager::FixedUpdate();
    }

    // 렌더링 수행
    void Application::Render()
    {
        Time::Render(mHdc);

        graphicDevice->Clear();
        graphicDevice->AdjustViewPorts();

        //SceneManager::Render();
		renderer::Render();
		CollisionManager::Render();

        //graphicDevice->Render();
        //graphicDevice->Present();
    }
    
	void Application::Destroy()
	{

	}

	// Running main engine loop
    void Application::Run()
    {
        Update();
        FixedUpdate();
        Render();
        Destroy();
    }

    void Application::Present()
	{
		graphicDevice->Present();
    }

    void Application::Release()
    {
        Resources::deleteTest();
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
        AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, true);
        SetWindowPos(mHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
        ShowWindow(mHwnd, true);
        UpdateWindow(mHwnd);
    }
}
