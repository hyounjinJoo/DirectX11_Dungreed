#pragma once
#include "hjScript.h"

namespace hj
{
	class CameraScript : public Script
	{
	public:
		CameraScript();
		virtual ~CameraScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render() override;

		void UseMoveLimit(bool use) { mbMoveLimit = use; }
		void SetLimitSpace(const Vector4& space) { mLimitLTRB = space; }
		void SetLimitSpace(float spaceL, float spaceT, float spaceR, float spaceB) { mLimitLTRB = Vector4(spaceL, spaceT, spaceR, spaceB); }
		void FollowPlayer(bool follow);
		void SetMoveLinearPos(const Vector2& pos, float time);
		void ChangeCameraLinearScale(float scale, float time);
		void ResetCameraScale();

		void Shake();
	private:
		class GameObject* GetPlayer();
		void UpdateCameraPos();
		void UpdateShakedCameraPos();
		Vector2 CalcRandomShakeValue();

		bool mbFollowPlayer;

		bool mbMoveLimit;
		Vector4 mLimitLTRB;
		Vector2 mRectHalfScale;

		float mShakeDuration;
		float mShakeTimer;
		float mDicreaseShakeFactor;
		Vector2 mShakeMoveLimit;
		Vector2 mOriginPos;

		bool mbLinearMove;
		Vector2 mLinearMoveTargetPos;
		float mLinearMoveTime;
		float mLinearMoveTimer;
		Vector2 mTargetPos;

		bool mbLinearScaleChange;
		float mTargetScale;
		float mLinearScaleChangeTime;
		float mLinearScaleChangeTimer;
	};
}