#pragma once
#include "CommonInclude.h"
#include "hjCollider.h"

namespace hj
{
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

		static void CollisionLayerCheck(eLayerType left, eLayerType right, bool enable = true);
		static void LayerCollision(class Scene* scene, eLayerType left, eLayerType right);
		static void MouseCollision(class Scene* scene, eLayerType layerType);
		static void ColliderCollision(Collider* left, Collider* right);
		static void ColliderCollision(Collider* left, const hj::math::Vector2& pointPos);
		static bool Intersect(Collider* left, Collider* right);
		static bool Intersect(Collider* left, const hj::math::Vector2& pointPos);
		static bool Intersect2D(class Collider2D* left, class Collider2D* right);
		static bool Intersect2DUI(class Collider2D* collider, const hj::math::Vector2& pointPos);
		static bool IntersectRectToPoint(class Collider2D* collider, const hj::math::Vector2& pointPos);
		static bool IntersectCircleToPoint(class Collider2D* collider, const hj::math::Vector2& pointPos);
		static bool IntersectRectToRect(class Collider2D* left, class Collider2D* right);
		static bool IntersectCircleToCircle(class Collider2D* left, class Collider2D* right);

		static bool Intersect3D(class Collider3D* left, class Collider3D* right);
		static bool Intersect3DUI(class Collider3D* collider, const hj::math::Vector2& pointPos);

	private:
		static std::bitset<(UINT)eLayerType::End> mLayerCollisionMatrix[(UINT)eLayerType::End];
		static std::map<UINT64, bool> mCollisionMap;
	};
}
