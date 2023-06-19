#pragma once
#include <hjActor.h>
#include "hjMaterial.h"

namespace hj
{
	class UIBase :
		public Actor
	{
	public:
		UIBase();
		virtual ~UIBase();

		void Initialize() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() override;

		void UseUV(bool use) { mbUseUV = use; }
		void UseCanvas(bool use) { mbUseCanvas = use; }
		void SetStartUV(const Vector2& uv) { mStartUV = uv; }
		void SetEndUV(const Vector2& uv) { mEndUV = uv; }
		void SetCanvasSize(const Vector2& size) { mCanvasSize = size; }
		void SetAtlasSize(const Vector2& size) { mAtlasSize = size; }

		void SetRenderer(class SpriteRenderer* renderer);
		void SetMaterial(std::shared_ptr<Material> material);

	protected:
		bool mbUseUV;
		Vector2 mStartUV;
		Vector2 mEndUV;
		bool mbUseCanvas;
		Vector2 mCanvasSize;
		Vector2 mAtlasSize;

		class SpriteRenderer* mUIRenderer;
		std::shared_ptr<Material> mMaterial;
	};
}