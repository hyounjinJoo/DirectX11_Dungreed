#pragma once

#include "hjGraphics.h"
#include "hjResource.h"

namespace hj
{
	class Mesh : public Resource
	{
	public:
		Mesh();
		virtual ~Mesh();

#pragma region Overrided Function
		virtual HRESULT Load(const std::wstring& path) override;
#pragma endregion

#pragma region Member Function
		bool CreateVertexBuffer(void* data, UINT count);
		bool CreateIndexBuffer(void* data, UINT count);
		void BindBuffer();
		void Render();
#pragma endregion

	private:
#pragma region Member Variables
		Microsoft::WRL::ComPtr<ID3D11Buffer> mVertexBuffer;
		D3D11_BUFFER_DESC mVBDesc;

		Microsoft::WRL::ComPtr<ID3D11Buffer> mIndexBuffer;
		D3D11_BUFFER_DESC mIBDesc;
		UINT mIndexCount;
#pragma endregion

	};
}
