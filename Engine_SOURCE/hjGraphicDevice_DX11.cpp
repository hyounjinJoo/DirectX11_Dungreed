
#include "hjGraphicDevice_DX11.h"
#include "hjApplication.h"
#include "hjRenderer.h"
#include "hjConstantBuffer.h"
#include "hjMesh.h"

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

		swapChainDesc.OutputWindow	= hwnd;
		swapChainDesc.Windowed		= TRUE;
		swapChainDesc.BufferCount	= static_cast<UINT>(2);
		swapChainDesc.SwapEffect	= DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;
		swapChainDesc.BufferUsage	= DXGI_USAGE_RENDER_TARGET_OUTPUT;

		swapChainDesc.BufferDesc.Width						= application.GetWidth();
		swapChainDesc.BufferDesc.Height						= application.GetHeight();
		swapChainDesc.BufferDesc.Format						= DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM;
		swapChainDesc.BufferDesc.RefreshRate.Numerator		= static_cast<UINT>(240);
		swapChainDesc.BufferDesc.RefreshRate.Denominator	= static_cast<UINT>(1);
		swapChainDesc.BufferDesc.ScanlineOrdering			= DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
		swapChainDesc.BufferDesc.Scaling					= DXGI_MODE_SCALING_UNSPECIFIED;

		swapChainDesc.SampleDesc.Count						= static_cast<UINT>(1);
		swapChainDesc.SampleDesc.Quality					= static_cast<UINT>(0);

		if (!CreateSwapChain(&swapChainDesc))
			return;

		// Get RenderTarget for SwapChain
		hr = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)mRenderTarget.GetAddressOf());

		// Create RenderTarget View
		hr = mDevice->CreateRenderTargetView(mRenderTarget.Get(), nullptr, mRenderTargetView.GetAddressOf());

		D3D11_TEXTURE2D_DESC depthBuffer = {};

		depthBuffer.BindFlags			= D3D11_BIND_FLAG::D3D11_BIND_DEPTH_STENCIL;
		depthBuffer.Format				= DXGI_FORMAT::DXGI_FORMAT_D24_UNORM_S8_UINT;
		depthBuffer.Usage				= D3D11_USAGE_DEFAULT;
		depthBuffer.CPUAccessFlags		= static_cast<UINT>(0);
		depthBuffer.Width				= application.GetWidth();
		depthBuffer.Height				= application.GetHeight();
		depthBuffer.ArraySize			= static_cast<UINT>(1);

		depthBuffer.SampleDesc.Count	= static_cast<UINT>(1);
		depthBuffer.SampleDesc.Quality	= static_cast<UINT>(0);

		depthBuffer.MipLevels			= static_cast<UINT>(0);
		depthBuffer.MiscFlags			= static_cast<UINT>(0);

		// Create Depth Stencil Buffer
		if (!CreateTexture(&depthBuffer, mDepthStencilBuffer.GetAddressOf()))
			return;

		// Create Depth Stencil View
		if (FAILED(mDevice->CreateDepthStencilView(mDepthStencilBuffer.Get(), nullptr, mDepthStencilView.GetAddressOf())))
			return;

		// 뷰포트 좌표계 설정
		// (0 , 0)             (right - left , 0)
		// |------------------------------------|
		// |                                    |
		// |------------------------------------|
		// (0 , bottom - top)                   (right - left, bottom - top)
		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);
		mViewPort = { 0.f, 0.f, FLOAT(winRect.right - winRect.left), FLOAT(winRect.bottom - winRect.top), 0.f, 1.f };
		BindViewports(&mViewPort);
		mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
	}

	GraphicDevice_DX11::~GraphicDevice_DX11()
	{
		renderer::Release();
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

	bool GraphicDevice_DX11::CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* desc, UINT NumElements, const void* byteCode, SIZE_T bytecodeLength, ID3D11InputLayout** ppInputLayout)
	{
		if (FAILED(mDevice->CreateInputLayout(desc, NumElements, byteCode, bytecodeLength, ppInputLayout)))
			return false;

		return true;
	}

	bool GraphicDevice_DX11::CreateBuffer(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* data, ID3D11Buffer** buffer)
	{
		if (FAILED(mDevice->CreateBuffer(desc, data, buffer)))
			return false;

		return true;
	}

	bool GraphicDevice_DX11::CreateVertexShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11VertexShader** ppVertexShader)
	{
		if (FAILED(mDevice->CreateVertexShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppVertexShader)))
			return false;

		return true;
	}

	bool GraphicDevice_DX11::CreatePixelShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11PixelShader** ppPixelShader)
	{
		if (FAILED(mDevice->CreatePixelShader(pShaderBytecode, BytecodeLength, pClassLinkage, ppPixelShader)))
			return false;

		return true;
	}

	bool GraphicDevice_DX11::CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState)
	{
		if (FAILED(mDevice->CreateSamplerState(pSamplerDesc, ppSamplerState)))
			return false;

		return true;
	}

	bool GraphicDevice_DX11::CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc, ID3D11RasterizerState** ppRasterizerState)
	{
		if (FAILED(mDevice->CreateRasterizerState(pRasterizerDesc, ppRasterizerState)))
			return false;

		return true;
	}

	bool GraphicDevice_DX11::CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* pDepthStencilDesc, ID3D11DepthStencilState** ppDepthStencilState)
	{
		if (FAILED(mDevice->CreateDepthStencilState(pDepthStencilDesc, ppDepthStencilState)))
			return false;

		return true;
	}

	bool GraphicDevice_DX11::CreateBlendState(const D3D11_BLEND_DESC* pBlendStateDesc, ID3D11BlendState** ppBlendState)
	{
		if (FAILED(mDevice->CreateBlendState(pBlendStateDesc, ppBlendState)))
			return false;

		return true;
	}

	void GraphicDevice_DX11::BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology)
	{
		mContext->IASetPrimitiveTopology(topology);
	}

	void GraphicDevice_DX11::BindInputLayout(ID3D11InputLayout* pInputLayout)
	{
		mContext->IASetInputLayout(pInputLayout);
	}

	void GraphicDevice_DX11::BindVertexBuffer(UINT StartSlot
		, UINT NumBuffers
		, ID3D11Buffer* const* ppVertexBuffers
		, const UINT* pStrides
		, const UINT* pOffsets)
	{		
		mContext->IASetVertexBuffers(StartSlot, NumBuffers, ppVertexBuffers, pStrides, pOffsets);
	}

	void GraphicDevice_DX11::BindIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset)
	{		
		mContext->IASetIndexBuffer(pIndexBuffer, Format, Offset);
	}

	void GraphicDevice_DX11::BindVertexShader(ID3D11VertexShader* pVertexShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances)
	{
		mContext->VSSetShader(pVertexShader, ppClassInstances, NumClassInstances);
	}

	void GraphicDevice_DX11::BindPixelShader(ID3D11PixelShader* pPixelShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances)
	{
		mContext->PSSetShader(pPixelShader, ppClassInstances, NumClassInstances);
	}

	void GraphicDevice_DX11::BindViewports(D3D11_VIEWPORT* viewPort)
	{
		mContext->RSSetViewports(1, viewPort);
	}

	void GraphicDevice_DX11::BindConstantBuffer(ID3D11Buffer* buffer, void* data, UINT size)
	{
		D3D11_MAPPED_SUBRESOURCE sub = {};
		mContext->Map(buffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &sub);
		memcpy(sub.pData, data, size);
		mContext->Unmap(buffer, 0);
	}

	void GraphicDevice_DX11::SetConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer)
	{
		switch (stage)
		{
		case hj::graphics::eShaderStage::VS:
			mContext->VSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case hj::graphics::eShaderStage::HS:
			mContext->HSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case hj::graphics::eShaderStage::DS:
			mContext->DSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case hj::graphics::eShaderStage::GS:
			mContext->GSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case hj::graphics::eShaderStage::PS:
			mContext->PSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		case hj::graphics::eShaderStage::CS:
			mContext->CSSetConstantBuffers((UINT)type, 1, &buffer);
			break;
		default:
			break;
		}
	}

	void GraphicDevice_DX11::SetShaderResource(eShaderStage stage, UINT slot
		, ID3D11ShaderResourceView* const* ppShaderResourceViews)
	{
		switch (stage)
		{
		case hj::graphics::eShaderStage::VS:
			mContext->VSSetShaderResources(slot, 1, ppShaderResourceViews);
			break;
		case hj::graphics::eShaderStage::HS:
			mContext->HSSetShaderResources(slot, 1, ppShaderResourceViews);
			break;
		case hj::graphics::eShaderStage::DS:
			mContext->DSSetShaderResources(slot, 1, ppShaderResourceViews);
			break;
		case hj::graphics::eShaderStage::GS:
			mContext->GSSetShaderResources(slot, 1, ppShaderResourceViews);
			break;
		case hj::graphics::eShaderStage::PS:
			mContext->PSSetShaderResources(slot, 1, ppShaderResourceViews);
			break;
		case hj::graphics::eShaderStage::CS:
			mContext->CSSetShaderResources(slot, 1, ppShaderResourceViews);
			break;
		default:
			break;
		}
	}

	void GraphicDevice_DX11::BindSamplers(eShaderStage stage, UINT slot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers)
	{
		switch (stage)
		{
		case hj::graphics::eShaderStage::VS:
			mContext->VSSetSamplers(slot, NumSamplers, ppSamplers);
			break;
		case hj::graphics::eShaderStage::HS:
			mContext->HSSetSamplers(slot, NumSamplers, ppSamplers);
			break;
		case hj::graphics::eShaderStage::DS:
			mContext->DSSetSamplers(slot, NumSamplers, ppSamplers);
			break;
		case hj::graphics::eShaderStage::GS:
			mContext->GSSetSamplers(slot, NumSamplers, ppSamplers);
			break;
		case hj::graphics::eShaderStage::PS:
			mContext->PSSetSamplers(slot, NumSamplers, ppSamplers);
			break;
		case hj::graphics::eShaderStage::CS:
			mContext->CSSetSamplers(slot, NumSamplers, ppSamplers);
			break;
		default:
			break;
		}
	}

	void GraphicDevice_DX11::BindsSamplers(UINT slot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers)
	{
		mContext->VSSetSamplers(slot, NumSamplers, ppSamplers);
		mContext->HSSetSamplers(slot, NumSamplers, ppSamplers);
		mContext->DSSetSamplers(slot, NumSamplers, ppSamplers);
		mContext->GSSetSamplers(slot, NumSamplers, ppSamplers);
		mContext->PSSetSamplers(slot, NumSamplers, ppSamplers);
	}

	void GraphicDevice_DX11::BindRasterizerState(ID3D11RasterizerState* pRasterizerState)
	{
		mContext->RSSetState(pRasterizerState);
	}

	void GraphicDevice_DX11::BindDepthStencilState(ID3D11DepthStencilState* pDepthStencilState)
	{
		mContext->OMSetDepthStencilState(pDepthStencilState, 0);
	}

	void GraphicDevice_DX11::BindBlendState(ID3D11BlendState* pBlendState)
	{
		mContext->OMSetBlendState(pBlendState, nullptr, 0xffffffff);
	}

	void GraphicDevice_DX11::Clear()
	{
		// 렌더 타겟 뷰를 지정된 색상으로 클리어 시켜준다.
		FLOAT backgroundColor[4] = { 0.2f, 0.2f, 0.2f, 1.0f };
		mContext->ClearRenderTargetView(mRenderTargetView.Get(), backgroundColor);
		mContext->ClearDepthStencilView(mDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
	}

	void GraphicDevice_DX11::AdjustViewPorts()
	{
		// ViewPort, RenderTarget 갱신
		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);
		mViewPort = { 0.f, 0.f, FLOAT(winRect.right - winRect.left), FLOAT(winRect.bottom - winRect.top), 0.f, 1.f };
		BindViewports(&mViewPort);
		mContext->OMSetRenderTargets(1, mRenderTargetView.GetAddressOf(), mDepthStencilView.Get());
	}

	void GraphicDevice_DX11::Draw()
	{
		mContext->Draw(0, 0);
	}

	void GraphicDevice_DX11::DrawIndexed(UINT indexCount, UINT startIndexLocation, UINT baseVertexLocation)
	{
		// 정점을 그려준다.
		mContext->DrawIndexed(indexCount, startIndexLocation, baseVertexLocation);
	}

	void GraphicDevice_DX11::Present()
	{
		mSwapChain->Present(0, 0);
	}
}
