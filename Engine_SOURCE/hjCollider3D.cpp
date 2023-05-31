#include "pch.h"
#include "hjCollider3D.h"
#include "hjGameObject.h"
#include "hjScript.h"

namespace hj
{
	Collider3D::Collider3D()
		: Collider()
	{
	}

	Collider3D::Collider3D(const Collider3D& collider3D)
		: Collider()
	{

	}

	Collider3D::~Collider3D()
	{
	}

	void Collider3D::Initialize()
	{
	}

	void Collider3D::Update()
	{
	}

	void Collider3D::FixedUpdate()
	{
	}

	void Collider3D::Render()
	{
	}

	hj::Component* Collider3D::Clone() const
	{
		//return new Collider3D(*this);
		return nullptr;
	}

	void Collider3D::OnCollisionEnter(Collider* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollisionEnter(collider);
		}
	}

	void Collider3D::OnCollisionStay(Collider* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollisionStay(collider);
		}
	}

	void Collider3D::OnCollisionExit(Collider* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollisionExit(collider);
		}
	}

	void Collider3D::OnTriggerEnter(Collider* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTriggerEnter(collider);
		}
	}

	void Collider3D::OnTriggerStay(Collider* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTriggerStay(collider);
		}
	}

	void Collider3D::OnTriggerExit(Collider* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTriggerExit(collider);
		}
	}
}