#include "hjMaterial.h"

namespace hj::graphics
{
	Material::Material()
		: Resource(eResourceType::Material)
		, mMode(eRenderingMode::Opaque)
	{
	}

	Material::~Material()
	{
	}

	HRESULT Material::Load(const std::wstring& path)
	{

		return S_OK;
	}

	void Material::SetData(eGPUParam param, void* data)
	{
		switch (param)
		{
		case hj::graphics::eGPUParam::Int_1:
			mCB.iData1 = *static_cast<int*>(data);
			break;
		case hj::graphics::eGPUParam::Int_2:
			mCB.iData2 = *static_cast<int*>(data);
			break;
		case hj::graphics::eGPUParam::Int_3:
			mCB.iData3 = *static_cast<int*>(data);
			break;
		case hj::graphics::eGPUParam::Int_4:
			mCB.iData4 = *static_cast<int*>(data);
			break;
		case hj::graphics::eGPUParam::Float_1:
			mCB.fData1 = *static_cast<float*>(data);
			break;
		case hj::graphics::eGPUParam::Float_2:
			mCB.fData2 = *static_cast<float*>(data);
			break;
		case hj::graphics::eGPUParam::Float_3:
			mCB.fData3 = *static_cast<float*>(data);
			break;
		case hj::graphics::eGPUParam::Float_4:
			mCB.fData4 = *static_cast<float*>(data);
			break;
		case hj::graphics::eGPUParam::Vector2_1:
			mCB.xy1 = *static_cast<Vector2*>(data);
			break;
		case hj::graphics::eGPUParam::Vector2_2:
			mCB.xy2 = *static_cast<Vector2*>(data);
			break;
		case hj::graphics::eGPUParam::Vector2_3:
			mCB.xy3 = *static_cast<Vector2*>(data);
			break;
		case hj::graphics::eGPUParam::Vector2_4:
			mCB.xy4 = *static_cast<Vector2*>(data);
			break;
		case hj::graphics::eGPUParam::Vector3_1:
			mCB.xyz1 = *static_cast<Vector3*>(data);
			break;
		case hj::graphics::eGPUParam::Vector3_2:
			mCB.xyz2 = *static_cast<Vector3*>(data);
			break;
		case hj::graphics::eGPUParam::Vector3_3:
			mCB.xyz3 = *static_cast<Vector3*>(data);
			break;
		case hj::graphics::eGPUParam::Vector3_4:
			mCB.xyz4 = *static_cast<Vector3*>(data);
			break;
		case hj::graphics::eGPUParam::Vector4_1:
			mCB.xyzw1 = *static_cast<Vector4*>(data);
			break;
		case hj::graphics::eGPUParam::Vector4_2:
			mCB.xyzw2 = *static_cast<Vector4*>(data);
			break;
		case hj::graphics::eGPUParam::Vector4_3:
			mCB.xyzw3 = *static_cast<Vector4*>(data);
			break;
		case hj::graphics::eGPUParam::Vector4_4:
			mCB.xyzw4 = *static_cast<Vector4*>(data);
			break;
		case hj::graphics::eGPUParam::Matrix_1:
			mCB.matrix1 = *static_cast<Matrix*>(data);
			break;
		case hj::graphics::eGPUParam::Matrix_2:
			mCB.matrix2 = *static_cast<Matrix*>(data);
			break;
		case hj::graphics::eGPUParam::Matrix_3:
			mCB.matrix3 = *static_cast<Matrix*>(data);
			break;
		case hj::graphics::eGPUParam::Matrix_4:
			mCB.matrix4 = *static_cast<Matrix*>(data);
			break;
		default:
			break;
		}
	}

	void Material::Bind()
	{
		if(mTexture)
			mTexture->BindShader(eShaderStage::PS, 0);

		ConstantBuffer* pCB = renderer::constantBuffers[(UINT)eCBType::Material];
		pCB->SetData(&mCB);
		pCB->Bind(eShaderStage::VS);
		pCB->Bind(eShaderStage::PS);

		mShader->Binds();
	}
	void Material::Clear()
	{
		mTexture->Clear(0);

		ConstantBuffer* pCB = renderer::constantBuffers[(UINT)eCBType::Material];		
		pCB->Clear();
	}
}

