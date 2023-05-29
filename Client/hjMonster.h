#pragma once
#include <hjGameObject.h>

namespace hj
{
	class Monster :
		public GameObject
	{
	public:
		Monster();
		~Monster();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		virtual void Damaged(float damage) = 0;

	private:
		virtual void ProcessDamaged(float damage) = 0;
	};
}