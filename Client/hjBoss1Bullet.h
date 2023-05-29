#pragma once
#include <hjGameObject.h>

namespace hj
{
	class Boss1Bullet :
		public GameObject
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

	private:
		void CreateAnimation();

	private:
		Vector2 mMoveDir;
		float mMoveSpeed;

		float mActiveTimer;
		float mActiveTime;
	};
}