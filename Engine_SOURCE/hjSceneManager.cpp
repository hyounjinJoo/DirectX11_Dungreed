#include "hjSceneManager.h"
#include "hjTransform.h"
#include "hjMeshRenderer.h"
#include "hjRenderer.h"
#include "hjResources.h"
#include "hjTexture.h"
#include "hjPlayerScript.h"
#include "hjCamera.h"
#include "hjCameraScript.h"
#include "hjSpriteRenderer.h"

namespace hj
{
	Scene* SceneManager::mPlayScene = nullptr;

	void SceneManager::Initialize()
	{
		mPlayScene = new Scene();
		mPlayScene->Initialize();

#pragma region Camera Game Object
		GameObject* cameraObj = new GameObject();
		Transform* cameraTr = new Transform();
		cameraTr->SetPosition(Vector3(0.f, 0.f, 0.f));
		cameraObj->AddComponent(cameraTr);
		
		Camera* cameraComp = new Camera();
		cameraObj->AddComponent(cameraComp);

		CameraScript* cameraScript = new CameraScript();
		cameraObj->AddComponent(cameraScript);
		
		mPlayScene->AddGameObject(cameraObj, eLayerType::Camera);
#pragma endregion
#pragma region Test Object(Transform / MeshRenderer / PlayerScript)
		GameObject* obj = new GameObject();
		Transform* tr = new Transform();
		tr->SetPosition(Vector3(0.f, 0.f, 10.f));
		obj->AddComponent(tr);

		MeshRenderer* mr = new MeshRenderer();
		obj->AddComponent(mr);

		std::shared_ptr<Mesh> mesh = Resources::Find<Mesh>(L"RectMesh");
		std::shared_ptr<Material> material = Resources::Find<Material>(L"RectMaterial");

		Vector2 vec2(1.f, 1.f);
		material->SetData(eGPUParam::Vector2, &vec2);

		mr->SetMaterial(material);
		mr->SetMesh(mesh);

		mPlayScene->AddGameObject(obj, eLayerType::Player);
#pragma endregion
#pragma region Sprite Object
		GameObject* spriteObj = new GameObject();
		Transform* spriteTr = new Transform();
		spriteTr->SetPosition(Vector3(0.f, 0.f, 20.f));
		spriteObj->AddComponent(spriteTr);

		SpriteRenderer* sr = new SpriteRenderer();
		spriteObj->AddComponent(sr);

		std::shared_ptr<Material> spriteMaterial = Resources::Find<Material>(L"SpriteMaterial");

		sr->SetMaterial(spriteMaterial);
		sr->SetMesh(mesh);

		mPlayScene->AddGameObject(spriteObj, eLayerType::Player);
#pragma endregion
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
	void SceneManager::Release()
	{
		delete mPlayScene;
		mPlayScene = nullptr;
	}
}