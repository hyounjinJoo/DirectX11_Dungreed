#include "hjScript.h"

namespace hj
{
	Script::Script()
		: Component(eComponentType::Script)
	{
	}

	Script::~Script()
	{
	}

	void Script::Initialize()
	{
	}

	void Script::Update()
	{
	}

	void Script::FixedUpdate()
	{
	}

	void Script::Render()
	{
	}

	hj::Component* Script::Clone() const
	{
		return new Script();
	}

}