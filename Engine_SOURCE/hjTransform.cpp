#include "hjTransform.h"
#include "hjRenderer.h"
#include "hjCamera.h"

namespace hj
{
	Transform::Transform()
		: Component(eComponentType::Transform)
		, mRelativeForward(Vector3::Forward)
		, mRelativeRight(Vector3::Right)
		, mRelativeUp(Vector3::Up)
		, mWorldForward(Vector3::Forward)
		, mWorldRight(Vector3::Right)
		, mWorldUp(Vector3::Up)
		, mRelativeScale(Vector3::One)
		, mRelativeRotation(Vector3::Zero)
		, mRelativePosition(Vector3::One)
		, mParent(nullptr)
		, mInheritParentScale(false)
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
		// 렌더링에 사용될 위치값을 업데이트.
		// 1. 월드 행렬 생성
		// - 크기 변환 행렬
		Matrix scale = Matrix::CreateScale(mRelativeScale);
		mWorldScale = mRelativeScale;

		// - 회전 변환 행렬
		Matrix rotation;
		rotation = Matrix::CreateRotationX(mRelativeRotation.x);
		rotation *= Matrix::CreateRotationY(mRelativeRotation.y);
		rotation *= Matrix::CreateRotationZ(mRelativeRotation.z);
		mWorldRotation = mRelativeRotation;

		// - 이동 변환 행렬
		Matrix position;
		position.Translation(mRelativePosition);
		mWorldPosition = mRelativePosition;

		mWorld = scale * rotation * position;
		mWorldForward = mRelativeForward = XMVector3TransformNormal(Vector3::Forward, rotation);
		mWorldRight = mRelativeRight = XMVector3TransformNormal(Vector3::Right, rotation);
		mWorldUp = mRelativeUp = XMVector3TransformNormal(Vector3::Up, rotation);

		if (mParent)
		{
			Matrix parentWorld = mParent->GetWorldMatrix();

			if (!mInheritParentScale)
			{
				Vector3 worldPos, worldScale;
				Quaternion worldRot;
				parentWorld.Decompose(worldScale, worldRot, worldPos);
				Matrix parentWorldNoScale = Matrix::CreateScale(1.f) * Matrix::CreateFromQuaternion(worldRot) * Matrix::CreateTranslation(worldPos);
				mWorld = mWorld * parentWorldNoScale;
			}
			else
			{
				mWorld *= parentWorld;
			}

			// - 월드 좌표, 크기, 회전 갱신
			Vector3 worldPos, worldScale;
			Quaternion worldRot;
			mWorld.Decompose(worldScale, worldRot, worldPos);
			mWorldPosition = mWorld.Translation();
			Vector3 quatToRadRot = worldRot.ToEuler();
			mWorldRotation = quatToRadRot;
			mWorldScale = worldScale;

			mWorldForward = XMVector3TransformNormal(mRelativeForward, rotation);
			mWorldRight = XMVector3TransformNormal(mRelativeRight, rotation);
			mWorldUp = XMVector3TransformNormal(mRelativeUp, rotation);
			mWorldForward.Normalize();
			mWorldRight.Normalize();
			mWorldUp.Normalize();
		}
	}

	void Transform::Render()
	{
	}

	void Transform::SetConstantBuffer()
	{
		renderer::TransformCB trCb = {};
		trCb.world = mWorld;
		trCb.view = Camera::GetGpuViewMatrix();
		trCb.projection = Camera::GetGpuProjectionMatrix();

		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::Transform];
		cb->Bind(&trCb);
		cb->SetPipeline(eShaderStage::VS);
	}
}
