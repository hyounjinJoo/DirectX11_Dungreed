#include "hjTransform.h"
#include "hjRenderer.h"
#include "hjCamera.h"

namespace hj
{
	Transform::Transform()
		: Component(eComponentType::Transform)
		, mForward(Vector3::Forward)
		, mRight(Vector3::Right)
		, mUp(Vector3::Up)
		, mScale(Vector3::One)
		, mRotation(Vector3::Zero)
		, mPosition(Vector3::One)
		, mParent(nullptr)
		, mInheritParentScale(false)
		, mInheritParentRotation(false)
		, mInheritParentPosition(false)
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
		Matrix scale = Matrix::CreateScale(mScale);
		mWorldScale = mScale;

		// - 회전 변환 행렬
		Matrix rotation;
		rotation = Matrix::CreateFromYawPitchRoll(mRotation.y, mRotation.x, mRotation.z);
		mWorldRotation = mRotation;

		// - 이동 변환 행렬
		Matrix position;
		position.Translation(mPosition);
		mWorldPosition = mPosition;

		Matrix localTransform = scale * rotation * position;
		
		// - 최종 계산을 거쳐 사용될 행렬, 상속을 처리하지 않을 경우에 대비해 localTransform 행렬을 먼저 대입.
		Matrix combinedTransform = localTransform;

		if (mParent)
		{
			Matrix parentTransform = Matrix::Identity;

			if (mInheritParentScale)
			{
				parentTransform *= Matrix::CreateScale(mParent->mWorldScale);
			}

			if (mInheritParentRotation)
			{
				parentTransform *= Matrix::CreateFromQuaternion(Quaternion::CreateFromYawPitchRoll(mParent->mWorldRotation.y, mParent->mWorldRotation.x, mParent->mWorldRotation.z));
			}

			if (mInheritParentPosition)
			{
				Matrix parentRotation = Matrix::CreateWorld(mParent->mWorldPosition, mParent->mUp, mParent->mForward);
				Vector3 parentPos = mParent->mWorldPosition;
				memcpy(&parentRotation._41, &parentPos, sizeof(Vector3));
				
				const XMFLOAT3* convertPos = (XMFLOAT3*)&mWorldPosition;
				Vector3 worldPos = XMVector3TransformCoord(XMLoadFloat3(convertPos), parentRotation);
				mWorldPosition = worldPos;
				
				memcpy(&combinedTransform._41, &worldPos, sizeof(Vector3));
			}

			combinedTransform *= parentTransform;

			// - 월드 좌표, 크기, 회전 갱신
			Vector3 worldPos, worldScale;
			Quaternion worldRot;
			combinedTransform.Decompose(worldScale, worldRot, worldPos);
			mWorldPosition = worldPos;
			Vector3 quatToRadRot = worldRot.ToEuler();
			mWorldRotation = quatToRadRot;
			mWorldScale = worldScale;
		}

		// - 월드 행렬 갱신
		mWorld = combinedTransform;

		// - 기저 벡터 갱신
		if (nullptr != mParent && (mInheritParentRotation || mInheritParentPosition))
		{
			Matrix rotation = Matrix::CreateFromYawPitchRoll(mWorldRotation.y, mWorldRotation.x, mWorldRotation.z);

			mForward = Vector3::TransformNormal(Vector3::Forward, rotation) + Vector3(0.f, 0.f, mWorldPosition.z);
			mRight = Vector3::TransformNormal(Vector3::Right, rotation) + Vector3(mWorldPosition.x, 0.f, 0.f);
			mUp = Vector3::TransformNormal(Vector3::Up, rotation) + Vector3(0.f, mWorldPosition.y, 0.f);
		}
		else
		{
			mForward = Vector3::TransformNormal(Vector3::Forward, rotation);
			mRight = Vector3::TransformNormal(Vector3::Right, rotation);
			mUp = Vector3::TransformNormal(Vector3::Up, rotation);
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
