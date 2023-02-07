#include "hjSceneManager.h"
#include "hjTransform.h"
#include "hjMeshRenderer.h"
#include "hjRenderer.h"
#include "hjResources.h"
#include "hjTexture.h"

namespace hj
{
	Scene* SceneManager::mPlayScene = nullptr;

	void SceneManager::Initialize()
	{
		mPlayScene = new Scene();
		mPlayScene->Initialize();

		GameObject* obj = new GameObject();
		Transform* tr = new Transform();
		tr->SetPosition(Vector3(0.2f, 0.2f, 0.2f));
		obj->AddComponent(tr);

		MeshRenderer* mr = new MeshRenderer();
		obj->AddComponent(mr);

		mr->SetShader(renderer::shader);
		mr->SetMesh(renderer::mesh);

		Texture* texture = Resources::Load<Texture>(L"DungeonEatFrame08", L"DungeonEat08.png");
		texture->BindShader(eShaderStage::PS, 0);

		mPlayScene->AddGameObject(obj, eLayerType::Player);
	}

	void SceneManager::Update()
	{
		mPlayScene->Update();
	}

	void SceneManager::FixedUpdate()
	{
		mPlayScene->FixedUpdate();
	}

	void SceneManager::Render()
	{
		mPlayScene->Render();
	}
}