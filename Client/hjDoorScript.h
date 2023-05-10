#pragma once
#include <hjScript.h>

namespace hj
{
	class DoorScript :
		public Script
	{
	public:
		virtual void OnCollisionEnter(Collider* collider) override;
	};
}