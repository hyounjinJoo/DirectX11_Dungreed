#include "hjCollisionManager.h"
#include "hjSceneManager.h"
#include "hjCollider2D.h"
#include "hjCollider3D.h"

namespace hj
{
	std::bitset<(UINT)eLayerType::End> CollisionManager::mLayerCollisionMatrix[(UINT)eLayerType::End] = {};
	std::map<UINT64, bool> CollisionManager::mCollisionMap;

	void CollisionManager::Initialize()
	{
	}

	void CollisionManager::Update()
	{
		Scene* scene = SceneManager::GetActiveScene();

		if (nullptr == scene)
			return;

		for (UINT row = 0; row < (UINT)eLayerType::End; ++row)
		{
			for (UINT column = 0; column < (UINT)eLayerType::End; ++column)
			{
				if (mLayerCollisionMatrix[row][column])
				{
					LayerCollision(scene, (eLayerType)row, (eLayerType)column);
				}
			}
		}
	}

	void CollisionManager::FixedUpdate()
	{
	}

	void CollisionManager::Render()
	{
	}

	void CollisionManager::CollisionLayerCheck(eLayerType left, eLayerType right, bool enable)
	{
		int row = 0;
		int column = 0;

		if (static_cast<UINT>(left) <= static_cast<UINT>(right))
		{
			row = static_cast<UINT>(left);
			column = static_cast<UINT>(right);
		}
		else
		{
			row = static_cast<UINT>(right);
			column = static_cast<UINT>(left);
		}

		mLayerCollisionMatrix[row][column] = enable;
	}

	void CollisionManager::LayerCollision(Scene* scene, eLayerType left, eLayerType right)
	{
		const std::vector<GameObject*>& lefts = scene->GetGameObjects(left);
		const std::vector<GameObject*>& rights = scene->GetGameObjects(right);

		if (lefts.empty() || rights.empty())
			return;

		for (GameObject* left : lefts)
		{
			if (left->GetState() != GameObject::Active)
				continue;
			if (left->GetComponent<Collider2D>() == nullptr && left->GetComponent<Collider3D>() == nullptr)
				continue;

			for (GameObject* right : rights)
			{
				if (right->GetState() != GameObject::Active)
					continue;
				if (right->GetComponent<Collider2D>() == nullptr && right->GetComponent<Collider3D>() == nullptr)
					continue;
				if(left == right)
					continue;

				if (left->GetComponent<Collider2D>() && right->GetComponent<Collider2D>())
					ColliderCollision(left->GetComponent<Collider2D>(), right->GetComponent<Collider2D>());

				if (left->GetComponent<Collider3D>() && right->GetComponent<Collider3D>())
					ColliderCollision(left->GetComponent<Collider3D>(), right->GetComponent<Collider3D>());
			}
		}
	}

	void CollisionManager::ColliderCollision(Collider* left, Collider* right)
	{
		// 두 충돌체로 구성된 ID 확인
		ColliderID colliderID;
		colliderID.left = reinterpret_cast<UINT32>(left);
		colliderID.right = reinterpret_cast<UINT32>(right);

		// 이전 충돌 정보를 검색한다.
		// 만약에 충돌정보가 없는 상태라면
		// 충돌 정보를 생성해준다.
		std::map<UINT64, bool>::iterator iter = mCollisionMap.find(colliderID.id);
		if (iter == mCollisionMap.end())
		{
			mCollisionMap.insert(std::make_pair(colliderID.id, false));
			iter = mCollisionMap.find(colliderID.id);
		}

		// 충돌 체크를 진행한다.
		bool triggerLeft = left->IsTrigger();
		bool triggerRight = right->IsTrigger();
		if (Intersect(left, right))
		{
			// 충돌 Enter
			if (iter->second == false)
			{
				if (triggerLeft)
					left->OnTriggerEnter(right);
				else
					left->OnCollisionEnter(right);

				if (triggerRight)
					right->OnTriggerEnter(left);
				else
					right->OnCollisionEnter(left);

				iter->second = true;
			}
			else // 충돌 Stay
			{
				if (triggerLeft)
					left->OnTriggerStay(right);
				else
					left->OnCollisionStay(right);

				if (triggerRight)
					right->OnTriggerStay(left);
				else
					right->OnCollisionStay(left);
			}
		}
		else	// 충돌하지 않은 상태
		{
			// 충돌에서 Exit
			if (iter->second == true)
			{
				if (triggerLeft)
					left->OnTriggerExit(right);
				else
					left->OnCollisionExit(right);

				if (triggerRight)
					right->OnTriggerExit(left);
				else
					right->OnCollisionExit(left);

				iter->second = false;
			}
		}
	}

	bool CollisionManager::Intersect(Collider* left, Collider* right)
	{
		if (dynamic_cast<Collider2D*>(left))
		{
			return Intersect2D(static_cast<Collider2D*>(left), static_cast<Collider2D*>(right));
		}
		else
		{
			return Intersect3D(static_cast<Collider3D*>(left), static_cast<Collider3D*>(right));
		}
	}

	bool CollisionManager::Intersect2D(Collider2D* left, Collider2D* right)
	{
		return true;
	}

	bool CollisionManager::Intersect3D(Collider3D* left, Collider3D* right)
	{
		return true;
	}
}