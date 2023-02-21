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
		Vector3 GetRotation() { return mRotation; }
		Vector3 GetScale() { return mScale; }

		void SetPosition(Vector3 position) { mPosition = position; }
		void SetRotation(Vector3 rotation) { mRotation = rotation; }
		void SetScale(Vector3 scale) { mScale = scale; }
		
		Vector3 Forward() { return mForward; }
		Vector3 Right() { return mRight; }
		Vector3 Up() { return mUp; }

		void SetInheritParentTransform(bool inherit) { mInheritParentTransform = inherit; }

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

