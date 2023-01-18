#pragma once
#include "hjGraphics.h"

namespace hj::graphics
{
	class GraphicDevice_DX11
	{
	public:
		// DirectX11 �ʱ�ȭ�� ����
		GraphicDevice_DX11(ValidationMode validationMode = ValidationMode::Disabled);
		~GraphicDevice_DX11();

#pragma region Create Function
		bool CreateSwapChain(DXGI_SWAP_CHAIN_DESC* desc);
		bool CreateTexture(D3D11_TEXTURE2D_DESC* desc, ID3D11Texture2D** ppTexture2D);

		bool CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* desc, UINT NumElements, const void* byteCode, SIZE_T bytecodeLength, ID3D11InputLayout** ppInputLayout);
		bool CreateBuffer(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* data, ID3D11Buffer** buffer);
		bool CreateShader();

		void BindVertexBuffer(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets);
		void BindIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset);
		void BindViewports(D3D11_VIEWPORT* viewPort);
		void BindConstantBuffer(ID3D11Buffer* buffer, void* data, UINT size);
		void SetConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer);

		void Clear();
		void AdjustViewPorts();

		void Draw();
		void DrawIndexed(UINT indexCount, UINT startIndexLocation, UINT baseVertexLocation);

		void Present();
		void Render();
#pragma endregion

#pragma region Member Variable( DirectX11 Graphic Device )
	private:
		// GPU ��ü(�׷��� ī��� ����Ǵ� �⺻���� ��ü) ����
		Microsoft::WRL::ComPtr <ID3D11Device>			mDevice;
		// GPU Read Write �뵵. ����̽��� ���� �������� �ʰ� �� ��ü�� ���ؼ� GPU�� ����� ������.
		Microsoft::WRL::ComPtr <ID3D11DeviceContext>	mContext;

		// ���������� �׷����� ��ȭ�� ����
		Microsoft::WRL::ComPtr <ID3D11Texture2D>		mRenderTarget;
		Microsoft::WRL::ComPtr <ID3D11RenderTargetView> mRenderTargetView;

		Microsoft::WRL::ComPtr <ID3D11Texture2D>		mDepthStencilBuffer;
		Microsoft::WRL::ComPtr <ID3D11DepthStencilView> mDepthStencilView;

		// ȭ�鿡 ���������� �׷����� �����(== Frame Buffer)�� �����ϰ�,
		// ������ ȭ�鿡 �������ϴ� ������ ����ϴ� ��ü
		Microsoft::WRL::ComPtr <IDXGISwapChain>			mSwapChain;

		// �ؽ�ó�� �ε��� �� ���ȴ�.
		Microsoft::WRL::ComPtr <ID3D11SamplerState>		mSampler;

		// ����� ����Ʈ
		D3D11_VIEWPORT mViewPort;
#pragma endregion

	};

	inline GraphicDevice_DX11*& GetDevice()
	{
		static GraphicDevice_DX11* device = nullptr;
		return device;
	}
}

