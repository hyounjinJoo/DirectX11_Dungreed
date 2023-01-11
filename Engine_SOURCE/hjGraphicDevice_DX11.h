#pragma once
#include "hjGraphics.h"

// 다이렉트X11을 사용하기 위한 헤더
#include <d3d11.h>
// 다이렉트X11을 사용한 것을 컴파일 해주는 컴파일러 헤더
#include <d3dcompiler.h>

namespace hj::graphics
{
	class GraphicDevice_DX11
	{
	public:
		GraphicDevice_DX11();
		~GraphicDevice_DX11();

#pragma region DirectX11 Member Variable
	private:
		// GPU 객체 생성
		ID3D11Device* mDevice;
		// GPU Read Write
		ID3D11DeviceContext* mDeviceContext;
		ID3D11Texture2D* mRenderTarget;
		ID3D11RenderTargetView* mRenderTargetView;

		ID3D11Texture2D* mDepthStencilBuffer;
		ID3D11DepthStencilView* mDepthStencilView;

		IDXGISwapChain* mSwapChain;
		ID3D11SamplerState* mSampler;
#pragma endregion

	};
}

