#pragma once
#include "hjActor.h"

namespace hj
{
	class Boss1SwordCreateDestroyFx :
		public Actor
	{
	public:
		Boss1SwordCreateDestroyFx();
		Boss1SwordCreateDestroyFx(const Boss1SwordCreateDestroyFx& fx);
		virtual ~Boss1SwordCreateDestroyFx();

		virtual GameObject* Clone();

	public:
		void FxEnd();
		void PlayFx();

	private:
		void CreateAnimation();
	};
}