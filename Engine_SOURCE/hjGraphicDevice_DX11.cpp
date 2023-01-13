#include "hjGraphicDevice_DX11.h"
#include "hjApplication.h"

extern hj::Application application;

namespace hj::graphics
{
	GraphicDevice_DX11::GraphicDevice_DX11(ValidationMode validationMode)
	{
		///	<Summary>
		///	1. Device와 SwapChain을 생성한다.
		/// 2. 백버퍼에 실제로 렌더링을 수행하는 렌더 타겟 뷰를 생성해야 한다.
		/// 3. 화면을 클리어 해줘야 한다. 뷰포트를 생성 해줘야 한다.
		/// 4. 매 프레임마다 위에서 생성한 렌더 타켓 뷰에 렌더링 해줘야 한다.
		
		HWND hwnd = application.GetHwnd();

		// Device, Device Context 생성
		UINT deviceFlag = D3D11_CREATE_DEVICE_DEBUG;
		D3D_FEATURE_LEVEL featureLevel = static_cast<D3D_FEATURE_LEVEL>(0);

		HRESULT hr = D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr
									, deviceFlag, nullptr, 0
									, D3D11_SDK_VERSION
									, mDevice.GetAddressOf()
									, &featureLevel
									, mContext.GetAddressOf());

		// Swap Chain 생성
		DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
		/*
			DXGI_SWAP_CHAIN_DESC

			DXGI_MODE_DESC		BufferDesc;
			DXGI_SAMPLE_DESC	SampleDesc;
			DXGI_USAGE			BufferUsage;
			UINT				BufferCount;
			HWND				OutputWindow;
			BOOL				Windowed;
			DXGI_SWAP_EFFECT	SwapEffect;
			UINT				Flags;
		*/

		swapChainDesc.OutputWindow	= hwnd;
		swapChainDesc.Windowed		= TRUE;
		swapChainDesc.BufferCount	= static_cast<UINT>(2);
		swapChainDesc.BufferUsage	= DXGI_USAGE_RENDER_TARGET_OUTPUT;

		/*
			DXGI_MODE_DESC

		    UINT Width;
		    UINT Height;
		    DXGI_RATIONAL RefreshRate;
		    DXGI_FORMAT Format;
		    DXGI_MODE_SCANLINE_ORDER ScanlineOrdering;
		    DXGI_MODE_SCALING Scaling;
		*/
		swapChainDesc.BufferDesc.Width						= application.GetWidth();
		swapChainDesc.BufferDesc.Height						= application.GetHeight();
		swapChainDesc.BufferDesc.Format						= DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator		= static_cast<UINT>(240);
		swapChainDesc.BufferDesc.RefreshRate.Denominator	= static_cast<UINT>(1);
		swapChainDesc.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;
		swapChainDesc.SampleDesc.Count						= static_cast<UINT>(1);
		swapChainDesc.SampleDesc.Quality					= static_cast<UINT>(0);

		// 실패 시 종료
		if (!CreateSwapChain(&swapChainDesc))
			return;

		// Get RenderTarget for SwapChain
		hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)mRenderTarget.GetAddressOf());

		// Create RenderTarget View
		hr = mDevice->CreateRenderTargetView(mRenderTarget.Get(), nullptr, mRenderTargetView.GetAddressOf());

		// Create Depth Stencil Buffer, Depth Stencil View
		D3D11_TEXTURE2D_DESC depthBuffer = {};
		/*
			D3D11_TEXTURE2D_DESC

			UINT Width;
			UINT Height;
			UINT MipLevels;
			UINT ArraySize;
			DXGI_FORMAT Format;
			DXGI_SAMPLE_DESC SampleDesc;
			D3D11_USAGE Usage;
			UINT BindFlags;
			UINT CPUAccessFlags;
			UINT MiscFlags;
		*/

		depthBuffer.BindFlags			= D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
		depthBuffer.Format				= DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBuffer.Usage				= D3D11_USAGE_DEFAULT;
		depthBuffer.CPUAccessFlags		= static_cast<UINT>(0);
		depthBuffer.Width				= application.GetWidth();
		depthBuffer.Height				= application.GetHeight();
		depthBuffer.ArraySize			= static_cast<UINT>(1);
		depthBuffer.SampleDesc.Count	= static_cast<UINT>(1);
		depthBuffer.SampleDesc.Quality	= static_cast<UINT>(0);
		depthBuffer.MipLevels			= static_cast<UINT>(1);
		depthBuffer.MiscFlags			= static_cast<UINT>(0);

		if (!CreateTexture(&depthBuffer, mDepthStencilBuffer.GetAddressOf()))
			return;

		if (FAILED(mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr, mDepthStencilView.GetAddressOf())))
			return;
	}

	GraphicDevice_DX11::~GraphicDevice_DX11()
	{
	}

	bool GraphicDevice_DX11::CreateSwapChain(DXGI_SWAP_CHAIN_DESC* desc)
	{
		Microsoft::WRL::ComPtr<IDXGIDevice> pDXGIDevice = nullptr;
		Microsoft::WRL::ComPtr<IDXGIAdapter> pDXGIAdapter = nullptr;
		Microsoft::WRL::ComPtr<IDXGIFactory> pDXGIFactory = nullptr;

		// IDXGIDevice 인터페이스 가져오기
		if (FAILED(mDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)pDXGIDevice.GetAddressOf())))
			return false;

		// IDXGIAdapter 인터페이스 가져오기
		if (FAILED(pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void**)pDXGIAdapter.GetAddressOf())))
			return false;

		// IDXGIFactory 인터페이스 가져오기
		if (FAILED(pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void**)pDXGIFactory.GetAddressOf())))
			return false;

		// Swap Chain 생성
		if (FAILED(pDXGIFactory->CreateSwapChain(mDevice.Get(), desc, mSwapChain.GetAddressOf())))
			return false;

		return true;
	}

	bool GraphicDevice_DX11::CreateTexture(D3D11_TEXTURE2D_DESC* desc, ID3D11Texture2D** ppTexture2D)
	{
		// ID3D11Texture2D 생성
		if (FAILED(mDevice->CreateTexture2D(desc, nullptr, ppTexture2D)))
			return false;

		return true;
	}

	void GraphicDevice_DX11::Draw()
	{
		// 렌더 타겟 뷰를 지정된 색상으로 클리어 시켜준다.
		FLOAT backgroundColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		mContext->ClearRenderTargetView(mRenderTargetView.Get(), backgroundColor);

		// 렌더링 된 이미지를 사용자에게 제공한다.
		mSwapChain->Present(1, 0);
	}

}
