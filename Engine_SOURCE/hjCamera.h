#pragma once
#include "hjComponent.h"

namespace hj
{
	using namespace math;
	class Camera : public Component
	{
	public:
		enum eProjectionType
		{
			Perspective,
			Orthographic,
		};

		__forceinline static Matrix& GetGpuViewMatrix() { return View; }
		__forceinline static Matrix& GetGpuProjectionMatrix() { return Projection; }
		__forceinline static void SetGpuViewMatrix(Matrix view) { View = view; }
		__forceinline static void SetGpuProjectionMatrix(Matrix projection) { Projection = projection; }

		Camera();
		virtual ~Camera();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void CreateViewMatrix();
		void CreateProjectionMatrix();
		void RegisterCameraInRenderer();

		void TurnLayerMask(eLayerType layer, bool enable = true);
		void EnableLayerMask() { mLayerMasks.set(); }
		void DisableLayerMask() { mLayerMasks.reset(); }

		void SetProjectionType(eProjectionType type) { mType = type; }

		float GetScale() { return mScale; }
		Matrix& GetViewMatrix() { return mView; }
		Matrix& GetProjectionMatrix() { return mProjection; }

		void SetScale(float scale) { mScale = scale; }

	private:
		void sortGameObjects();
		void renderOpaque();
		void renderCutout();
		void renderTransparent();
		void pushGameObjectToRenderingModes(GameObject* gameObj);

		static bool compareZPosInSameRenderMode(GameObject* src, GameObject* dest);

	private:
		static Matrix View;
		static Matrix Projection;

		// 다중 카메라 지원을 위해 멤버 변수로 뷰, 투영행렬을 추가로 관리
		Matrix mView;
		Matrix mProjection;

		eProjectionType mType;
		float mAspectRatio;

		float mNear;
		float mFar;
		float mScale;

		std::bitset<(UINT)eLayerType::End> mLayerMasks;
		std::vector<GameObject*> mOpaqueGameObjects;
		std::vector<GameObject*> mCutoutGameObjects;
		std::vector<GameObject*> mTransparentGameObjects;
	};
}
