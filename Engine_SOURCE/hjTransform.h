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
		Vector2 GetPositionXY() { return Vector2(mPosition.x, mPosition.y); }

		Vector3 GetRotation() { return mRotation; }
		float GetRotationX() { return mRotation.x; }
		float GetRotationY() { return mRotation.y; }
		float GetRotationZ() { return mRotation.z; }
		Vector2 GetRotationXY() { return Vector2(mRotation.x, mRotation.y); }

		Vector3 GetScale() { return mScale; }
		float GetScaleX() { return mScale.x; }
		float GetScaleY() { return mScale.y; }
		float GetScaleZ() { return mScale.z; }
		Vector2 GetScaleXY() { return Vector2(mScale.x, mScale.y); }

		void SetPosition(const Vector3& position) { mPosition = position; }
		void SetPositionX(const float posX) { mPosition.x = posX; }
		void SetPositionY(const float posY) { mPosition.y = posY; }
		void SetPositionZ(const float posZ) { mPosition.z = posZ; }
		void SetPositionXY(const Vector2& position) { mPosition.x = position.x; mPosition.y = position.y; }

		void SetRotation(const Vector3& rotation) { mRotation = rotation; }
		void SetRotationX(const float rotX) { mRotation.x = rotX; }
		void SetRotationY(const float rotY) { mRotation.y = rotY; }
		void SetRotationZ(const float rotZ) { mRotation.z = rotZ; }
		void SetRotationXY(const Vector2& rotation) { mRotation.x = rotation.x; mRotation.y = rotation.y; }
		
		void SetScale(const Vector3& scale) { mScale = scale; }
		void SetScaleX(const float scaleX) { mScale.x = scaleX; }
		void SetScaleY(const float scaleY) { mScale.y = scaleY; }
		void SetScaleZ(const float scaleZ) { mScale.z = scaleZ; }
		void SetScaleXY(const Vector2& scale) { mScale.x = scale.x; mScale.y = scale.y; }
		
		void AddPosition(const Vector3& position) { mPosition += position; }
		void AddPositionX(const float posX) { mPosition.x += posX; }
		void AddPositionY(const float posY) { mPosition.y += posY; }
		void AddPositionZ(const float posZ) { mPosition.z += posZ; }
		void AddPositionXY(const Vector2& position) { mPosition.x += position.x; mPosition.y += position.y; }

		void AddRotation(const Vector3& rotation) { mRotation += rotation; }
		void AddRotationX(const float rotX) { mRotation.x += rotX; }
		void AddRotationY(const float rotY) { mRotation.y += rotY; }
		void AddRotationZ(const float rotZ) { mRotation.z += rotZ; }

		void AddScale(const Vector3& scale) { mScale += scale; }
		void AddScaleX(const float scaleX) { mScale.x += scaleX; }
		void AddScaleY(const float scaleY) { mScale.y += scaleY; }
		void AddScaleZ(const float scaleZ) { mScale.z += scaleZ; }

		Vector3 Forward() { return mForward; }
		Vector3 Right() { return mRight; }
		Vector3 Up() { return mUp; }

		void SetInheritParentPosition(bool inherit) { mInheritParentPosition = inherit; }
		void SetInheritParentRotation(bool inherit) { mInheritParentRotation = inherit; }
		void SetInheritParentScale(bool inherit) { mInheritParentScale = inherit; }

		const Matrix& GetWorldMatrix() { return mWorld; }

		const Vector3& GetWorldPosition() { return mWorldPosition; }
		const Vector3& GetWorldRotation() { return mWorldRotation; }
		const Vector3& GetWorldScale() { return mWorldScale; }

		float GetWorldPositionX() { return mWorldPosition.x; }
		float GetWorldPositionY() { return mWorldPosition.y; }
		float GetWorldPositionZ() { return mWorldPosition.z; }
		float GetWorldRotationX() { return mWorldRotation.x; }
		float GetWorldRotationY() { return mWorldRotation.y; }
		float GetWorldRotationZ() { return mWorldRotation.z; }
		float GetWorldScaleX() { return mWorldScale.x; }
		float GetWorldScaleY() { return mWorldScale.y; }
		float GetWorldScaleZ() { return mWorldScale.z; }

	private:
		Transform* mParent;

		Vector3 mForward;
		Vector3 mRight;
		Vector3 mUp;
		Vector3 mPosition;
		Vector3 mRotation;
		Vector3 mScale;
		Matrix mWorld;
		bool mInheritParentScale;
		bool mInheritParentRotation;
		bool mInheritParentPosition;

		Vector3 mWorldPosition;
		Vector3 mWorldRotation;
		Vector3 mWorldScale;
	};
}

