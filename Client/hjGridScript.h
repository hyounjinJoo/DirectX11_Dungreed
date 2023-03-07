#pragma once
#include "hjScript.h"
#include "hjCamera.h"

namespace hj
{
    class GridScript : public Script
    {
	public:
		GridScript();
		virtual ~GridScript();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

		void SetCamera(Camera* camera) { mCamera = camera; }

	private:
		Camera* mCamera;
		float	mMeshScale;
	};
}
