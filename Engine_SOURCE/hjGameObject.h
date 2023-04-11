#pragma once

#include "hjComponent.h"
#include "hjScript.h"
#include "hjEntity.h"
#include "hjTransform.h"

namespace hj
{
	class GameObject : public Entity
	{
	public:
		enum eState
		{
			Active,			// Update 로직 수행
			Paused,			// Update 로직 수행X
			Dead,			// 소멸 대기
		};

		GameObject();
		virtual ~GameObject();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

		template <typename T>
		T* AddComponent()
		{
			T* comp = new T();
			eComponentType order = comp->GetOrder();

			if (order != eComponentType::Script)
			{
				mComponents[(UINT)order] = comp;
				mComponents[(UINT)order]->SetOwner(this);
			}
			else
			{
				mScripts.push_back(dynamic_cast<Script*>(comp));
				comp->SetOwner(this);
			}

			return comp;
		}

		void AddComponent(Component* comp);

		template <typename T>
		T* GetComponent()
		{
			T* comp;
			for (auto c : mComponents)
			{
				comp = dynamic_cast<T*>(c);
				
				if (comp != nullptr)
				{
					return comp;
				}
			}

			return nullptr;
		}

		template <typename T>
		std::vector<T*> GetComponents()
		{
			std::vector<T*> components = {};

			T* comp;
			for (auto c : mComponents)
			{
				comp = dynamic_cast<T*>(c);

				if (comp != nullptr)
					components.push_back(comp);
			}

			return components;
		}

		const std::vector<Script*>& GetScripts() { return mScripts; }

		bool IsDead()
		{
			if (mState == eState::Dead)
				return true;

			return false;
		}

		void Pause() { mState = eState::Paused; }
		void Death() { mState = eState::Dead; }
		eState GetState() { return mState; }

		bool IsDontDestroy() { return mbDontDestroy; }
		void DontDestroy(bool enable) { mbDontDestroy = enable; }
		eLayerType GetLayerType() { return mLayerType; }
		void SetLayerType(eLayerType type) { mLayerType = type; }

	protected:
		std::vector<Component*> mComponents;

	private:
		eState mState;
		eLayerType mLayerType;
		std::vector<Script*> mScripts;
		bool mbDontDestroy;


	public:
		Transform* GetTransform() const
		{ 
			if(mComponents[(UINT)eComponentType::Transform])
				return static_cast<Transform*>(mComponents[(UINT)eComponentType::Transform]); 
			
			return nullptr;
		}
		
		Vector3 GetPosition() const { return GetTransform()->GetPosition(); }
		float GetPositionX() const{ return GetTransform()->GetPositionX(); }
		float GetPositionY() const{ return GetTransform()->GetPositionY(); }
		float GetPositionZ() const{ return GetTransform()->GetPositionZ(); }
		Vector2 GetPositionXY() const { return GetTransform()->GetPositionXY(); }

		Vector3 GetRotation() const { return GetTransform()->GetRotation(); }
		float GetRotationX() const{ return GetTransform()->GetRotationX(); }
		float GetRotationY() const{ return GetTransform()->GetRotationY(); }
		float GetRotationZ() const{ return GetTransform()->GetRotationZ(); }
		Vector2 GetRotationXY() const { return GetTransform()->GetRotationXY(); }
		
		Vector3 GetScale() const { return GetTransform()->GetScale(); }
		float GetScaleX() const{ return GetTransform()->GetScaleX(); }
		float GetScaleY() const{ return GetTransform()->GetScaleY(); }
		float GetScaleZ() const{ return GetTransform()->GetScaleZ(); }
		Vector2 GetScaleXY() const { return GetTransform()->GetScaleXY(); }

		void SetPosition(const Vector3& position) { GetTransform()->SetPosition(position); }
		void SetPositionX(const float posX) { GetTransform()->SetPositionX(posX); }
		void SetPositionY(const float posY) { GetTransform()->SetPositionY(posY); }
		void SetPositionZ(const float posZ) { GetTransform()->SetPositionZ(posZ); }
		void SetPositionXY(const Vector2& position) { GetTransform()->SetPositionXY(position); }

		void SetRotation(const Vector3& rotation) { GetTransform()->SetRotation(rotation); }
		void SetRotationX(const float rotX) { GetTransform()->SetRotationX(rotX); }
		void SetRotationY(const float rotY) { GetTransform()->SetRotationY(rotY); }
		void SetRotationZ(const float rotZ) { GetTransform()->SetRotationZ(rotZ); }
		void SetRotationXY(const Vector2& rotation) { GetTransform()->SetRotationXY(rotation); }

		void SetScale(const Vector3& scale) { GetTransform()->SetScale(scale); }
		void SetScaleX(const float scaleX) { GetTransform()->SetScaleX(scaleX); }
		void SetScaleY(const float scaleY) { GetTransform()->SetScaleY(scaleY); }
		void SetScaleZ(const float scaleZ) { GetTransform()->SetScaleZ(scaleZ); }
		void SetScaleXY(const Vector2& scale) { GetTransform()->SetScaleXY(scale); }

		void AddPosition(const Vector3& position) { GetTransform()->AddPosition(position); }
		void AddPositionX(const float posX) { GetTransform()->AddPositionX(posX); }
		void AddPositionY(const float posY) { GetTransform()->AddPositionY(posY); }
		void AddPositionZ(const float posZ) { GetTransform()->AddPositionZ(posZ); }
		void AddPositionXY(const Vector2& position) { GetTransform()->AddPositionXY(position); }

		void AddRotation(const Vector3& rotation) { GetTransform()->AddRotation(rotation); }
		void AddRotationX(const float rotX) { GetTransform()->AddRotationX(rotX); }
		void AddRotationY(const float rotY) { GetTransform()->AddRotationY(rotY); }
		void AddRotationZ(const float rotZ) { GetTransform()->AddRotationZ(rotZ); }

		void AddScale(const Vector3& scale) { GetTransform()->AddScale(scale); }
		void AddScaleX(const float scaleX) { GetTransform()->AddScaleX(scaleX); }
		void AddScaleY(const float scaleY) { GetTransform()->AddScaleY(scaleY); }
		void AddScaleZ(const float scaleZ) { GetTransform()->AddScaleZ(scaleZ); }

		Vector3 GetWorldPosition() const { return GetTransform()->GetWorldPosition(); }
		Vector3 GetWorldRotation() const { return GetTransform()->GetWorldRotation(); }
		Vector3 GetWorldScale() const { return GetTransform()->GetWorldScale(); }

		float GetWorldPositionX() const{ return GetTransform()->GetWorldPositionX(); }
		float GetWorldPositionY() const{ return GetTransform()->GetWorldPositionY(); }
		float GetWorldPositionZ() const{ return GetTransform()->GetWorldPositionZ(); }
		Vector2 GetWorldPositionXY() const { return GetTransform()->GetWorldPositionXY(); }
		float GetWorldRotationX() const { return GetTransform()->GetWorldRotationX(); }
		float GetWorldRotationY() const { return GetTransform()->GetWorldRotationY(); }
		float GetWorldRotationZ() const { return GetTransform()->GetWorldRotationZ(); }
		float GetWorldScaleX() const{ return GetTransform()->GetWorldScaleX(); }
		float GetWorldScaleY() const{ return GetTransform()->GetWorldScaleY(); }
		float GetWorldScaleZ() const{ return GetTransform()->GetWorldScaleZ(); }

		Vector3 Forward() const { return GetTransform()->Forward(); }
		Vector3 Right() const { return GetTransform()->Right(); }
		Vector3 Up() const { return GetTransform()->Up(); }

		Vector3 WorldForward() const { return GetTransform()->WorldForward(); }
		Vector3 WorldRight() const { return GetTransform()->WorldRight(); }
		Vector3 WorldUp() const { return GetTransform()->WorldUp(); }
		
		const math::Matrix& GetWorldMatrix() const { return GetTransform()->GetWorldMatrix(); }

		Vector2 GetWorldCenterUp() const { return GetTransform()->GetWorldCenterUp(); }
		Vector2 GetWorldCenterBottom() const { return GetTransform()->GetWorldCenterBottom(); }
		Vector2 GetWorldLeftUp() const { return GetTransform()->GetWorldLeftUp(); }
		Vector2 GetWorldLeftCenter() const { return GetTransform()->GetWorldLeftCenter(); }
		Vector2 GetWorldLeftBottom() const { return GetTransform()->GetWorldLeftBottom(); }
		Vector2 GetWorldRightUp() const { return GetTransform()->GetWorldRightUp(); }
		Vector2 GetWorldRightCenter() const { return GetTransform()->GetWorldRightCenter(); }
		Vector2 GetWorldRightBottom() const { return GetTransform()->GetWorldRightBottom(); }
	};
}