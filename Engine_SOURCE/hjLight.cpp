#include "pch.h"
#include "hjLight.h"
#include "hjGameObject.h"
#include "hjRenderer.h"

namespace hj
{
	Light::Light()
		: Component(eComponentType::Light)
	{
	}

	Light::Light(const Light& light)
		: Component(eComponentType::Light)
	{

	}

	Light::~Light()
	{
	}
	
	void Light::Initialize()
	{
	}

	void Light::Update()
	{
	}

	void Light::FixedUpdate()
	{
		GameObject* owner = GetOwner();
		if (!owner)
		{
			return;
		}

		Vector4 worldPosition = Vector4(owner->GetWorldPosition(), 1.f);
		mAttribute.position = worldPosition;

		Vector4 direction = Vector4(owner->Right(), 1.f);
		mAttribute.direction = direction;

		renderer::PushLightAttribute(mAttribute);
	}

	void Light::Render()
	{
	}

	hj::Component* Light::Clone() const
	{
		return new Light(*this);
	}

}