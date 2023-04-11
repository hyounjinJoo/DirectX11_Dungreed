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

		Vector3 GetPosition() const { return mRelativePosition; }
		float GetPositionX() const { return mRelativePosition.x; }
		float GetPositionY() const { return mRelativePosition.y; }
		float GetPositionZ() const { return mRelativePosition.z; }
		Vector2 GetPositionXY() const { return Vector2(mRelativePosition.x, mRelativePosition.y); }

		Vector3 GetRotation() const { return mRelativeRotation; }
		float GetRotationX() const { return mRelativeRotation.x; }
		float GetRotationY() const { return mRelativeRotation.y; }
		float GetRotationZ() const { return mRelativeRotation.z; }
		Vector2 GetRotationXY() const { return Vector2(mRelativeRotation.x, mRelativeRotation.y); }

		Vector3 GetScale() const { return mRelativeScale; }
		float GetScaleX() const { return mRelativeScale.x; }
		float GetScaleY() const { return mRelativeScale.y; }
		float GetScaleZ() const { return mRelativeScale.z; }
		Vector2 GetScaleXY() const { return Vector2(mRelativeScale.x, mRelativeScale.y); }

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

		const Vector3& Forward() const { return mRelativeForward; }
		const Vector3& Right() const { return mRelativeRight; }
		const Vector3& Up() const { return mRelativeUp; }
		const Vector3& WorldForward() const { return mWorldForward; }
		const Vector3& WorldRight() const { return mWorldRight; }
		const Vector3& WorldUp() const { return mWorldUp; }

		void SetInheritParentScale(bool inherit) { mInheritParentScale = inherit; }

		const Matrix& GetWorldMatrix() const { return mWorld; }

		const Vector3& GetWorldPosition() const { return mWorldPosition; }
		const Vector3& GetWorldRotation() const { return mWorldRotation; }
		const Vector3& GetWorldScale() const { return mWorldScale; }

		float GetWorldPositionX() const { return mWorldPosition.x; }
		float GetWorldPositionY() const { return mWorldPosition.y; }
		float GetWorldPositionZ() const { return mWorldPosition.z; }
		Vector2 GetWorldPositionXY() const { return Vector2(mWorldPosition.x, mWorldPosition.y); }
		float GetWorldRotationX() const { return mWorldRotation.x; }
		float GetWorldRotationY() const { return mWorldRotation.y; }
		float GetWorldRotationZ() const { return mWorldRotation.z; }
		float GetWorldScaleX() const { return mWorldScale.x; }
		float GetWorldScaleY() const { return mWorldScale.y; }
		float GetWorldScaleZ() const { return mWorldScale.z; }

		Vector2 GetWorldCenterUp() const { return Vector2(mWorldPosition.x, mWorldPosition.y + mWorldScale.y * 0.5f); }
		Vector2 GetWorldCenterBottom() const { return Vector2(mWorldPosition.x, mWorldPosition.y - mWorldScale.y * 0.5f); }
		Vector2 GetWorldLeftUp() const { return Vector2(mWorldPosition.x - mWorldScale.x * 0.5f, mWorldPosition.y + mWorldScale.y * 0.5f); }
		Vector2 GetWorldLeftCenter() const { return Vector2(mWorldPosition.x - mWorldScale.x * 0.5f, mWorldPosition.y); }
		Vector2 GetWorldLeftBottom() const { return Vector2(mWorldPosition.x - mWorldScale.x * 0.5f, mWorldPosition.y - mWorldScale.y * 0.5f); }
		Vector2 GetWorldRightUp() const { return Vector2(mWorldPosition.x + mWorldScale.x * 0.5f, mWorldPosition.y + mWorldScale.y * 0.5f); }
		Vector2 GetWorldRightCenter() const { return Vector2(mWorldPosition.x + mWorldScale.x * 0.5f, mWorldPosition.y); }
		Vector2 GetWorldRightBottom() const { return Vector2(mWorldPosition.x + mWorldScale.x * 0.5f, mWorldPosition.y - mWorldScale.y * 0.5f); }

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

