#include "hjEditor.h"

#include "hjObject.h"
#include "hjMeshRenderer.h"
#include "hjResources.h"
#include "hjMesh.h"
#include "hjMaterial.h"
#include "hjGridScript.h"

namespace hj
{
	void Editor::Initialize()
	{
		// 충돌체의 종류 갯수만큼만 있으면 된다.
		mDebugObjects.resize((UINT)eColliderType::End);

		std::shared_ptr<Mesh> mesh = MESH_FIND("Mesh_DebugRect");
		std::shared_ptr<Material> material = MTRL_FIND("MTRL_Debug");

		mDebugObjects[(UINT)eColliderType::Rect] = object::Instantiate<DebugObject>();
		MeshRenderer* renderer
			= mDebugObjects[(UINT)eColliderType::Rect]->AddComponent<MeshRenderer>();

		renderer->SetMaterial(material);
		renderer->SetMesh(mesh);

		mesh = MESH_FIND("Mesh_Circle");

		mDebugObjects[(UINT)eColliderType::Circle] = object::Instantiate<DebugObject>();
		renderer
			= mDebugObjects[(UINT)eColliderType::Circle]->AddComponent<MeshRenderer>();

		renderer->SetMaterial(material);
		renderer->SetMesh(mesh);

#pragma region Grid Object
		EditorObject* gridObj = object::Instantiate<EditorObject>();
		MeshRenderer* gridMR = new MeshRenderer();
		mesh = MESH_FIND("Mesh_Rect");
		material = MTRL_FIND("MTRL_Grid");
		gridMR->SetMesh(mesh);
		gridMR->SetMaterial(material);
		gridObj->AddComponent(gridMR);

		GridScript* gridScript = new GridScript();
		gridScript->SetCamera(mainCamera);
		gridObj->AddComponent(gridScript);

		mEditorObjects.push_back(gridObj);
#pragma endregion
	}

	void Editor::Run()
	{
		Update();
		FixedUpdate();
		Render();
	}

	void Editor::Update()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->Update();
		}
	}
	
	void Editor::FixedUpdate()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->FixedUpdate();
		}
	}
	
	void Editor::Render()
	{
		for (EditorObject* obj : mEditorObjects)
		{
			obj->Render();
		}

		for (DebugMesh& mesh : renderer::debugMeshes)
		{
			DebugRender(mesh);
		}
		renderer::debugMeshes.clear();
	}
	
	void Editor::Release()
	{
		for (auto obj : mWidgets)
		{
			if (obj)
			{
				delete obj;
				obj = nullptr;
			}
		}
		for (auto obj : mEditorObjects)
		{
			if (obj)
			{
				delete obj;
				obj = nullptr;
			}
		}
		
		for (auto obj : mDebugObjects)
		{
			if (obj)
			{
				delete obj;
				obj = nullptr;
			}
		}
	}
	
	void Editor::DebugRender(graphics::DebugMesh& mesh)
	{
		DebugObject* debugObj = mDebugObjects[(UINT)mesh.type];

		Transform* tr = debugObj->GetComponent<Transform>();
		tr->SetPosition(mesh.position);
		tr->SetRotation(mesh.rotation);

		switch (mesh.type)
		{
		case hj::enums::eColliderType::None:
			break;
		case hj::enums::eColliderType::Rect:
			tr->SetScale(mesh.scale);
			break;
		case hj::enums::eColliderType::Circle:
			tr->SetScale(Vector3(mesh.radius));
			break;
		case hj::enums::eColliderType::Box:
			break;
		case hj::enums::eColliderType::Sphere:
			break;
		case hj::enums::eColliderType::End:
			break;
		default:
			break;
		}

		BaseRenderer* renderer = debugObj->GetComponent<BaseRenderer>();
		Camera* camera = renderer::mainCamera;
		std::shared_ptr<Material> material = renderer->GetMaterial();
		material->SetData(eGPUParam::Int_1, &mesh.state);
		tr->FixedUpdate();

		Camera::SetGpuViewMatrix(renderer::mainCamera->GetViewMatrix());
		Camera::SetGpuProjectionMatrix(renderer::mainCamera->GetProjectionMatrix());

		debugObj->Render();		
	}
}