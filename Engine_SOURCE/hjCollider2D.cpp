#include "pch.h"
#include "hjCollider2D.h"
#include "hjGameObject.h"
#include "hjRenderer.h"
#include "hjScript.h"

namespace hj
{
	Collider2D::Collider2D()
		: Collider()
		, mTransform(nullptr)
		, mSize(Vector2::One)
		, mCenter(Vector2::Zero)
		, mPosition(Vector3::Zero)
		, mRadius(50.f)
		, mPrevPos(Vector3::Zero)
		, mLTRBPos(Vector4::Zero)
	{
		SetColliderType(eColliderType::Rect);
	}

	Collider2D::Collider2D(const Collider2D& collider2D)
		: Collider(collider2D)
		, mTransform(nullptr)
		, mSize(collider2D.mSize)
		, mCenter(collider2D.mCenter)
		, mPosition(collider2D.mPosition)
		, mRadius(collider2D.mRadius)
		, mPrevPos(collider2D.mPrevPos)
		, mLTRBPos(collider2D.mLTRBPos)
	{
	}

	Collider2D::~Collider2D()
	{
	}

	void Collider2D::Initialize()
	{
		mTransform = GetOwner()->GetComponent<Transform>();
	}

	void Collider2D::Update()
	{
		if (mPrevPos != mPosition)
		{
			mPrevPos = mPosition;
			
			Vector3 scale = mTransform->GetWorldScale();
			scale *= Vector3(mSize.x, mSize.y, 1.f);
			mLTRBPos.x = mPosition.x - fabsf(scale.x * 0.5f);
			mLTRBPos.y = mPosition.y + fabsf(scale.y * 0.5f);
			mLTRBPos.z = mPosition.x + fabsf(scale.x * 0.5f);
			mLTRBPos.w = mPosition.y - fabsf(scale.y * 0.5f);
		}
	}

	void Collider2D::FixedUpdate()
	{
		if (nullptr == mTransform)
			return;

		Vector3 scale = mTransform->GetWorldScale();
		scale *= Vector3(mSize.x, mSize.y, 1.f);

		Vector3 rotation = mTransform->GetWorldRotation();

		Vector3 position = mTransform->GetWorldPosition();
		mPosition = position + Vector3(mCenter.x, mCenter.y, 0.f);

		Matrix scaleMatrix = Matrix::CreateScale(scale);
		Matrix rotationMatrix;
		rotationMatrix = Matrix::CreateRotationX(rotation.x);
		rotationMatrix *= Matrix::CreateRotationY(rotation.y);
		rotationMatrix *= Matrix::CreateRotationZ(rotation.z);

		Matrix positionMatrix;
		positionMatrix.Translation(Vector3(mPosition.x, mPosition.y, mPosition.z));

		Matrix worldMatrix = scaleMatrix * rotationMatrix * positionMatrix;

		DebugMesh meshAttribute = {};
		meshAttribute.position = Vector3(mPosition.x, mPosition.y, mPosition.z);
		meshAttribute.radius = mRadius;
		meshAttribute.rotation = rotation;
		meshAttribute.scale = scale;
		meshAttribute.type = mColliderType;
		meshAttribute.state = mState;

		renderer::debugMeshes.push_back(meshAttribute);
	}

	void Collider2D::Render()
	{
	}

	hj::Component* Collider2D::Clone() const
	{
		return new Collider2D(*this);
	}

	void Collider2D::SetSize(Vector2 size)
	{
		switch (mColliderType)
		{
		case hj::enums::eColliderType::Circle:
			mSize = Vector2::One;
			mRadius = size.x >= size.y ? size.x : size.y;
			break;
		case hj::enums::eColliderType::None:
		case hj::enums::eColliderType::Rect:
		case hj::enums::eColliderType::Box:
		case hj::enums::eColliderType::Sphere:
		case hj::enums::eColliderType::End:
		default:
			mSize = size;
			break;
		}
	}

	void Collider2D::OnCollisionEnter(Collider* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollisionEnter(collider);
		}
	}
	
	void Collider2D::OnCollisionStay(Collider* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollisionStay(collider);
		}
	}
	
	void Collider2D::OnCollisionExit(Collider* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollisionExit(collider);
		}
	}
	
	void Collider2D::OnTriggerEnter(Collider* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTriggerEnter(collider);
		}
	}
	
	void Collider2D::OnTriggerStay(Collider* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTriggerStay(collider);
		}
	}
	
	void Collider2D::OnTriggerExit(Collider* collider)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTriggerExit(collider);
		}
	}
	void Collider2D::OnCollisionMouseEnter(const Vector2& mousePos)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollisionMouseEnter(mousePos);
		}
	}
	void Collider2D::OnCollisionMouseStay(const Vector2& mousePos)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollisionMouseStay(mousePos);
		}
	}
	void Collider2D::OnCollisionMouseExit(const Vector2& mousePos)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollisionMouseExit(mousePos);
		}
	}
	void Collider2D::OnTriggerMouseEnter(const Vector2& mousePos)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTriggerMouseEnter(mousePos);
		}
	}
	void Collider2D::OnTriggerMouseStay(const Vector2& mousePos)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTriggerMouseStay(mousePos);
		}
	}
	void Collider2D::OnTriggerMouseExit(const Vector2& mousePos)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTriggerMouseExit(mousePos);
		}
	}
}