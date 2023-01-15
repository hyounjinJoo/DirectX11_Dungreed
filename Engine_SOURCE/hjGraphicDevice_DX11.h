#pragma once
#include "hjGraphics.h"

namespace hj::graphics
{
	class GraphicDevice_DX11
	{
	public:
		// DirectX11 초기화를 수행
		GraphicDevice_DX11(ValidationMode validationMode = ValidationMode::Disabled);
		~GraphicDevice_DX11();

#pragma region Create Function
		bool CreateSwapChain(DXGI_SWAP_CHAIN_DESC* desc);
		bool CreateTexture(D3D11_TEXTURE2D_DESC* desc, ID3D11Texture2D** ppTexture2D);
		bool CreateBuffer(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* data, ID3D11Buffer** buffer);
		bool CreateShader();

		void Draw();
#pragma endregion

#pragma region Member Variable( DirectX11 Graphic Device )
	private:
		// GPU 객체(그래픽 카드와 연결되는 기본적인 객체) 생성
		Microsoft::WRL::ComPtr <ID3D11Device>			mDevice;
		// GPU Read Write 용도. 디바이스에 직접 접근하지 않고 이 객체를 통해서 GPU에 명령을 내린다.
		Microsoft::WRL::ComPtr <ID3D11DeviceContext>	mContext;

		// 최종적으로 그려지는 도화지 역할
		Microsoft::WRL::ComPtr <ID3D11Texture2D>		mRenderTarget;
		Microsoft::WRL::ComPtr <ID3D11RenderTargetView> mRenderTargetView;

		Microsoft::WRL::ComPtr <ID3D11Texture2D>		mDepthStencilBuffer;
		Microsoft::WRL::ComPtr <ID3D11DepthStencilView> mDepthStencilView;

		// 화면에 최종적으로 그려지는 백버퍼(== Frame Buffer)를 관리하고,
		// 실제로 화면에 렌더링하는 역할을 담당하는 객체
		Microsoft::WRL::ComPtr <IDXGISwapChain>			mSwapChain;

		// 텍스처를 로딩할 때 사용된다.
		Microsoft::WRL::ComPtr <ID3D11SamplerState>		mSampler;
#pragma endregion

	};

	inline GraphicDevice_DX11*& GetDevice()
	{
		static GraphicDevice_DX11* device = nullptr;
		return device;
	}
}

