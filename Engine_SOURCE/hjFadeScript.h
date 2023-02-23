#pragma once
#include "hjScript.h"
#include "hjCamera.h"

namespace hj
{
	class FadeScript : public Script
	{
		enum class eFadeState
		{
			FadeInStart = 0,
			FadeInEnd,
			FadeOutStart,
			FadeOutEnd,
			End,
		};

	public:
		FadeScript();
		virtual ~FadeScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void SetTotalTransitionTime(float time) { mTotalTransitionTime = time; }
		void SetFadeColor(eFadeState state, const Vector4& color);

	private:
		eFadeState mFadeState;

		float mTotalTransitionTime;
		float mCurrentTime;

		Vector4 mFadeInStartColor;
		Vector4 mFadeInEndColor;
		Vector4 mFadeOutStartColor;
		Vector4 mFadeOutEndColor;
		
		Vector4 mCurrentColor;
	};
}