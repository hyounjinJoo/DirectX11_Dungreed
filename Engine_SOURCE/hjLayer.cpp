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
		for (GameObject* obj : mGameObjects)
		{
			if (obj == nullptr)
				continue;

			obj->Initialize();
		}
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

		// 삭제할 오브젝트를 전부 찾아온다.
		for (GameObject* gameObject : mGameObjects)
		{
			if (gameObject->GetState() == GameObject::eState::Dead)
			{
				deleteObjects.insert(gameObject);
			}
		}

		// 지워야 할 오브젝트들을 게임 오브젝트 모음 안에서 삭제
		for (GameObjectIter iter = mGameObjects.begin()
			; iter != mGameObjects.end()
			; )
		{
			std::set<GameObject*>::iterator deleteIter
				= deleteObjects.find(*iter);

			if (deleteIter != deleteObjects.end())
			{
				mGameObjects.erase(iter);
			}
			else
			{
				++deleteIter;
			}
		}

		// 삭제할 오브젝트들을 실제 메모리에서 삭제
		for (GameObject* gameObject : deleteObjects)
		{
			delete gameObject;
			gameObject = nullptr;
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