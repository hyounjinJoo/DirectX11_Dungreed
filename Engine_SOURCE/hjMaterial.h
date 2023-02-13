#pragma once

#include "hjResource.h"
#include "hjShader.h"
#include "hjRenderer.h"

using namespace hj::renderer;
namespace hj::graphics
{
	class Material : public Resource
	{
	public:
		Material();
		virtual ~Material();

		virtual HRESULT Load(const std::wstring& path) override;
		
		void SetData(eGPUParam param, void* data);
		void Bind();

		void SetShader(Shader* shader) { mShader = shader; }
		Shader* GetShader() { return mShader; }

	private:
		Shader* mShader;
		MaterialCB mCB;
	};
}