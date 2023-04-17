#pragma once
#include "hjComponent.h"
#include "hjCollider.h"

namespace hj
{
	class Script :public Component
	{
	public:
		Script();
		virtual ~Script();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

		virtual void OnCollisionEnter(Collider* collider) {};
		virtual void OnCollisionStay(Collider* collider) {};
		virtual void OnCollisionExit(Collider* collider) {};

		virtual void OnTriggerEnter(Collider* collider) {};
		virtual void OnTriggerStay(Collider* collider) {};
		virtual void OnTriggerExit(Collider* collider) {};

		virtual void OnCollisionMouseEnter(const Vector2& mousePos) {};
		virtual void OnCollisionMouseStay(const Vector2& mousePos) {};
		virtual void OnCollisionMouseExit(const Vector2& mousePos) {};

		virtual void OnTriggerMouseEnter(const Vector2& mousePos) {};
		virtual void OnTriggerMouseStay(const Vector2& mousePos) {};
		virtual void OnTriggerMouseExit(const Vector2& mousePos) {};

	private:
		//std::vector<Script*> mScripts;		
	};
}

