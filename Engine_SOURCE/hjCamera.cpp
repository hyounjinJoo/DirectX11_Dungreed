#include "hjCamera.h"
#include "hjTransform.h"
#include "hjGameObject.h"
#include "hjApplication.h"
#include "hjRenderer.h"
#include "hjScene.h"
#include "hjSceneManager.h"
#include "hjMaterial.h"
#include "hjBaseRenderer.h"
#include "hjSceneManager.h"

extern hj::Application application;

namespace hj
{
	Matrix Camera::View = Matrix::Identity;
	Matrix Camera::Projection = Matrix::Identity;

	Camera::Camera()
		: Component(eComponentType::Camera)
		, mType(eProjectionType::Perspective)
		, mAspectRatio(1.f)
		, mNear(1.f)
		, mFar(1000.f)
		, mScale(1.f)
	{
		EnableLayerMask(); 
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

		RegisterCameraInRenderer();
	}
	
	void Camera::Render()
	{
		View = mView;
		Projection = mProjection;

		sortGameObjects();

		renderOpaque();
		renderCutout();
		renderTransparent();
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

		float width = static_cast<float>(winRect.right - winRect.left) * mScale;
		float height = static_cast<float>(winRect.bottom - winRect.top) * mScale;
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

	void Camera::RegisterCameraInRenderer()
	{
		eSceneType type = SceneManager::GetActiveScene()->GetSceneType();
		renderer::cameras[(UINT)type].push_back(this);
	}
	
	void Camera::TurnLayerMask(eLayerType layer, bool enable)
	{
		mLayerMasks.set((UINT)layer, enable);
	}
	
	void Camera::sortGameObjects()
	{
		mOpaqueGameObjects.clear();
		mCutoutGameObjects.clear();
		mTransparentGameObjects.clear();

		Scene* scene = SceneManager::GetActiveScene();
		for (size_t i = 0; i < (UINT)eLayerType::End; ++i)
		{
			if (mLayerMasks[i] == true)
			{
				Layer& layer = scene->GetLayer((eLayerType)i);
				GameObjects gameObjects = layer.GetGameObejcts();
				if (gameObjects.size() == 0)
					continue;

				for (GameObject* obj : gameObjects)
				{
					pushGameObjectToRenderingModes(obj);
				}
			}
		}

		std::sort(mOpaqueGameObjects.begin(), mOpaqueGameObjects.end(), &Camera::compareZPosInSameRenderMode);
		std::sort(mCutoutGameObjects.begin(), mCutoutGameObjects.end(), &Camera::compareZPosInSameRenderMode);
		std::sort(mTransparentGameObjects.begin(), mTransparentGameObjects.end(), &Camera::compareZPosInSameRenderMode);
	}
	
	void Camera::renderOpaque()
	{
		for (GameObject* obj : mOpaqueGameObjects)
		{
			if (obj == nullptr)
				continue;

			obj->Render();
		}
	}
	
	void Camera::renderCutout()
	{
		for (GameObject* obj : mCutoutGameObjects)
		{
			if (obj == nullptr)
				continue;

			obj->Render();
		}
	}
	
	void Camera::renderTransparent()
	{
		for (GameObject* obj : mTransparentGameObjects)
		{
			if (obj == nullptr)
				continue;

			obj->Render();
		}
	}
	
	void Camera::pushGameObjectToRenderingModes(GameObject* gameObj)
	{
		BaseRenderer* renderer
			= gameObj->GetComponent<BaseRenderer>();
		if (renderer == nullptr)
			return;

		std::shared_ptr<Material> material = renderer->GetMaterial();
		eRenderingMode mode = material->GetRenderingMode();

		switch (mode)
		{
		case eRenderingMode::Opaque:
			mOpaqueGameObjects.push_back(gameObj);
			break;
		case eRenderingMode::Cutout:
			mCutoutGameObjects.push_back(gameObj);
			break;
		case eRenderingMode::Transparent:
			mTransparentGameObjects.push_back(gameObj);
			break;
		default:
			break;
		}
	}
	bool Camera::compareZPosInSameRenderMode(GameObject* src, GameObject* dest)
	{
		Transform* srcTR = src->GetComponent<Transform>();
		Transform* destTR = dest->GetComponent<Transform>();

		if (nullptr == srcTR || nullptr == destTR)
			return false;

		float srcZ = srcTR->GetPosition().z;
		float destZ = destTR->GetPosition().z;

		return srcZ > destZ;
	}
}