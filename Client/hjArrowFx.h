#pragma once
#include "hjActor.h"

namespace hj
{
	class ArrowFx :
		public Actor
	{
	public:
		ArrowFx();
		virtual ~ArrowFx();

		void PlayFx();
		void StopFx();

	private:
		void CreateAnimation();
	};
}