#pragma once
#include <hjGameObject.h>

namespace hj
{
	class RoomNotPass :
		public GameObject
	{
	public:
		RoomNotPass();
		virtual ~RoomNotPass();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
		class NotPassScript* mScript;
	};

	class NotPassScript :
		public Script
	{
	public:
		NotPassScript();
		virtual ~NotPassScript();

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