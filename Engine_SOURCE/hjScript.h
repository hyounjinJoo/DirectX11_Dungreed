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


	private:
		//std::vector<Script*> mScripts;		
	};
}

