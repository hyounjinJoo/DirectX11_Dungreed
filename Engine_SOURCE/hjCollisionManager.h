#pragma once

#include "hjEnums.h"

namespace hj
{
	class Scene;
	class Collider;
	class Collider2D;
	class Collider3D;
	namespace math
	{
		struct Vector2;
	}

	union ColliderID
	{
		struct  
		{
			UINT32 left;
			UINT32 right;
		};
		UINT64 id;
	};

	class CollisionManager
	{
	public:
		static void Initialize();
		static void Update();
		static void FixedUpdate();
		static void Render();

		static void CollisionLayerCheck(enums::eLayerType left, enums::eLayerType right, bool enable = true);
		static void LayerCollision(Scene* scene, enums::eLayerType left, enums::eLayerType right);
		static void MouseCollision(Scene* scene, enums::eLayerType layerType);
		static void ColliderCollision(Collider* left, Collider* right);
		static void ColliderCollision(Collider* left, const math::Vector2& pointPos);
		static bool Intersect(Collider* left, Collider* right);
		static bool Intersect(Collider* left, const math::Vector2& pointPos);
		static bool Intersect2D(Collider2D* left, Collider2D* right);
		static bool Intersect2DUI(Collider2D* collider, const math::Vector2& pointPos);
		static bool IntersectRectToPoint(Collider2D* collider, const math::Vector2& pointPos);
		static bool IntersectCircleToPoint(Collider2D* collider, const math::Vector2& pointPos);
		static bool IntersectRectToRect(Collider2D* left, Collider2D* right);
		static bool IntersectCircleToCircle(Collider2D* left, Collider2D* right);
		static bool IntersectRectToCircle(Collider2D* left, Collider2D* right);

		static bool Intersect3D(Collider3D* left, Collider3D* right);
		static bool Intersect3DUI(Collider3D* collider, const math::Vector2& pointPos);

		static void ResetCollisionInfo(Collider* left, Collider* right);
	private:
		static std::bitset<(UINT)enums::eLayerType::End> mLayerCollisionMatrix[(UINT)enums::eLayerType::End];
		static std::map<UINT64, bool> mCollisionMap;
	};
}
