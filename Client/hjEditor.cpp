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

		tr->FixedUpdate();

		Camera::SetGpuViewMatrix(renderer::mainCamera->GetViewMatrix());
		Camera::SetGpuProjectionMatrix(renderer::mainCamera->GetProjectionMatrix());

		debugObj->Render();		
	}
}