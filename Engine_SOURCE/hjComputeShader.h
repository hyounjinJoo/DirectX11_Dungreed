#pragma once
#include "hjGraphics.h"
#include "hjResource.h"

namespace hj::graphics
{
    class ComputeShader :
        public Resource
    {
	public:
		ComputeShader(UINT threadGroupX, UINT threadGroupY, UINT threadGroupZ);
        ComputeShader();
        virtual ~ComputeShader();

        HRESULT Load(const std::wstring& path) override;

        bool Create(const std::wstring& file, const std::string& funcName);
        void OnExecute();

        virtual void Binds();
        virtual void Clear();

    private:
		inline void PrintErrorBlobMessage();
		inline void ReleaseErrorBlob();

    protected:
        Microsoft::WRL::ComPtr<ID3DBlob> mCSBlob;
        Microsoft::WRL::ComPtr<ID3D11ComputeShader> mCS;
		Microsoft::WRL::ComPtr<ID3DBlob> mErrorBlob;

		UINT mThreadGroupCountX;
		UINT mThreadGroupCountY;
		UINT mThreadGroupCountZ;

		UINT mGroupX;
		UINT mGroupY;
		UINT mGroupZ;
    };
}