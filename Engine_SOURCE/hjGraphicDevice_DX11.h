#pragma once
#include "hjGraphics.h"

// ���̷�ƮX11�� ����ϱ� ���� ���
#include <d3d11.h>
// ���̷�ƮX11�� ����� ���� ������ ���ִ� �����Ϸ� ���
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
		// GPU ��ü ����
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

