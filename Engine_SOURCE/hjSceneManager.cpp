#include "hjSceneManager.h"
#include "hjTransform.h"
#include "hjMeshRenderer.h"
#include "hjRenderer.h"
#include "hjResources.h"
#include "hjTexture.h"
#include "hjPlayerScript.h"

namespace hj
{
	Scene* SceneManager::mPlayScene = nullptr;

	void SceneManager::Initialize()
	{
		mPlayScene = new Scene();
		mPlayScene->Initialize();

		GameObject* obj = new GameObject();
		Transform* tr = new Transform();
		tr->SetPosition(Vector3(0.f, 0.f, 0.f));
		obj->AddComponent(tr);

		MeshRenderer* mr = new MeshRenderer();
		obj->AddComponent(mr);

		Mesh* mesh = Resources::Find<Mesh>(L"RectMesh");
		Material* material = Resources::Find<Material>(L"RectMaterial");

		mr->SetMaterial(material);
		mr->SetMesh(mesh);

		PlayerScript* script = new PlayerScript();
		obj->AddComponent(script);

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