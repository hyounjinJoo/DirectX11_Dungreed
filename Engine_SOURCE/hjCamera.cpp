#include "hjCamera.h"
#include "hjTransform.h"
#include "hjGameObject.h"
#include "hjApplication.h"

extern hj::Application application;

namespace hj
{
	Matrix Camera::mView = Matrix::Identity;
	Matrix Camera::mProjection = Matrix::Identity;

	Camera::Camera()
		: Component(eComponentType::Camera)
		, mType(eProjectionType::Perspective)
		, mAspectRatio(1.f)
		, mNear(1.f)
		, mFar(1000.f)
		, mScale(1.f)
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::Initialize()
	{
	}
	
	void Camera::Update()
	{
	}
	
	void Camera::FixedUpdate()
	{
		CreateViewMatrix();
		CreateProjectionMatrix();
	}
	
	void Camera::Render()
	{
	}

	void Camera::CreateViewMatrix()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 cameraPos = tr->GetPosition();

		// Create Translation View Matrix
		mView = Matrix::Identity;
		mView *= Matrix::CreateTranslation(-cameraPos);

		// Rotation Info
		Vector3 up = tr->Up();
		Vector3 right = tr->Right();
		Vector3 forward = tr->Forward();

		Matrix viewRotation;
		viewRotation._11 = right.x;	viewRotation._12 = up.x; viewRotation._13 = forward.x;
		viewRotation._21 = right.y;	viewRotation._22 = up.y; viewRotation._33 = forward.y;
		viewRotation._31 = right.z;	viewRotation._32 = up.z; viewRotation._33 = forward.z;

		mView *= viewRotation;
	}

	void Camera::CreateProjectionMatrix()
	{
		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);

		float width = winRect.right - winRect.left;
		float height = winRect.bottom - winRect.top;
		mAspectRatio = width / height;

		if (mType == eProjectionType::Perspective)
		{
			mProjection = Matrix::CreatePerspectiveFieldOfViewLH
			(
				XM_2PI / 6.f
				, mAspectRatio
				, mNear
				, mFar
			);
		}
		else
		{
			mProjection = Matrix::CreateOrthographicLH(width, height, mNear, mFar);
		}
	}
}