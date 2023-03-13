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
		colliderID.left = left->colliderID;
		colliderID.right = right->colliderID;

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
		eColliderType leftType = left->GetType();
		eColliderType rightType = right->GetType();

		if (leftType != rightType)
			return;

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
				left->SetState(eCollisionState::CollisionEnter);
				right->SetState(eCollisionState::CollisionEnter);
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

				left->SetState(eCollisionState::CollisionStay);
				right->SetState(eCollisionState::CollisionStay);
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

				left->SetState(eCollisionState::CollisionExit);
				right->SetState(eCollisionState::CollisionExit);

			}
			else
			{
				left->SetState(eCollisionState::CollisionNot);
				right->SetState(eCollisionState::CollisionNot);

				mCollisionMap.erase(iter);
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
		eColliderType leftType = left->GetType();
		eColliderType rightType = right->GetType();

		switch (leftType)
		{
		case eColliderType::Rect:
			if (rightType == eColliderType::Rect)
				return IntersectRectToRect(left, right);
			break;
		case eColliderType::Circle:
			if (rightType == eColliderType::Circle)
				return IntersectCircleToCircle(left, right);
			break;
		default:
			break;
		}

		return false;
	}

	bool CollisionManager::IntersectRectToRect(Collider2D* left, Collider2D* right)
	{
		Vector3 arrLocalPos[4] =
		{
			Vector3{-0.5f, 0.5f, 0.f}
			,Vector3{0.5f, 0.5f, 0.f}
			,Vector3{0.5f, -0.5f, 0.f}
			,Vector3{-0.5f, -0.5f, 0.f}
		};

		Transform* leftTr = left->GetOwner()->GetComponent<Transform>();
		Transform* rightTr = right->GetOwner()->GetComponent<Transform>();

		Matrix leftMat = leftTr->GetWorldMatrix();
		Matrix rightMat = rightTr->GetWorldMatrix();

		// 분리축 벡터 (투영 벡터)
		Vector3 Axis[4] = {};
		Axis[0] = Vector3::Transform(arrLocalPos[1], leftMat);
		Axis[1] = Vector3::Transform(arrLocalPos[3], leftMat);
		Axis[2] = Vector3::Transform(arrLocalPos[1], rightMat);
		Axis[3] = Vector3::Transform(arrLocalPos[3], rightMat);

		Axis[0] -= Vector3::Transform(arrLocalPos[0], leftMat);
		Axis[1] -= Vector3::Transform(arrLocalPos[0], leftMat);
		Axis[2] -= Vector3::Transform(arrLocalPos[0], rightMat);
		Axis[3] -= Vector3::Transform(arrLocalPos[0], rightMat);

		Vector3 leftScale = Vector3(left->GetSize().x, left->GetSize().y, 1.f);
		Axis[0] = Axis[0] * leftScale;
		Axis[1] = Axis[1] * leftScale;

		Vector3 rightScale = Vector3(right->GetSize().x, right->GetSize().y, 1.f);
		Axis[2] = Axis[2] * rightScale;
		Axis[3] = Axis[3] * rightScale;

		for (size_t i = 0; i < 4; ++i)
		{
			Axis[i].z = 0.f;
		}

		Vector3 vc = leftTr->GetPosition() - rightTr->GetPosition();
		vc.z = 0.f;

		Vector3 centerDir = vc;

		for (size_t i = 0; i < 4; ++i)
		{
			Vector3 vA = Axis[i];

			float projDist = 0.f;
			for (size_t j = 0; j < 4; ++j)
			{
				projDist += fabsf(Axis[j].Dot(vA) / 2.f);
			}

			if (projDist < fabsf(centerDir.Dot(vA)))
			{
				return false;
			}
		}

		return true;
	}

	bool CollisionManager::IntersectCircleToCircle(Collider2D* left, Collider2D* right)
	{
		Vector3 leftPos = left->GetPosition();
		Vector3 rightPos = right->GetPosition();

		float centerDist = Vector3::Distance(leftPos, rightPos);

		Vector2 leftSize = left->GetSize();
		Vector2 rightSize = right->GetSize();
		float	leftRadius = leftSize.x >= leftSize.y ? leftSize.x : leftSize.y;
		float	rightRadius = rightSize.x >= rightSize.y ? rightSize.x : rightSize.y;

		float radiusDist = leftRadius * 0.5f + rightRadius * 0.5f;

		if (radiusDist < centerDist)
			return false;

		return true;
	}

	bool CollisionManager::Intersect3D(Collider3D* left, Collider3D* right)
	{
		return true;
	}
}