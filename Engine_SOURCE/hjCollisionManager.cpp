#include "pch.h"
#include "hjCollisionManager.h"
#include "hjSceneManager.h"
#include "hjCollider2D.h"
#include "hjCollider3D.h"
#include "hjInput.h"
#include "hjGameObject.h"
#include "hjObject.h"

namespace hj
{
	std::bitset<(UINT)eLayerType::End> CollisionManager::mLayerCollisionMatrix[(UINT)eLayerType::End] = {};
	std::map<UINT64, bool> CollisionManager::mCollisionMap;

	void CollisionManager::Initialize()
	{
	}

	void CollisionManager::Update()
	{		
	}

	void CollisionManager::FixedUpdate()
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

		// UI에 대한 Mouse Collision 수행
		MouseCollision(scene, eLayerType::UI);
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

			Collider2D* left2DCollider = left->GetComponent<Collider2D>();
			Collider3D* left3DCollider = left->GetComponent<Collider3D>();

			if (left2DCollider == nullptr && left3DCollider == nullptr)
				continue;
			if (left2DCollider && left2DCollider->IsActive() == false)
				continue;
			if (left3DCollider && left3DCollider->IsActive() == false)
				continue;

			for (GameObject* right : rights)
			{
				if (right->GetState() != GameObject::Active)
					continue;

				Collider2D* right2DCollider = right->GetComponent<Collider2D>();
				Collider3D* right3DCollider = right->GetComponent<Collider3D>();

				if (right2DCollider == nullptr && right3DCollider == nullptr)
					continue;
				if (right2DCollider && right2DCollider->IsActive() == false)
					continue;
				if (right3DCollider && right3DCollider->IsActive() == false)
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

	void CollisionManager::MouseCollision(class Scene* scene, eLayerType layerType)
	{
		const std::vector<GameObject*>& gameObjects = scene->GetGameObjects(layerType);

		if (gameObjects.empty())
			return;

		Vector2 mousePos = Input::GetMouseWorldPosition();
		for (GameObject* object : gameObjects)
		{
			if (object->GetState() != GameObject::Active)
				continue;
			if (object->GetComponent<Collider2D>() == nullptr)
				continue;

			Collider2D* uiCollider = object->GetComponent<Collider2D>();
			if (uiCollider && uiCollider->IsUsingMouseCollision())
				ColliderCollision(uiCollider, mousePos);
		}
	}

	void CollisionManager::ColliderCollision(Collider* left, Collider* right)
	{
		// 두 충돌체로 구성된 ID 확인
		ColliderID colliderID = ColliderID{};
		
		colliderID.left = left->GetColliderID();
		colliderID.right = right->GetColliderID();

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

				//mCollisionMap.erase(iter);
			}
		}
	}

	void CollisionManager::ColliderCollision(Collider* left, const hj::math::Vector2& pointPos)
	{
		// 두 충돌체로 구성된 ID 확인
		ColliderID colliderID = ColliderID{};
		colliderID.left = left->GetColliderID();
		colliderID.right = 0;

		// 이전 충돌 정보를 검색한다.
		// 만약에 충돌정보가 없는 상태라면
		// 충돌 정보를 생성해준다.
		auto iter = mCollisionMap.find(colliderID.id);
		if (iter == mCollisionMap.end())
		{
			mCollisionMap.insert(std::make_pair(colliderID.id, false));
			iter = mCollisionMap.find(colliderID.id);
		}

		// 충돌 체크를 진행한다.
		bool triggerLeft = left->IsMouseTrigger();

		// 충돌체크를 해준다.
		bool collision = Intersect(left, pointPos);
		if (collision) // 충돌을 한 상태
		{
			// 최초 충돌중 Enter
			if (iter->second == false)
			{
				if (left->IsTrigger())
					left->OnTriggerMouseEnter(pointPos);
				else
					left->OnCollisionMouseEnter(pointPos);

				iter->second = true;
				left->SetMouseState(eCollisionState::CollisionEnter);
			}
			else // 충돌 중이지 않은 상태 Enter
			{
				if (left->IsTrigger())
					left->OnTriggerMouseStay(pointPos);
				else
					left->OnCollisionMouseStay(pointPos);

				left->SetMouseState(eCollisionState::CollisionStay);
			}
		}
		else if(!collision) // 충돌하지 않은상태
		{
			// 충돌 중인상태 Exit
			if (iter->second == true)
			{
				if (left->IsTrigger())
					left->OnTriggerMouseExit(pointPos);
				else
					left->OnCollisionMouseExit(pointPos);

				iter->second = false;
				left->SetMouseState(eCollisionState::CollisionExit);
			}
			else
			{
				left->SetMouseState(eCollisionState::CollisionNot);
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

	bool CollisionManager::Intersect(Collider* left, const hj::math::Vector2& pointPos)
	{
		if (dynamic_cast<Collider2D*>(left))
		{
			return Intersect2DUI(static_cast<Collider2D*>(left), pointPos);
		}
		else
		{
			return Intersect3DUI(static_cast<Collider3D*>(left), pointPos);
		}
	}

	bool CollisionManager::Intersect2D(Collider2D* left, Collider2D* right)
	{
		eColliderType leftType = left->GetColliderType();
		eColliderType rightType = right->GetColliderType();

		switch (leftType)
		{
		case eColliderType::Rect:
			if (rightType == eColliderType::Rect)
				return IntersectRectToRect(left, right);
			else if (rightType == eColliderType::Circle)
				return IntersectRectToCircle(left, right);
			break;
		case eColliderType::Circle:
			if (rightType == eColliderType::Circle)
				return IntersectCircleToCircle(left, right);
			else if (rightType == eColliderType::Rect)
				return IntersectRectToCircle(right, left);
			break;
		default:
			break;
		}

		return false;
	}

	bool CollisionManager::Intersect2DUI(Collider2D* collider, const hj::math::Vector2& pointPos)
	{
		eColliderType colliderType = collider->GetColliderType();

		switch (colliderType)
		{
		case eColliderType::Rect:
			if (colliderType == eColliderType::Rect)
				return IntersectRectToPoint(collider, pointPos);
			break;
		case eColliderType::Circle:
			if (colliderType == eColliderType::Circle)
				return IntersectCircleToPoint(collider, pointPos);
			break;
		default:
			break;
		}

		return false;
	}

	bool CollisionManager::IntersectRectToPoint(Collider2D* collider, const hj::math::Vector2& pointPos)
	{
		// 트랜스폼(위치 정보)를 가져온다.
		Transform* leftTr = collider->GetOwner()->GetComponent<Transform>();

		Vector3 colliderRot = leftTr->GetRotation();
		Vector3 colliderHalfScale = leftTr->GetWorldScale() * 0.5f;
		Vector3 colliderPos = leftTr->GetPosition();

		Vector2 dirX = Vector2(cosf(colliderRot.z), sinf(colliderRot.z));
		Vector2 dirY = Vector2(-sinf(colliderRot.z), cosf(colliderRot.z));
		dirY.x = 0.f;
		// OBB의 중심과 점 간의 상대적인 거리를 구한다.
		Vector2 pointRel = pointPos - Vector2(colliderPos.x, colliderPos.y);

		// OBB의 가로,세로 축에 거리를 투영
		float dotX = pointRel.Dot(dirX);
		float dotY = pointRel.Dot(dirY);

		// 투영한 거리가 충돌체 크기의 반(x,y 모두 해당)보다 작다면 점이 OBB 내부에 존재한다.
		if (fabsf(dotX) <= colliderHalfScale.x && fabsf(dotY) <= colliderHalfScale.y)
		{
			return true;
		}

		return false;
	}

	bool CollisionManager::IntersectCircleToPoint(Collider2D* collider, const hj::math::Vector2& pointPos)
	{
		Vector3 colliderPos = collider->GetPosition();
		colliderPos.z = 0.f;
		Vector3 pointXYZ = Vector3(pointPos, 0);

		float centerDist = Vector3::Distance(colliderPos, pointXYZ);

		Vector2 colliderSize = collider->GetSize();
		float	colliderhalfRadius = colliderSize.x >= colliderSize.y ? colliderSize.x : colliderSize.y;
		colliderhalfRadius *= 0.5f;

		float dist = Vector3::Distance(colliderPos, pointXYZ);

		if (colliderhalfRadius < dist)
			return false;

		return true;
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

		// 각 트랜스폼(위치 정보)를 가져온다.
		Transform* leftTr = left->GetOwner()->GetComponent<Transform>();
		Transform* rightTr = right->GetOwner()->GetComponent<Transform>();

		// 월드 행렬을 가져온다.
		Matrix leftMat = leftTr->GetWorldMatrix();
		Matrix rightMat = rightTr->GetWorldMatrix();

		// 분리축 벡터 (투영 벡터)
		Vector3 Axis[4] = {};

		// 충돌체의 크기만큼 축 확장
		Vector3 leftScale = Vector3(left->GetSize().x, left->GetSize().y, 1.f);
		Matrix finalLeft = Matrix::CreateScale(leftScale);
		finalLeft *= leftMat;

		Vector3 rightScale = Vector3(right->GetSize().x, right->GetSize().y, 1.f);
		Matrix finalRight = Matrix::CreateScale(rightScale);
		finalRight *= rightMat;

		Axis[0] = Vector3::Transform(arrLocalPos[1], finalLeft);	// Left 우상단으로 확장
		Axis[1] = Vector3::Transform(arrLocalPos[3], finalLeft);	// Left 좌하단으로 확장
		Axis[2] = Vector3::Transform(arrLocalPos[1], finalRight);	// Right 우상단으로 확장
		Axis[3] = Vector3::Transform(arrLocalPos[3], finalRight);	// Right 좌하단으로 확장

		Axis[0] -= Vector3::Transform(arrLocalPos[0], finalLeft);	// Left의 가로변 길이
		Axis[1] -= Vector3::Transform(arrLocalPos[0], finalLeft);	// Left의 세로변 길이
		Axis[2] -= Vector3::Transform(arrLocalPos[0], finalRight);	// Right의 가로변 길이
		Axis[3] -= Vector3::Transform(arrLocalPos[0], finalRight);	// Right의 세로변 길이

		// 2D 계산이기 때문에 Z값을 0으로 제한
		for (size_t i = 0; i < 4; ++i)
		{
			Axis[i].z = 0.f;
		}

		// 두 OBB의 원점거리 벡터
		Vector3 BetweenCenterVector = leftTr->GetWorldPosition() - rightTr->GetWorldPosition();
		BetweenCenterVector.z = 0.f;

		Vector3 projAxis = Vector3::Zero;
		float projDist = 0.f;
		float projCenter = 0.f;
		for (size_t i = 0; i < 4; ++i)
		{
			// 투영 기준 축 설정 및 투영 거리 초기화
			projAxis = Axis[i];
			projDist = 0.f;

			for (size_t j = 0; j < 4; ++j)
			{	
				// 각 변의 반을 축에 투영시켜 전부 더한 거리
				projDist += fabsf(projAxis.Dot(Axis[j] * 0.5f));
			}
			projCenter = fabsf(projAxis.Dot(BetweenCenterVector));

			// 분리축 존재 여부 검사, 하나라도 존재한다면 충돌 중이지 않은 상태
			if (projDist < projCenter)
			{
				return false;
			}
		}
		// 여기까지 도달한다면 분리축이 존재하지 않으므로 충돌 중인 상태
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

	bool CollisionManager::IntersectRectToCircle(class Collider2D* left, class Collider2D* right)
	{
		// Calc Distance Dir Vector
		Vector2 circlePos = right->GetOwner()->GetPositionXY();
		Vector2 rectPos = left->GetOwner()->GetPositionXY();
		Vector2 centerDistDir = circlePos - rectPos;

		// Calc Intersection Point between Rect and Dir Vector
		Vector2 rectHalfExtents = left->GetOwner()->GetScaleXY() * 0.5f;
		float dotXLength = (Vector2::Right).Dot(centerDistDir);
		dotXLength = std::clamp<float>(dotXLength, -rectHalfExtents.x, rectHalfExtents.x);
		float dotYLength = (Vector2::Up).Dot(centerDistDir);
		dotYLength = std::clamp<float>(dotYLength, -rectHalfExtents.y, rectHalfExtents.y);

		Vector2 closestPoint = rectPos + dotXLength * Vector2::Right + dotYLength * Vector2::Up - circlePos;

		// Calc Each Vector's length
		float lengthSquared = closestPoint.LengthSquared();
		float circleRadius = right->GetSize().x * 0.5f;
		float circleRadiusSquared = circleRadius * circleRadius;

		// Check Collision
		bool isCollision = lengthSquared <= circleRadiusSquared;
		return isCollision;
	}

	bool CollisionManager::Intersect3D(Collider3D* left, Collider3D* right)
	{
		return true;
	}

	bool CollisionManager::Intersect3DUI(class Collider3D* collider, const hj::math::Vector2& pointPos)
	{
		return true;
	}

	void CollisionManager::ResetCollisionInfo(Collider* left, Collider* right)
	{
		if (!left || !right)
			return;

		ColliderID colliderID = ColliderID{};

		colliderID.left = left->GetColliderID();
		colliderID.right = right->GetColliderID();

		// 이전 충돌 정보를 검색한다.
		// 만약에 충돌정보가 있다면
		// 충돌 정보를 충돌하지 않음 상태로 변경해준다.
		std::map<UINT64, bool>::iterator iter = mCollisionMap.find(colliderID.id);
		if (iter != mCollisionMap.end())
		{
			iter->second = false;
			left->SetState(eCollisionState::CollisionNot);
			right->SetState(eCollisionState::CollisionNot);
		}
	}

}