#include "pch.h"
#include "hjLayer.h"

namespace hj
{
	Layer::Layer()
	{
	}

	Layer::~Layer()
	{
		for (GameObject* obj : mGameObjects)
		{
			if (obj == nullptr)
				continue;

			delete obj;
			obj = nullptr;
		}
	}

	void Layer::Initialize()
	{
	}

	void Layer::Update()
	{
		for (GameObject* obj : mGameObjects)
		{
			if (obj == nullptr)
				continue;
			if (obj->GetState() != GameObject::eState::Active)
				continue;

			obj->Update();
		}
	}

	void Layer::FixedUpdate()
	{
		for (GameObject* obj : mGameObjects)
		{
			if (obj == nullptr)
				continue;
			if (obj->GetState() != GameObject::eState::Active)
				continue;

			obj->FixedUpdate();
		}
	}

	void Layer::Render()
	{
		for (GameObject* obj : mGameObjects)
		{
			if (obj == nullptr)
				continue;
			if (obj->GetState() != GameObject::eState::Active)
				continue;

			obj->Render();
		}
	}

	void Layer::Destroy()
	{
		std::set<GameObject*> deleteObjects;
		std::vector<GameObject*> dontGameObjs = GetDontDestroyGameObjects();

		// ������ ������Ʈ�� ���� ã�ƿ´�.
		for (GameObject* gameObject : mGameObjects)
		{
			//if (gameObject->GetState() == GameObject::eState::Dead)
			//{
			//	deleteObjects.insert(gameObject);
			//}
			auto findResult = find(dontGameObjs.begin(), dontGameObjs.end(), gameObject);
			if (findResult != dontGameObjs.end())
				continue;

			deleteObjects.insert(gameObject);
		}

		// ������ �� ������Ʈ���� ���� ������Ʈ ���� �ȿ��� ����
		for (GameObjectIter iter = mGameObjects.begin()
			; iter != mGameObjects.end()
			; )
		{
			std::set<GameObject*>::iterator deleteIter
				= deleteObjects.find(*iter);

			if (deleteIter != deleteObjects.end())
			{
				iter = mGameObjects.erase(iter);
			}
			else
			{
				++iter;
			}
		}

		// ������ ������Ʈ���� ���� �޸𸮿��� ����
		for (GameObject* gameObject : deleteObjects)
		{
			delete gameObject;
			gameObject = nullptr;
		}
	}

	void Layer::RemoveDeadObjects()
	{
		if (mGameObjects.empty())
			return;

		GameObject::eState objectState;

		for (GameObjectIter iter = mGameObjects.begin()
			; iter != mGameObjects.end()
			; )
		{
			objectState = (*iter)->GetState();
			if (GameObject::eState::Dead == objectState)
			{
				GameObject* obj = *iter;
				iter = mGameObjects.erase(iter);
				delete obj;

				continue;
			}

			++iter;
		}
	}

	void Layer::AddGameObject(GameObject* gameObject)
	{
		if (gameObject == nullptr)
			return;

		mGameObjects.push_back(gameObject);
	}

	std::vector<GameObject*> Layer::GetDontDestroyGameObjects()
	{
		std::vector<GameObject*> donts;

		if (mGameObjects.empty())
			return donts;

		for (GameObjectIter iter = mGameObjects.begin()
			; iter != mGameObjects.end()
			;)
		{
			if ((*iter)->IsDontDestroy() == true)
			{
				donts.push_back((*iter));
				iter = mGameObjects.erase(iter);
			}
			else
			{
				++iter;
			}
		}

		return donts;
	}

}