#include "hjCollider.h"

namespace hj
{
	Collider::Collider()
		: Component(eComponentType::Collider)
		, mType(eColliderType::None)
		, mbTrigger(false)
	{
	}

	Collider::~Collider()
	{
	}

	void Collider::Initialize()
	{
	}

	void Collider::Update()
	{
	}

	void Collider::FixedUpdate()
	{
	}

	void Collider::Render()
	{
	}

}