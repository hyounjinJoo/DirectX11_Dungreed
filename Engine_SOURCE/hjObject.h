#pragma once
#include "hjScene.h"
#include "hjTransform.h"

namespace hj::object
{
#pragma region Instantiate
	template <typename T>
	static T* Instantiate(enums::eLayerType type)
	{
		T* gameObj = new T();

		Scene* scene = SceneManager::GetActiveScene();
		Layer& layer = scene->GetLayer(type);
		layer.AddGameObject(gameObj);

		return gameObj;
	}

	#pragma region Instantiate with Parent Transform
		template <typename T>
		static T* Instantiate(enums::eLayerType type, Transform* parent)
		{
			T* gameObj = Instantiate<T>(type);
	
			Transform* tr = gameObj->GameObject::GetComponent<Transform>();
			tr->SetParent(parent);
	
			return gameObj;
		}
	
		template <typename T>
		static T* Instantiate(enums::eLayerType type, Transform* parent, Vector3 position)
		{
			T* gameObj = Instantiate<T>(type, parent);
	
			Transform* tr = gameObj->GameObject::GetComponent<Transform>();
			tr->SetPosition(position);
	
			tr->SetParent(parent);
	
			return gameObj;
		}
	
		template <typename T>
		static T* Instantiate(enums::eLayerType type, Transform* parent, Vector3 position, Vector3 rotation)
		{
			T* gameObj = Instantiate<T>(type, parent);
	
			Transform* tr = gameObj->GameObject::GetComponent<Transform>();
			tr->SetPosition(position);
			tr->SetRotation(rotation);
	
			tr->SetParent(parent);
	
			return gameObj;
		}
	
		template <typename T>
		static T* Instantiate(enums::eLayerType type, Transform* parent, Vector3 position, Vector3 rotation, Vector3 scale)
		{
			T* gameObj = Instantiate<T>(type, parent);
	
			Transform* tr = gameObj->GameObject::GetComponent<Transform>();
			tr->SetPosition(position);
			tr->SetRotation(rotation);
			tr->SetScale(scale);
	
			tr->SetParent(parent);
	
			return gameObj;
		}
	#pragma endregion
	#pragma region Instantiate No Parent Trnasform
		template <typename T>
		static T* Instantiate(enums::eLayerType type, Vector3 position)
		{
			T* gameObj = Instantiate<T>(type);
	
			Transform* tr = gameObj->GameObject::GetComponent<Transform>();
			tr->SetPosition(position);
	
			return gameObj;
		}
	
		template <typename T>
		static T* Instantiate(enums::eLayerType type, Vector3 position, Vector3 rotation)
		{
			T* gameObj = Instantiate<T>(type);
	
			Transform* tr = gameObj->GameObject::GetComponent<Transform>();
			tr->SetPosition(position);
			tr->SetRotation(rotation);
	
			return gameObj;
		}
	
		template <typename T>
		static T* Instantiate(enums::eLayerType type, Vector3 position, Vector3 rotation, Vector3 scale)
		{
			T* gameObj = Instantiate<T>(type);
	
			Transform* tr = gameObj->GameObject::GetComponent<Transform>();
			tr->SetPosition(position);
			tr->SetRotation(rotation);
			tr->SetScale(scale);
	
			return gameObj;
		}
	#pragma endregion
#pragma endregion

	void Destroy(GameObject* gameObj)
	{
		gameObj->Death();
	}
}