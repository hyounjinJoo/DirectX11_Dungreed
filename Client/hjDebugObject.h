#pragma once
#include "..\Engine_SOURCE\hjGameObject.h"

namespace hj
{
	class DebugObject : public GameObject
	{
	public:
		DebugObject();
		virtual ~DebugObject();


		void Initialize() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() override;

	private:
	};
}