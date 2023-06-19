#pragma once
#include "hjActor.h"

namespace hj
{
	class PlayerLifeBarWaveUI :
		public Actor
	{
	public:
		PlayerLifeBarWaveUI();
		virtual ~PlayerLifeBarWaveUI();

	private:
		void CreateAnimation();
	};
}
