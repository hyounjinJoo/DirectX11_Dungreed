#include "pch.h"
#include "hjScene.h"
#include "hjObject.h"
#include "hjCamera.h"
#include "hjRenderer.h"
#include "../Client/hjCameraScript.h"

extern hj::Camera* hj::renderer::mainCamera;
namespace hj
{
	Scene::Scene(eSceneType type)
		: mSceneType(type)
	{
		mLayers.resize((UINT)eLayerType::End);
	}

	Scene::~Scene()
	{
	}

	void Scene::Initialize()
	{
		for (Layer& layer : mLayers)
		{
			layer.Initialize();
		}
	}

	void Scene::Update()
	{
		for (Layer& layer : mLayers)
		{
			layer.Update();
		}
	}

	void Scene::FixedUpdate()
	{
		for (Layer& layer : mLayers)
		{
			layer.FixedUpdate();
		}
	}

	void Scene::Render()
	{
		for (Layer& layer : mLayers)
		{
			layer.Render();
		}
	}

	void Scene::Destroy()
	{
		for (Layer& layer : mLayers)
		{
			layer.Destroy();
		}
	}

	void Scene::RemoveDeadObjects()
	{
		for (Layer& layer : mLayers)
		{
			layer.RemoveDeadObjects();
		}
	}

	void Scene::OnEnter()
	{
	}

	void Scene::OnExit()
	{
	}

	void Scene::CreateDefaultCamera()
	{
#pragma region Main Camera
		Vector3 pos = Vector3::Zero;
		Vector3 rot = Vector3::Zero;
		Vector3 scale = Vector3::One;

		GameObject* cameraObj = object::Instantiate<GameObject>(eLayerType::Camera, pos);

		Camera* cameraComp = new Camera();
		cameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		//cameraComp->RegisterCameraInRenderer();
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		cameraObj->AddComponent(cameraComp);
		renderer::mainCamera = cameraComp;

		cameraObj->AddComponent(new CameraScript());
		object::DontDestroyOnLoad(cameraObj);
#pragma endregion
#pragma region UI Camera
		pos = Vector3::Zero;
		rot = Vector3::Zero;
		scale = Vector3::One;

		GameObject* cameraUIObj = object::Instantiate<GameObject>(eLayerType::Camera, pos);

		Camera* cameraUIComp = new Camera();
		cameraUIComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		cameraUIComp->DisableLayerMask();
		cameraUIComp->TurnLayerMask(eLayerType::UI, true);
		cameraUIObj->AddComponent(cameraUIComp);
		object::DontDestroyOnLoad(cameraUIObj);
#pragma endregion
	}

	void Scene::AddGameObject(GameObject* gameObject, const eLayerType type)
	{
		mLayers[(UINT)type].AddGameObject(gameObject);
		gameObject->SetLayerType(type);
	}

	std::vector<GameObject*> Scene::GetDontDestroyGameObjects()
	{
		std::vector<GameObject*> gameObjects;
		for (Layer& layer : mLayers)
		{
			std::vector<GameObject*> dontGameObjs
				= layer.GetDontDestroyGameObjects();

			if(dontGameObjs.empty())
				continue;

			gameObjects.insert(gameObjects.end()
				, dontGameObjs.begin()
				, dontGameObjs.end());
		}

		return gameObjects;
	}

	const std::vector<GameObject*>& Scene::GetGameObjects(const eLayerType type)
	{
		return mLayers[(UINT)type].GetGameObejcts();
	}

}