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

		// UI�� ���� Mouse Collision ����
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
		// �� �浹ü�� ������ ID Ȯ��
		ColliderID colliderID = ColliderID{};
		
		colliderID.left = left->GetColliderID();
		colliderID.right = right->GetColliderID();

		// ���� �浹 ������ �˻��Ѵ�.
		// ���࿡ �浹������ ���� ���¶��
		// �浹 ������ �������ش�.
		std::map<UINT64, bool>::iterator iter = mCollisionMap.find(colliderID.id);
		if (iter == mCollisionMap.end())
		{
			mCollisionMap.insert(std::make_pair(colliderID.id, false));
			iter = mCollisionMap.find(colliderID.id);
		}

		// �浹 üũ�� �����Ѵ�.
		bool triggerLeft = left->IsTrigger();
		bool triggerRight = right->IsTrigger();

		if (Intersect(left, right))
		{
			// �浹 Enter
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
			else // �浹 Stay
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
		else	// �浹���� ���� ����
		{
			// �浹���� Exit
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
		// �� �浹ü�� ������ ID Ȯ��
		ColliderID colliderID = ColliderID{};
		colliderID.left = left->GetColliderID();
		colliderID.right = 0;

		// ���� �浹 ������ �˻��Ѵ�.
		// ���࿡ �浹������ ���� ���¶��
		// �浹 ������ �������ش�.
		auto iter = mCollisionMap.find(colliderID.id);
		if (iter == mCollisionMap.end())
		{
			mCollisionMap.insert(std::make_pair(colliderID.id, false));
			iter = mCollisionMap.find(colliderID.id);
		}

		// �浹 üũ�� �����Ѵ�.
		bool triggerLeft = left->IsMouseTrigger();

		// �浹üũ�� ���ش�.
		bool collision = Intersect(left, pointPos);
		if (collision) // �浹�� �� ����
		{
			// ���� �浹�� Enter
			if (iter->second == false)
			{
				if (left->IsTrigger())
					left->OnTriggerMouseEnter(pointPos);
				else
					left->OnCollisionMouseEnter(pointPos);

				iter->second = true;
				left->SetMouseState(eCollisionState::CollisionEnter);
			}
			else // �浹 ������ ���� ���� Enter
			{
				if (left->IsTrigger())
					left->OnTriggerMouseStay(pointPos);
				else
					left->OnCollisionMouseStay(pointPos);

				left->SetMouseState(eCollisionState::CollisionStay);
			}
		}
		else if(!collision) // �浹���� ��������
		{
			// �浹 ���λ��� Exit
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
		// Ʈ������(��ġ ����)�� �����´�.
		Transform* leftTr = collider->GetOwner()->GetComponent<Transform>();

		Vector3 colliderRot = leftTr->GetRotation();
		Vector3 colliderHalfScale = leftTr->GetWorldScale() * 0.5f;
		Vector3 colliderPos = leftTr->GetPosition();

		Vector2 dirX = Vector2(cosf(colliderRot.z), sinf(colliderRot.z));
		Vector2 dirY = Vector2(-sinf(colliderRot.z), cosf(colliderRot.z));
		dirY.x = 0.f;
		// OBB�� �߽ɰ� �� ���� ������� �Ÿ��� ���Ѵ�.
		Vector2 pointRel = pointPos - Vector2(colliderPos.x, colliderPos.y);

		// OBB�� ����,���� �࿡ �Ÿ��� ����
		float dotX = pointRel.Dot(dirX);
		float dotY = pointRel.Dot(dirY);

		// ������ �Ÿ��� �浹ü ũ���� ��(x,y ��� �ش�)���� �۴ٸ� ���� OBB ���ο� �����Ѵ�.
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

		// �� Ʈ������(��ġ ����)�� �����´�.
		Transform* leftTr = left->GetOwner()->GetComponent<Transform>();
		Transform* rightTr = right->GetOwner()->GetComponent<Transform>();

		// ���� ����� �����´�.
		Matrix leftMat = leftTr->GetWorldMatrix();
		Matrix rightMat = rightTr->GetWorldMatrix();

		// �и��� ���� (���� ����)
		Vector3 Axis[4] = {};

		// �浹ü�� ũ�⸸ŭ �� Ȯ��
		Vector3 leftScale = Vector3(left->GetSize().x, left->GetSize().y, 1.f);
		Matrix finalLeft = Matrix::CreateScale(leftScale);
		finalLeft *= leftMat;

		Vector3 rightScale = Vector3(right->GetSize().x, right->GetSize().y, 1.f);
		Matrix finalRight = Matrix::CreateScale(rightScale);
		finalRight *= rightMat;

		Axis[0] = Vector3::Transform(arrLocalPos[1], finalLeft);	// Left �������� Ȯ��
		Axis[1] = Vector3::Transform(arrLocalPos[3], finalLeft);	// Left ���ϴ����� Ȯ��
		Axis[2] = Vector3::Transform(arrLocalPos[1], finalRight);	// Right �������� Ȯ��
		Axis[3] = Vector3::Transform(arrLocalPos[3], finalRight);	// Right ���ϴ����� Ȯ��

		Axis[0] -= Vector3::Transform(arrLocalPos[0], finalLeft);	// Left�� ���κ� ����
		Axis[1] -= Vector3::Transform(arrLocalPos[0], finalLeft);	// Left�� ���κ� ����
		Axis[2] -= Vector3::Transform(arrLocalPos[0], finalRight);	// Right�� ���κ� ����
		Axis[3] -= Vector3::Transform(arrLocalPos[0], finalRight);	// Right�� ���κ� ����

		// 2D ����̱� ������ Z���� 0���� ����
		for (size_t i = 0; i < 4; ++i)
		{
			Axis[i].z = 0.f;
		}

		// �� OBB�� �����Ÿ� ����
		Vector3 BetweenCenterVector = leftTr->GetWorldPosition() - rightTr->GetWorldPosition();
		BetweenCenterVector.z = 0.f;

		Vector3 projAxis = Vector3::Zero;
		float projDist = 0.f;
		float projCenter = 0.f;
		for (size_t i = 0; i < 4; ++i)
		{
			// ���� ���� �� ���� �� ���� �Ÿ� �ʱ�ȭ
			projAxis = Axis[i];
			projDist = 0.f;

			for (size_t j = 0; j < 4; ++j)
			{	
				// �� ���� ���� �࿡ �������� ���� ���� �Ÿ�
				projDist += fabsf(projAxis.Dot(Axis[j] * 0.5f));
			}
			projCenter = fabsf(projAxis.Dot(BetweenCenterVector));

			// �и��� ���� ���� �˻�, �ϳ��� �����Ѵٸ� �浹 ������ ���� ����
			if (projDist < projCenter)
			{
				return false;
			}
		}
		// ������� �����Ѵٸ� �и����� �������� �����Ƿ� �浹 ���� ����
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

		// ���� �浹 ������ �˻��Ѵ�.
		// ���࿡ �浹������ �ִٸ�
		// �浹 ������ �浹���� ���� ���·� �������ش�.
		std::map<UINT64, bool>::iterator iter = mCollisionMap.find(colliderID.id);
		if (iter != mCollisionMap.end())
		{
			iter->second = false;
			left->SetState(eCollisionState::CollisionNot);
			right->SetState(eCollisionState::CollisionNot);
		}
	}

}