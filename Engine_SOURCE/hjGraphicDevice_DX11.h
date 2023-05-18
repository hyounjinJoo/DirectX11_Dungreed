#pragma once
#include "hjGraphics.h"

using namespace hj::enums;
namespace hj::graphics
{
	class GraphicDevice_DX11
	{
	public:
		// DirectX11 �ʱ�ȭ�� ����
		GraphicDevice_DX11(ValidationMode validationMode = ValidationMode::Disabled);
		~GraphicDevice_DX11();

#pragma region Member Function
	#pragma region Create Function
		bool CreateSwapChain(DXGI_SWAP_CHAIN_DESC* desc);
		bool CreateTexture(D3D11_TEXTURE2D_DESC* desc, ID3D11Texture2D** ppTexture2D);

		bool CreateInputLayout(D3D11_INPUT_ELEMENT_DESC* desc, UINT NumElements, const void* byteCode, SIZE_T bytecodeLength, ID3D11InputLayout** ppInputLayout);
		bool CreateBuffer(D3D11_BUFFER_DESC* desc, D3D11_SUBRESOURCE_DATA* data, ID3D11Buffer** buffer);
		bool CreateRenderTargetView(ID3D11Resource* pResource, const D3D11_RENDER_TARGET_VIEW_DESC* pDesc, ID3D11RenderTargetView** ppRTView);
		bool CreateUnorderedAccessView(ID3D11Resource* pResource, const D3D11_UNORDERED_ACCESS_VIEW_DESC* pDesc, ID3D11UnorderedAccessView** ppUAView);
		bool CreateDepthStencilView(ID3D11Resource* pResource, const D3D11_DEPTH_STENCIL_VIEW_DESC* pDesc, ID3D11DepthStencilView** ppDSView);
		bool CreateShaderResourceView(ID3D11Resource* pResource, const D3D11_SHADER_RESOURCE_VIEW_DESC* pDesc, ID3D11ShaderResourceView** ppSRView);
		bool CreateVertexShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11VertexShader** ppVertexShader);
		bool CreateGeometryShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11GeometryShader** ppVertexShader);
		bool CreatePixelShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11PixelShader** ppPixelShader);
		bool CreateComputeShader(const void* pShaderBytecode, SIZE_T BytecodeLength, ID3D11ClassLinkage* pClassLinkage, ID3D11ComputeShader** ppComputeShader);
		bool CreateSamplerState(const D3D11_SAMPLER_DESC* pSamplerDesc, ID3D11SamplerState** ppSamplerState);
		bool CreateRasterizerState(const D3D11_RASTERIZER_DESC* pRasterizerDesc, ID3D11RasterizerState** ppRasterizerState);
		bool CreateDepthStencilState(const D3D11_DEPTH_STENCIL_DESC* pDepthStencilDesc, ID3D11DepthStencilState** ppDepthStencilState);
		bool CreateBlendState(const D3D11_BLEND_DESC* pBlendStateDesc, ID3D11BlendState** ppBlendState);
	#pragma endregion 
		
	#pragma region Bind Function
		void BindPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY topology);
		void BindInputLayout(ID3D11InputLayout* pInputLayout);
		void BindVertexBuffer(UINT StartSlot, UINT NumBuffers, ID3D11Buffer* const* ppVertexBuffers, const UINT* pStrides, const UINT* pOffsets);
		void BindIndexBuffer(ID3D11Buffer* pIndexBuffer, DXGI_FORMAT Format, UINT Offset);
		void BindVertexShader(ID3D11VertexShader* pVertexShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances);
		void BindHullShader(ID3D11HullShader* pHullShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances);
		void BindDomainShader(ID3D11DomainShader* pDomainShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances);
		void BindGeometryShader(ID3D11GeometryShader* pGeometryShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances);
		void BindPixelShader(ID3D11PixelShader* pPixelShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances);
		void BindComputeShader(ID3D11ComputeShader* pComputeShader, ID3D11ClassInstance* const* ppClassInstances, UINT NumClassInstances);
		void Dispatch(UINT ThreadGroupCountX, UINT ThreadGroupCountY, UINT ThreadGroupCountZ);
		void BindViewports(D3D11_VIEWPORT* viewPort);
		void SetData(ID3D11Buffer* buffer, void* data, UINT size);
		void GetData(ID3D11Buffer* buffer, void* data, UINT size);
		void ClearConstantBuffer(ID3D11Buffer* buffer, UINT size);
		void BindConstantBuffer(eShaderStage stage, eCBType type, ID3D11Buffer* buffer);
		void BindShaderResource(eShaderStage stage, UINT slot, ID3D11ShaderResourceView* const* ppShaderResourceViews);
		void BindUnorderedAccessView(UINT startSlot, UINT NumUAVs, ID3D11UnorderedAccessView* const* ppUnorderedAccessViews, const UINT* pUAVInitialCounts);
		void BindSamplers(eShaderStage stage, UINT slot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers);
		void BindsSamplers(UINT slot, UINT NumSamplers, ID3D11SamplerState* const* ppSamplers);
		void BindRasterizerState(ID3D11RasterizerState* pRasterizerState);
		void BindDepthStencilState(ID3D11DepthStencilState* pDepthStencilState);
		void BindBlendState(ID3D11BlendState* pBlendState);
		void CopyResource(ID3D11Resource* pDstResource, ID3D11Resource* pSrcResource);
	#pragma endregion

	#pragma region Render Function
		void Clear();
		void AdjustViewPorts();

		void Draw();
		void DrawIndexed(UINT indexCount, UINT startIndexLocation, UINT baseVertexLocation);
		void DrawIndexedInstanced(UINT indexCountPerInstance, UINT instanceCount, UINT startIndexLocation, UINT baseVertexLocation, UINT startInstanceLocation);

		void Present();
	#pragma endregion

	#pragma region Getter
		ID3D11Device* GetID3D11Device() { return mDevice.Get(); }
		UINT GetSwapChainNumerator() { return mSwapChainNumerator; }
	#pragma endregion
#pragma endregion

#pragma region Member Variable( DirectX11 Graphic Device )
	private:
		Microsoft::WRL::ComPtr <ID3D11Device>			mDevice;
		Microsoft::WRL::ComPtr <ID3D11DeviceContext>	mContext;

		std::shared_ptr <class Texture>		mRenderTargetTexture;
		std::shared_ptr <class Texture>		mDepthStencilTexture;
		Microsoft::WRL::ComPtr <IDXGISwapChain>			mSwapChain;

		D3D11_VIEWPORT mViewPort;
#pragma endregion

		UINT mSwapChainNumerator;
	};

	inline GraphicDevice_DX11*& GetDevice()
	{
		static GraphicDevice_DX11* device = nullptr;
		return device;
	}
}

