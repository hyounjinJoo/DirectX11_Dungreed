#pragma once
#include "hjScript.h"

namespace hj
{
	class PlayerScript : public Script
	{
	public:
		PlayerScript();
		virtual ~PlayerScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		Vector2 GetOwnerScreenPos();
	private:
		class RigidBody* mOwnerRigid;
	};
}