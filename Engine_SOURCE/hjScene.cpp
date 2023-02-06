#include "hjScene.h"

namespace hj
{
	Scene::Scene()
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

	void Scene::AddGameObject(GameObject* gameObject, const eLayerType type)
	{
		mLayers[(UINT)type].AddGameObject(gameObject);
	}
}