#include "hjEditorObject.h"

namespace hj
{
	EditorObject::EditorObject()
		: GameObject()
	{
	}
	EditorObject::~EditorObject()
	{
	}
	void EditorObject::Initialize()
	{
		GameObject::Initialize();
	}
	void EditorObject::Update()
	{
		GameObject::Update();
	}
	void EditorObject::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}
	void EditorObject::Render()
	{
		GameObject::Render();
	}
}