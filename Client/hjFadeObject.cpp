#include "hjFadeObject.h"
#include "hjFadeScript.h"
#include "hjMeshRenderer.h"
#include "hjResourceManager.h"
#include "hjApplication.h"

extern hj::Application application;
namespace hj
{
	FadeObject::FadeObject()
	{
		mFadeScript = AddComponent<FadeScript>();
		
		MeshRenderer* fadeMR = AddComponent<MeshRenderer>();
		std::shared_ptr<Mesh> mesh = MESH_FIND_STR("Mesh_Rect");
		std::shared_ptr<Material> material = MTRL_FIND_STR("MTRL_Fade");
		assert(mesh && material);

		fadeMR->SetMesh(mesh);
		fadeMR->SetMaterial(material);

		Vector2 scale = application.GetWindowSize();
		SetScaleXY(scale);
	}

	FadeObject::~FadeObject()
	{
	}

	void FadeObject::Initialize()
	{
	}

	void FadeObject::Update()
	{
		GameObject::Update();
	}

	void FadeObject::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void FadeObject::Render()
	{
		GameObject::Render();
	}

	class FadeScript* FadeObject::GetFadeScript()
	{
		if (mFadeScript)
			return mFadeScript;

		return nullptr;
	}

}