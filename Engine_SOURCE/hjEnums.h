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
		BackGround,
		MidGround,
		ForeGround,
		UI,
		End = 16,
	};

	enum class eComponentType
	{
		None,
		Transform,	// 위치 데이터를 수정하는 컴포넌트
		Camera,
		Mesh,
		Collider,
		//Collider2,
		RigidBody,
		MeshRenderer,
		SpriteRenderer,
		Animator,
		Light,
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

	enum class eMoveDir
	{
		Left = 0,
		Top,
		Right,
		Bottom,
		End,
	};

	enum class eLightType
	{
		Directional,
		Point,
		Spot,
		End,
	};
}