#pragma once
#include "hjGameObject.h"

namespace hj
{
	class EditorObject : public GameObject
	{
	public:
		EditorObject();
		virtual ~EditorObject();


		void Initialize() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() override;

	private:
	};
}