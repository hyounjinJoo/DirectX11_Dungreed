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
#include "hjApplication.h"

extern hj::Application application;

namespace hj
{
	Scene* SceneManager::mActiveScene = nullptr;

	void SceneManager::Initialize()
	{
		mActiveScene = new Scene();
		mActiveScene->Initialize();

#pragma region Setting
	#pragma region Main Camera
		GameObject* cameraObj = new GameObject();
		Transform* cameraTr = new Transform();
		cameraTr->SetPosition(Vector3(0.f, 0.f, 0.f));
		cameraObj->AddComponent(cameraTr);
		
		Camera* cameraComp = new Camera();
		cameraComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		cameraComp->TurnLayerMask(eLayerType::UI, false);
		cameraObj->AddComponent(cameraComp);

		CameraScript* cameraScript = new CameraScript();
		cameraObj->AddComponent(cameraScript);
		
		mActiveScene->AddGameObject(cameraObj, eLayerType::Camera);
	#pragma endregion
	#pragma region UI Camera
		GameObject* cameraUIObj = new GameObject();
		Transform* cameraUITr = new Transform();
		cameraUITr->SetPosition(Vector3(0.f, 0.f, 0.f));
		cameraUIObj->AddComponent(cameraUITr);

		Camera* cameraUIComp = new Camera();
		cameraUIComp->SetProjectionType(Camera::eProjectionType::Orthographic);
		cameraUIComp->DisableLayerMask();
		cameraUIComp->TurnLayerMask(eLayerType::UI, true);
		cameraUIObj->AddComponent(cameraUIComp);

		mActiveScene->AddGameObject(cameraUIObj, eLayerType::Camera);
	#pragma endregion
#pragma endregion

#pragma region Objects
	#pragma region light Object
		GameObject* light = new GameObject();
		Transform* lightTr = new Transform();
		light->SetName(L"Light");

		lightTr->SetPosition(Vector3(0.f, 0.f, 10.f));
		lightTr->SetScale(Vector3(500.f, 500.f, 1.f));
		light->AddComponent(lightTr);

		SpriteRenderer* sr = new SpriteRenderer();
		light->AddComponent(sr);

		std::shared_ptr<Mesh> mesh = Resources::Find<Mesh>(L"RectMesh");
		std::shared_ptr<Material> lightMaterial = Resources::Find<Material>(L"RectMaterial");

		sr->SetMaterial(lightMaterial);
		sr->SetMesh(mesh);

		mActiveScene->AddGameObject(light, eLayerType::Player);
	#pragma endregion
	#pragma region Test Object(Transform / MeshRenderer / PlayerScript)
		GameObject* obj = new GameObject();
		obj->SetName(L"Test Obj");
		
		Transform* tr = new Transform();
		tr->SetPosition(Vector3(0.f, 0.f, 10.f));
		tr->SetScale(Vector3(117.f * 4.f, 85.f * 4.f, 1.f));
		obj->AddComponent(tr);
		
		MeshRenderer* mr = new MeshRenderer();
		obj->AddComponent(mr);
				
		std::shared_ptr<Material> material = Resources::Find<Material>(L"SpriteMaterial");
		
		mr->SetMaterial(material);
		mr->SetMesh(mesh);

		PlayerScript* playerScript = new PlayerScript();
		obj->AddComponent(playerScript);
		
		mActiveScene->AddGameObject(obj, eLayerType::Player);
	#pragma endregion
	#pragma region Test Transform Inherit Object
		obj = new GameObject();
		obj->SetName(L"Test Obj - Transform Inherit");

		Transform* childTR = new Transform();
		childTR->SetPosition(Vector3(0.5f, 0.5f, 0.f));
		childTR->SetScale(Vector3(1.f, 1.f, 1.f));
		childTR->SetParent(tr);
		childTR->SetInheritParentTransform(true);
		obj->AddComponent(childTR);

		mr = new MeshRenderer();
		obj->AddComponent(mr);

		material = Resources::Find<Material>(L"SpriteMaterial");

		mr->SetMaterial(material);
		mr->SetMesh(mesh);

		mActiveScene->AddGameObject(obj, eLayerType::Player);
	#pragma endregion
#pragma endregion
#pragma region UI

		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);

		float width = static_cast<float>(winRect.right - winRect.left);
		float height = static_cast<float>(winRect.bottom - winRect.top);
		Vector3 originLT = Vector3(0.f - width / 2.f, height / 2.f, 0.f);

#pragma region HPBarBase
		GameObject* hpBarBase = new GameObject();
		hpBarBase->SetName(L"HPBarBase");
		Transform* hpBarBaseTR = new Transform();


		Vector3 scale = Vector3(74.f * 4.f, 16.f * 4.f, 1.f);
		Vector3 calcPos = originLT + Vector3(scale.x / 2.f + 20.f, scale.y / -2.f - 20.f, 0.f);
		hpBarBaseTR->SetPosition(calcPos);
		hpBarBaseTR->SetScale(scale);
		hpBarBase->AddComponent(hpBarBaseTR);

		SpriteRenderer* hpBarBaseSR = new SpriteRenderer();
		hpBarBase->AddComponent(hpBarBaseSR);

		std::shared_ptr<Mesh> hpBarBaseMesh = Resources::Find<Mesh>(L"RectMesh");
		std::shared_ptr<Material> hpBarBaseMaterial = Resources::Find<Material>(L"UIMaterial");

		hpBarBaseSR->SetMesh(hpBarBaseMesh);
		hpBarBaseSR->SetMaterial(hpBarBaseMaterial);

		mActiveScene->AddGameObject(hpBarBase, eLayerType::UI);
	#pragma endregion
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