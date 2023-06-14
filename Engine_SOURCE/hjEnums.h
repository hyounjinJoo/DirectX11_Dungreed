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
		// 0 ~ 4
		None = 0,
		Camera,
		Grid,
		Monster,
		MonsterHas,
		// 5 ~ 9
		MonsterAttack_ForeGround,
		MonsterAttack_NotForeGround,
		Player,
		PlayerHas,
		PlayerAttack_ForeGround,
		// 10 ~ 14
		PlayerAttack_NotForeGround,
		BackGround,
		MidGround,
		ForeGround,
		Particle,
		// 15 ~ 17
		PostProcess,
		UI,
		End = 32,
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
		TileMap,
		Animator,
		ParticleSystem,
		BossDeathParticleSystem,
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
		MapData,
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