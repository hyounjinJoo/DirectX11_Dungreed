#include "hjDebugObject.h"

namespace hj
{
	DebugObject::DebugObject()
		: GameObject()
	{
		SetName(WIDE("Debug Obj"));
	}

	DebugObject::~DebugObject()
	{
	}
	
	void DebugObject::Initialize()
	{
	}
	
	void DebugObject::Update()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->Update();
		}
	}
	void DebugObject::FixedUpdate()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->FixedUpdate();
		}
	}
	void DebugObject::Render()
	{
		for (Component* comp : mComponents)
		{
			if (comp == nullptr)
				continue;

			comp->Render();
		}
	}
}