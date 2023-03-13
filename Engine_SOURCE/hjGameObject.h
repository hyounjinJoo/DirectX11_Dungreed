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
		Transform* GetTransform() { return static_cast<Transform*>(mComponents[(UINT)eComponentType::Transform]); }
		
		Vector3 GetPosition() { return GetTransform()->GetPosition(); }
		float GetPositionX() { return GetTransform()->GetPositionX(); }
		float GetPositionY() { return GetTransform()->GetPositionY(); }
		float GetPositionZ() { return GetTransform()->GetPositionZ(); }

		Vector3 GetRotation() { return GetTransform()->GetRotation(); }
		float GetRotationX() { return GetTransform()->GetRotationX(); }
		float GetRotationY() { return GetTransform()->GetRotationY(); }
		float GetRotationZ() { return GetTransform()->GetRotationZ(); }
		
		Vector3 GetScale() { return GetTransform()->GetScale(); }
		float GetScaleX() { return GetTransform()->GetScaleX(); }
		float GetScaleY() { return GetTransform()->GetScaleY(); }
		float GetScaleZ() { return GetTransform()->GetScaleZ(); }

		void SetPosition(Vector3 position) { GetTransform()->SetPosition(position); }
		void SetPositionX(float posX) { GetTransform()->SetPositionX(posX); }
		void SetPositionY(float posY) { GetTransform()->SetPositionY(posY); }
		void SetPositionZ(float posZ) { GetTransform()->SetPositionZ(posZ); }

		void SetRotation(Vector3 rotation) { GetTransform()->SetRotation(rotation); }
		void SetRotationX(float rotX) { GetTransform()->SetRotationX(rotX); }
		void SetRotationY(float rotY) { GetTransform()->SetRotationY(rotY); }
		void SetRotationZ(float rotZ) { GetTransform()->SetRotationZ(rotZ); }

		void SetScale(Vector3 scale) { GetTransform()->SetScale(scale); }
		void SetScaleX(float scaleX) { GetTransform()->SetScaleX(scaleX); }
		void SetScaleY(float scaleY) { GetTransform()->SetScaleY(scaleY); }
		void SetScaleZ(float scaleZ) { GetTransform()->SetScaleZ(scaleZ); }

		Vector3 Forward() { return GetTransform()->Forward(); }
		Vector3 Right() { return GetTransform()->Right(); }
		Vector3 Up() { return GetTransform()->Up(); }

		void SetInheritParentTransform(bool inherit) { GetTransform()->SetInheritParentTransform(inherit); }

		const math::Matrix& GetWorldMatrix() { return GetTransform()->GetWorldMatrix(); }
	};
}