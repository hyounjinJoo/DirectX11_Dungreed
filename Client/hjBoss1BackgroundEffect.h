#pragma once
#include "hjActor.h"

namespace hj
{
	class Boss1BackgroundEffect :
		public Actor
	{
	public:
		Boss1BackgroundEffect();
		virtual ~Boss1BackgroundEffect();

	private:
		void CreateAnimation();
	};
}