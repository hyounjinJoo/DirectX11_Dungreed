#pragma once

namespace hj::enums
{
	enum class eSceneType
	{
		Title,
		Story,
		Town,
		Dungeon,
		Ending,
		Test,
		End,
	};

	enum class eLayerType
	{
		None = 0,
		Camera,
		Grid,
		Monster,
		Player,

		UI,
		End = 16,
	};

	enum class eComponentType
	{
		None,
		Transform,	// ��ġ �����͸� �����ϴ� ������Ʈ
		Camera,
		Mesh,
		Collider,
		//Collider2,
		MeshRenderer,
		SpriteRenderer,
		UI,
		Script,
		End,
	};

	enum class eResourceType
	{
		Mesh,
		Texture,
		Material,
		Sound,
		/*Font,*/
		Prefab,
		MeshData,
		GraphicShader,
		ComputeShader,
		Script,
		End,
	};

	enum class eColliderType
	{
		None,
		Rect,
		Circle,
		Box,
		Sphere,
		End,
	};

	enum class eCollisionState
	{
		CollisionNot = 0,
		CollisionEnter,
		CollisionStay,
		CollisionExit,
		End,
	};
}