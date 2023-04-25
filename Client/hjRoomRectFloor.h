#pragma once
#include "hjGameObject.h"
#include "hjScript.h"

namespace hj
{
	class FloorScript :
		public Script
	{
	public:
		FloorScript();
		virtual ~FloorScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		virtual void OnCollisionEnter(Collider* collider) override;
		virtual void OnCollisionStay(Collider* collider) override;

		void SetCollider(class Collider2D* collider);
	private:
		Collider2D* mRectCollider;
	};

	class RoomRectFloor :
		public GameObject
	{
	public:
		RoomRectFloor();
		virtual ~RoomRectFloor();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
		FloorScript* mScript;
	};

}

