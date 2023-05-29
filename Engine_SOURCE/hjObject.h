#pragma once
#include "hjScene.h"
#include "hjLayer.h"
#include "hjGameObject.h"
#include "hjSceneManager.h"
#include "hjTransform.h"

namespace hj::object
{
#pragma region Instantiate with No Layer(For Editor)
	template <typename T>
	static T* Instantiate()
	{
		T* gameObject = new T();

		return gameObject;
	}
#pragma endregion

#pragma region Instantiate
	template <typename T>
	static T* Instantiate(enums::eLayerType type)
	{
		T* gameObject = new T();

		Scene* scene = SceneManager::GetActiveScene();
		Layer& layer = scene->GetLayer(type);
		gameObject->SetLayerType(type);
		layer.AddGameObject(gameObject);
		gameObject->Initialize();

		return gameObject;
	}

	template <typename T>
	static T* Instantiate(enums::eLayerType type, Scene* scene)
	{
		T* gameObject = new T();
		Layer& layer = scene->GetLayer(type);
		gameObject->SetLayerType(type);
		layer.AddGameObject(gameObject);

		return gameObject;
	}

	#pragma region Instantiate with Parent Transform
		template <typename T>
		static T* Instantiate(enums::eLayerType type, Transform* parent)
		{
			T* gameObject = Instantiate<T>(type);
	
			Transform* tr = gameObject->GameObject::GetComponent<Transform>();
			tr->SetParent(parent);
	
			return gameObject;
		}
	
		template <typename T>
		static T* Instantiate(enums::eLayerType type, Transform* parent, Vector3 position)
		{
			T* gameObject = Instantiate<T>(type, parent);
	
			Transform* tr = gameObject->GameObject::GetComponent<Transform>();
			tr->SetPosition(position);
			tr->FixedUpdate();
	
			tr->SetParent(parent);
	
			return gameObject;
		}
	
		template <typename T>
		static T* Instantiate(enums::eLayerType type, Transform* parent, Vector3 position, Vector3 rotation)
		{
			T* gameObject = Instantiate<T>(type, parent);
	
			Transform* tr = gameObject->GameObject::GetComponent<Transform>();
			tr->SetPosition(position);
			tr->SetRotation(rotation);
			tr->FixedUpdate();
	
			tr->SetParent(parent);
	
			return gameObject;
		}
	
		template <typename T>
		static T* Instantiate(enums::eLayerType type, Transform* parent, Vector3 position, Vector3 rotation, Vector3 scale)
		{
			T* gameObject = Instantiate<T>(type, parent);
	
			Transform* tr = gameObject->GameObject::GetComponent<Transform>();
			tr->SetPosition(position);
			tr->SetRotation(rotation);
			tr->SetScale(scale);
			tr->FixedUpdate();
	
			tr->SetParent(parent);
	
			return gameObject;
		}
	#pragma endregion
	#pragma region Instantiate No Parent Trnasform
		template <typename T>
		static T* Instantiate(enums::eLayerType type, Vector3 position)
		{
			T* gameObject = Instantiate<T>(type);
	
			Transform* tr = gameObject->GameObject::GetComponent<Transform>();
			tr->SetPosition(position);
			tr->FixedUpdate();
	
			return gameObject;
		}
	
		template <typename T>
		static T* Instantiate(enums::eLayerType type, Vector3 position, Vector3 rotation)
		{
			T* gameObject = Instantiate<T>(type);
	
			Transform* tr = gameObject->GameObject::GetComponent<Transform>();
			tr->SetPosition(position);
			tr->SetRotation(rotation);
			tr->FixedUpdate();
	
			return gameObject;
		}
	
		template <typename T>
		static T* Instantiate(enums::eLayerType type, Vector3 position, Vector3 rotation, Vector3 scale)
		{
			T* gameObject = Instantiate<T>(type);
	
			Transform* tr = gameObject->GameObject::GetComponent<Transform>();
			tr->SetPosition(position);
			tr->SetRotation(rotation);
			tr->SetScale(scale);
			tr->FixedUpdate();
	
			return gameObject;
		}
	#pragma endregion
#pragma endregion
#pragma region Clone
	template <typename T>
	static T* Clone(T* object)
	{
		T* gameObject = static_cast<T*>(object->Clone());

		Scene* scene = SceneManager::GetActiveScene();
		eLayerType type = object->GetLayerType();
		Layer& layer = scene->GetLayer(type);
		gameObject->SetLayerType(type);
		layer.AddGameObject(gameObject);
		gameObject->Initialize();

		return gameObject;
	}
#pragma endregion
	static void Destroy(GameObject* gameObject)
	{
		if (gameObject == nullptr)
			return;

		gameObject->Death();
	}

	static void DontDestroyOnLoad(GameObject* gameObject)
	{
		if (gameObject == nullptr)
			return;

		gameObject->DontDestroy(true);
	}
}