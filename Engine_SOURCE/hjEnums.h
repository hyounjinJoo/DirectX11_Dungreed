#pragma once

namespace hj::enums
{
	enum class eLayerType
	{
		None = 0,
		Camera,
		Monster,
		Player,
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
}