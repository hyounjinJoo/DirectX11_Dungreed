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

		Vector3 GetPosition() { return mRelativePosition; }
		float GetPositionX() { return mRelativePosition.x; }
		float GetPositionY() { return mRelativePosition.y; }
		float GetPositionZ() { return mRelativePosition.z; }
		Vector2 GetPositionXY() { return Vector2(mRelativePosition.x, mRelativePosition.y); }

		Vector3 GetRotation() { return mRelativeRotation; }
		float GetRotationX() { return mRelativeRotation.x; }
		float GetRotationY() { return mRelativeRotation.y; }
		float GetRotationZ() { return mRelativeRotation.z; }
		Vector2 GetRotationXY() { return Vector2(mRelativeRotation.x, mRelativeRotation.y); }

		Vector3 GetScale() { return mRelativeScale; }
		float GetScaleX() { return mRelativeScale.x; }
		float GetScaleY() { return mRelativeScale.y; }
		float GetScaleZ() { return mRelativeScale.z; }
		Vector2 GetScaleXY() { return Vector2(mRelativeScale.x, mRelativeScale.y); }

		void SetPosition(const Vector3& position) { mRelativePosition = position; }
		void SetPositionX(const float posX) { mRelativePosition.x = posX; }
		void SetPositionY(const float posY) { mRelativePosition.y = posY; }
		void SetPositionZ(const float posZ) { mRelativePosition.z = posZ; }
		void SetPositionXY(const Vector2& position) { mRelativePosition.x = position.x; mRelativePosition.y = position.y; }

		void SetRotation(const Vector3& rotation) { mRelativeRotation = rotation; }
		void SetRotationX(const float rotX) { mRelativeRotation.x = rotX; }
		void SetRotationY(const float rotY) { mRelativeRotation.y = rotY; }
		void SetRotationZ(const float rotZ) { mRelativeRotation.z = rotZ; }
		void SetRotationXY(const Vector2& rotation) { mRelativeRotation.x = rotation.x; mRelativeRotation.y = rotation.y; }
		
		void SetScale(const Vector3& scale) { mRelativeScale = scale; }
		void SetScaleX(const float scaleX) { mRelativeScale.x = scaleX; }
		void SetScaleY(const float scaleY) { mRelativeScale.y = scaleY; }
		void SetScaleZ(const float scaleZ) { mRelativeScale.z = scaleZ; }
		void SetScaleXY(const Vector2& scale) { mRelativeScale.x = scale.x; mRelativeScale.y = scale.y; }
		
		void AddPosition(const Vector3& position) { mRelativePosition += position; }
		void AddPositionX(const float posX) { mRelativePosition.x += posX; }
		void AddPositionY(const float posY) { mRelativePosition.y += posY; }
		void AddPositionZ(const float posZ) { mRelativePosition.z += posZ; }
		void AddPositionXY(const Vector2& position) { mRelativePosition.x += position.x; mRelativePosition.y += position.y; }

		void AddRotation(const Vector3& rotation) { mRelativeRotation += rotation; }
		void AddRotationX(const float rotX) { mRelativeRotation.x += rotX; }
		void AddRotationY(const float rotY) { mRelativeRotation.y += rotY; }
		void AddRotationZ(const float rotZ) { mRelativeRotation.z += rotZ; }

		void AddScale(const Vector3& scale) { mRelativeScale += scale; }
		void AddScaleX(const float scaleX) { mRelativeScale.x += scaleX; }
		void AddScaleY(const float scaleY) { mRelativeScale.y += scaleY; }
		void AddScaleZ(const float scaleZ) { mRelativeScale.z += scaleZ; }

		const Vector3& Forward() { return mRelativeForward; }
		const Vector3& Right() { return mRelativeRight; }
		const Vector3& Up() { return mRelativeUp; }
		const Vector3& WorldForward() { return mWorldForward; }
		const Vector3& WorldRight() { return mWorldRight; }
		const Vector3& WorldUp() { return mWorldUp; }

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

		Vector3 mRelativeForward;
		Vector3 mRelativeRight;
		Vector3 mRelativeUp;
		Vector3 mWorldForward;
		Vector3 mWorldRight;
		Vector3 mWorldUp;

		Vector3 mRelativePosition;
		Vector3 mRelativeRotation;
		Vector3 mRelativeScale;
		
		Matrix mWorld;
		
		bool mInheritParentScale;

		Vector3 mWorldPosition;
		Vector3 mWorldRotation;
		Vector3 mWorldScale;
	};
}

