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
		// �������� ���� ��ġ���� ������Ʈ.

		// 1. ���� ��� ����
		// - ũ�� ��ȯ ���
		Matrix scale = Matrix::CreateScale(mScale);
		mWorldScale = mScale;

		// - ȸ�� ��ȯ ���
		Matrix rotation;
		rotation = Matrix::CreateFromYawPitchRoll(mRotation.y, mRotation.x, mRotation.z);
		mWorldRotation = mRotation;

		// - �̵� ��ȯ ���
		Matrix position;
		position.Translation(mPosition);
		mWorldPosition = mPosition;

		Matrix localTransform = scale * rotation * position;
		
		// - ���� ����� ���� ���� ���, ����� ó������ ���� ��쿡 ����� localTransform ����� ���� ����.
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

			// - ���� ��ǥ, ũ��, ȸ�� ����
			Vector3 worldPos, worldScale;
			Quaternion worldRot;
			combinedTransform.Decompose(worldScale, worldRot, worldPos);
			mWorldPosition = worldPos;
			Vector3 quatToRadRot = worldRot.ToEuler();
			mWorldRotation = quatToRadRot;
			mWorldScale = worldScale;
		}

		// - ���� ��� ����
		mWorld = combinedTransform;

		// - ���� ���� ����
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
