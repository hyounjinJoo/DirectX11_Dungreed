#pragma once
#include <hjGameObject.h>

namespace hj
{
	class RoomPassThrough :
		public GameObject
	{
	public:
		RoomPassThrough();
		virtual ~RoomPassThrough();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
		class PassThroughScript* mScript;
	};

	class PassThroughScript :
		public Script
	{
	public:
		PassThroughScript();
		virtual ~PassThroughScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		virtual void OnCollisionEnter(Collider* collider) override;
		virtual void OnCollisionStay(Collider* collider) override;
		virtual void OnCollisionExit(Collider* collider) override;

		void SetCollider(class Collider2D* collider);
	private:
		Collider2D* mRectCollider;
	};
}
