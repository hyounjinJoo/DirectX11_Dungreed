#include "pch.h"
#include "hjApplication.h"
#include "hjRenderer.h"
#include "hjTime.h"
#include "hjInput.h"
#include "hjSceneManager.h"
#include "hjResources.h"
#include "hjCollisionManager.h"
#include "hjFmod.h"

namespace hj
{
    using namespace graphics;

    Application::Application()
        : mHwnd(nullptr)
        , mHdc(nullptr)
        , mHeight(1600)
        , mWidth(900)
        , mbIsFocusedWindow(false)
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
		Fmod::Initialize();
        CollisionManager::Initialize();
        renderer::Initialize();
        SceneManager::Initialize();
    }

    // ���� ���� ĳ���� �̵� ����� CPU Update
    void Application::Update()
    {
        UpdateFocusInfo();
        Time::Update();
        Input::Update();
		CollisionManager::Update();
        SceneManager::Update();
    }

    // CPU Update �� �������� GPU�� Update
    void Application::FixedUpdate()
	{
		CollisionManager::FixedUpdate();
        SceneManager::FixedUpdate();
    }

    // ������ ����
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
    
	void Application::RemoveDeadObjects()
	{
        SceneManager::RemoveDeadObjects();
	}

	// Running main engine loop
    void Application::Run()
    {
        Update();
        FixedUpdate();
        Render();
        if (SceneManager::IsNeedToLoadScene())
            SceneManager::LoadScene();
    }

    void Application::Present()
	{
		graphicDevice->Present();
    }

    void Application::Release()
    {
		Resources::deleteTest();
		Fmod::Release();
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
            //graphics::GetDevice() = graphicDevice.get();
        }

        // ������ ũ�� ����
        RECT rt = { 0, 0, (LONG)width, (LONG)height };
        AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, true);
        SetWindowPos(mHwnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
        ShowWindow(mHwnd, true);
        UpdateWindow(mHwnd);
    }

	Vector2 Application::GetResolutionRatio()
	{
		RECT	windowRC;

		GetClientRect(mHwnd, &windowRC);

		float width = static_cast<float>(windowRC.right - windowRC.left);
		float height = static_cast<float>(windowRC.bottom - windowRC.top);

        return Vector2(mWidth / width, mHeight / height);
	}

    void Application::UpdateFocusInfo()
    {
        HWND currentActiveWindow = GetFocus();
        mbIsFocusedWindow = (mHwnd == currentActiveWindow) ? true : false;
    }
}
