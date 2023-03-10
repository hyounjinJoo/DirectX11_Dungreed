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
		static void ColliderCollision(Collider* left, Collider* right);
		static bool Intersect(Collider* left, Collider* right);
		static bool Intersect2D(class Collider2D* left, class Collider2D* right);
		static bool IntersectRectToRect(class Collider2D* left, class Collider2D* right);
		static bool IntersectCircleToCircle(class Collider2D* left, class Collider2D* right);

		static bool Intersect3D(class Collider3D* left, class Collider3D* right);

	private:
		static std::bitset<(UINT)eLayerType::End> mLayerCollisionMatrix[(UINT)eLayerType::End];
		static std::map<UINT64, bool> mCollisionMap;
	};
}
