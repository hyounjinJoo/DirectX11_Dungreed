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
	Scene* SceneManager::mActiveScene = nullptr;

	void SceneManager::Initialize()
	{
		mActiveScene = new Scene();
		mActiveScene->Initialize();

#pragma region Camera Game Object
		GameObject* cameraObj = new GameObject();
		Transform* cameraTr = new Transform();
		cameraTr->SetPosition(Vector3(0.f, 0.f, 0.f));
		cameraObj->AddComponent(cameraTr);
		
		Camera* cameraComp = new Camera();
		cameraObj->AddComponent(cameraComp);

		CameraScript* cameraScript = new CameraScript();
		cameraObj->AddComponent(cameraScript);
		
		mActiveScene->AddGameObject(cameraObj, eLayerType::Camera);
#pragma endregion
#pragma region Sprite Object
		GameObject* spriteObj = new GameObject();
		Transform* spriteTr = new Transform();
		spriteObj->SetName(L"Sprite Obj");

		spriteTr->SetPosition(Vector3(0.f, 0.f, 10.f));
		spriteTr->SetScale(Vector3(5.f, 5.f, 1.f));
		spriteObj->AddComponent(spriteTr);

		SpriteRenderer* sr = new SpriteRenderer();
		spriteObj->AddComponent(sr);

		std::shared_ptr<Mesh> mesh = Resources::Find<Mesh>(L"RectMesh");
		std::shared_ptr<Material> spriteMaterial = Resources::Find<Material>(L"SpriteMaterial");

		sr->SetMaterial(spriteMaterial);
		sr->SetMesh(mesh);

		mActiveScene->AddGameObject(spriteObj, eLayerType::Player);
#pragma endregion
#pragma region Test Object(Transform / MeshRenderer / PlayerScript)
		GameObject* obj = new GameObject();
		obj->SetName(L"Test Obj");

		Transform* tr = new Transform();
		tr->SetPosition(Vector3(0.f, 0.f, 10.f));
		tr->SetScale(Vector3(5.f, 5.f, 1.f));
		obj->AddComponent(tr);

		MeshRenderer* mr = new MeshRenderer();
		obj->AddComponent(mr);
				
		std::shared_ptr<Material> material = Resources::Find<Material>(L"RectMaterial");

		Vector2 vec2(1.f, 1.f);
		material->SetData(eGPUParam::Vector2, &vec2);

		mr->SetMaterial(material);
		mr->SetMesh(mesh);

		mActiveScene->AddGameObject(obj, eLayerType::Player);
#pragma endregion

		mActiveScene->Initialize();
	}

	void SceneManager::Update()
	{
		mActiveScene->Update();
	}

	void SceneManager::FixedUpdate()
	{
		mActiveScene->FixedUpdate();
	}

	void SceneManager::Render()
	{
		mActiveScene->Render();
	}

	void SceneManager::Release()
	{
		delete mActiveScene;
		mActiveScene = nullptr;
	}
}