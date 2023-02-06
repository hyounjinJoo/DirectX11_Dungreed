#include "hjTransform.h"
#include "hjRenderer.h"

namespace hj
{
	Transform::Transform()
		: Component(eComponentType::Transform)
	{
	}

	Transform::~Transform()
	{
	}

	void Transform::Initialize()
	{
	}

	void Transform::Update()
	{
	}

	void Transform::FixedUpdate()
	{
		SetConstantBuffer();
	}

	void Transform::Render()
	{
	}

	void Transform::SetConstantBuffer()
	{
		// ��� ���۸� �����ͼ� �ش� ������ۿ�
		// SetConstantBuffer(eShaderStage::VS, eCBType::Transform, renderer::triangleConstantBuffer.Get());
		// ����ó�� ���� �������־�� �Ѵ�.	
		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::Transform];

		Vector4 pos(mPosition.x, mPosition.y, mPosition.z, 0.f);
		cb->Bind(&pos);
		cb->SetPipeline(eShaderStage::VS);
	}
}
