#pragma once
#include "hjUIBase.h"

namespace hj
{
	enum class progressBarType
	{
		RightToLeft,
		DownToUp,
		End,
	};

	class UIProgressBar :
	    public UIBase
	{
	public:
		UIProgressBar();
		virtual ~UIProgressBar();
	
		void Initialize() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() override;
	
		void SetScale(const Vector3& scale);
		void SetScaleXY(const Vector2& scaleXY);
		void SetScaleX(const float scaleX);
		void SetScaleY(const float scaleY);

		void SetCurrentPercentValue(float value)
		{
			mCurrentPercentValue = value;
			mbIsNeedToUpdate = true;
		}
		void SetMaxPercentValue(float value) 
		{
			mMaxPercentValue = value;
			mbIsNeedToUpdate = true;
		}

		float GetCurrentPercentValue() { return mCurrentPercentValue; }
		float GetMaxPercentValue() { return mMaxPercentValue; }

	private:
		void CreateAnimation();

	private:
		progressBarType mType;

		Vector2 mMaxPercentUV;
		Vector2 mCurrentPercentUV;
		float mCurrentPercentValue;
		float mMaxPercentValue;

		bool mbIsNeedToUpdate;
	};
}
