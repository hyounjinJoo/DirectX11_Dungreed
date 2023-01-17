#pragma once

#include "hjComponent.h"

namespace hj
{
	class GameObject
	{
	public:
		enum eState
		{
			Active,			// Update ���� ����
			Paused,			// Update ���� ����X
			Dead,			// �Ҹ� ���
		};

		GameObject();
		virtual ~GameObject();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

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

	private:
		eState mState;
		std::vector<Component*> mComponents;

	};
}