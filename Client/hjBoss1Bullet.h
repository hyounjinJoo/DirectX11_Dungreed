#pragma once
#include "hjActor.h"

namespace hj
{
	class Boss1Bullet :
		public Actor
	{
	public:
		Boss1Bullet();
		Boss1Bullet(const Boss1Bullet& bullet);
		virtual ~Boss1Bullet();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		virtual GameObject* Clone() const override;

	public:
		void SetMoveDirection(const Vector2& dir) { mMoveDir = dir; }

	public:
		void FxStart();
		void FxEnd();

	private:
		void CreateAnimation();

	private:
		bool mbIsBulletState;

		Vector2 mMoveDir;
		float mMoveSpeed;

		float mActiveTimer;
		float mActiveTime;

		class RoomBase* mOwnerRoom;
	};
}