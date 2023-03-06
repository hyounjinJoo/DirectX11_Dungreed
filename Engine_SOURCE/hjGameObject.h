#pragma once

#include "hjComponent.h"
#include "hjScript.h"
#include "hjEntity.h"

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
				mScripts.push_back(comp);
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
		std::vector<Component*> mScripts;
		bool mbDontDestroy;
	};
}