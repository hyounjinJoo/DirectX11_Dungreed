#pragma once
#include "hjActor.h"

namespace hj
{
	class DashCountBase :
		public Actor
	{
	public:
		DashCountBase();
		virtual ~DashCountBase();

	public:
		bool IsActivateDashCount() { return mbIsActivateDashCount; }
		void ActivateDashCount();
		void DeactivateDashCount();

	private:
		void CreateBaseUIAnimation();
		void CreateDashCountUIAnimation();

	private:
		bool mbIsActivateDashCount;
		Actor* mDashCount;
	};
}