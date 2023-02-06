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
		// 상수 버퍼를 가져와서 해당 상수버퍼에
		// SetConstantBuffer(eShaderStage::VS, eCBType::Transform, renderer::triangleConstantBuffer.Get());
		// 예시처럼 값을 세팅해주어야 한다.	
		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::Transform];

		Vector4 pos(mPosition.x, mPosition.y, mPosition.z, 0.f);
		cb->Bind(&pos);
		cb->SetPipeline(eShaderStage::VS);
	}
}
