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
	}

	void Collider2D::FixedUpdate()
	{
		if (nullptr == mTransform)
			return;

		Vector3 scale = mTransform->GetScale();
		scale *= Vector3(mSize.x, mSize.y, 1.f);

		Vector3 rotation = mTransform->GetRotation();

		Vector3 position = mTransform->GetPosition();
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
		meshAttribute.type = mType;
		meshAttribute.state = mState;

		renderer::debugMeshes.push_back(meshAttribute);
	}

	void Collider2D::Render()
	{
	}

	void Collider2D::SetSize(Vector2 size)
	{
		switch (mType)
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
}