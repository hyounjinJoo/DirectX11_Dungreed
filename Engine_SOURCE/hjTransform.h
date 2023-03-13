#pragma once

#include "hjComponent.h"

using namespace hj::math;
namespace hj
{
	class Transform : public Component
	{
	public:
		Transform();
		virtual ~Transform();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void SetConstantBuffer();

		void SetParent(Transform* parent) { mParent = parent; }
		Transform* GetParent() { return mParent; }

		Vector3 GetPosition() { return mPosition; }
		float GetPositionX() { return mPosition.x; }
		float GetPositionY() { return mPosition.y; }
		float GetPositionZ() { return mPosition.z; }

		Vector3 GetRotation() { return mRotation; }
		float GetRotationX() { return mRotation.x; }
		float GetRotationY() { return mRotation.y; }
		float GetRotationZ() { return mRotation.z; }

		Vector3 GetScale() { return mScale; }
		float GetScaleX() { return mScale.x; }
		float GetScaleY() { return mScale.y; }
		float GetScaleZ() { return mScale.z; }

		void SetPosition(Vector3 position) { mPosition = position; }
		void SetPositionX(float posX) { mPosition.x = posX; }
		void SetPositionY(float posY) { mPosition.y = posY; }
		void SetPositionZ(float posZ) { mPosition.z = posZ; }

		void SetRotation(Vector3 rotation) { mRotation = rotation; }
		void SetRotationX(float rotX) { mRotation.x = rotX; }
		void SetRotationY(float rotY) { mRotation.y = rotY; }
		void SetRotationZ(float rotZ) { mRotation.z = rotZ; }
		
		void SetScale(Vector3 scale) { mScale = scale; }
		void SetScaleX(float scaleX) { mScale.x = scaleX; }
		void SetScaleY(float scaleY) { mScale.y = scaleY; }
		void SetScaleZ(float scaleZ) { mScale.z = scaleZ; }
		
		Vector3 Forward() { return mForward; }
		Vector3 Right() { return mRight; }
		Vector3 Up() { return mUp; }

		void SetInheritParentTransform(bool inherit) { mInheritParentTransform = inherit; }

		const Matrix& GetWorldMatrix() { return mWorld; }

	private:
		Transform* mParent;

		Vector3 mForward;
		Vector3 mRight;
		Vector3 mUp;
		Vector3 mPosition;
		Vector3 mRotation;
		Vector3 mScale;
		Matrix mWorld;
		bool mInheritParentTransform;
	};
}

