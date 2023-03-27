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
		Transform* GetTransform() 
		{ 
			if(mComponents[(UINT)eComponentType::Transform])
				return static_cast<Transform*>(mComponents[(UINT)eComponentType::Transform]); 
			
			return nullptr;
		}
		
		Vector3 GetPosition() { return GetTransform()->GetPosition(); }
		float GetPositionX() { return GetTransform()->GetPositionX(); }
		float GetPositionY() { return GetTransform()->GetPositionY(); }
		float GetPositionZ() { return GetTransform()->GetPositionZ(); }
		Vector2 GetPositionXY() { return GetTransform()->GetPositionXY(); }

		Vector3 GetRotation() { return GetTransform()->GetRotation(); }
		float GetRotationX() { return GetTransform()->GetRotationX(); }
		float GetRotationY() { return GetTransform()->GetRotationY(); }
		float GetRotationZ() { return GetTransform()->GetRotationZ(); }
		Vector2 GetRotationXY() { return GetTransform()->GetRotationXY(); }
		
		Vector3 GetScale() { return GetTransform()->GetScale(); }
		float GetScaleX() { return GetTransform()->GetScaleX(); }
		float GetScaleY() { return GetTransform()->GetScaleY(); }
		float GetScaleZ() { return GetTransform()->GetScaleZ(); }
		Vector2 GetScaleXY() { return GetTransform()->GetScaleXY(); }

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

		const Vector3& GetWorldPosition() { return GetTransform()->GetWorldPosition(); }
		const Vector3& GetWorldRotation() { return GetTransform()->GetWorldRotation(); }
		const Vector3& GetWorldScale() { return GetTransform()->GetWorldScale(); }

		float GetWorldPositionX() { return GetTransform()->GetWorldPositionX(); }
		float GetWorldPositionY() { return GetTransform()->GetWorldPositionY(); }
		float GetWorldPositionZ() { return GetTransform()->GetWorldPositionZ(); }
		float GetWorldRotationX() { return GetTransform()->GetWorldRotationX(); }
		float GetWorldRotationY() { return GetTransform()->GetWorldRotationY(); }
		float GetWorldRotationZ() { return GetTransform()->GetWorldRotationZ(); }
		float GetWorldScaleX() { return GetTransform()->GetWorldScaleX(); }
		float GetWorldScaleY() { return GetTransform()->GetWorldScaleY(); }
		float GetWorldScaleZ() { return GetTransform()->GetWorldScaleZ(); }

		Vector3 Forward() { return GetTransform()->Forward(); }
		Vector3 Right() { return GetTransform()->Right(); }
		Vector3 Up() { return GetTransform()->Up(); }

		//void SetInheritParentTransform(bool inherit) { GetTransform()->SetInheritParentTransform(inherit); }

		const math::Matrix& GetWorldMatrix() { return GetTransform()->GetWorldMatrix(); }
	};
}