#pragma once
#include "hjActor.h"

namespace hj
{
	class DashCountBase;

	class PlayerDashUI :
		public Actor
	{
	public:
		PlayerDashUI();
		virtual ~PlayerDashUI();

	public:
		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();

	public:
		UINT GetMaxDashCount() { return mDashBaseMaxCount; }
		void SetMaxDashCount(UINT count);
		UINT GetCurDashCount() { return mCurrentDashCanCount; }

		void DicreaseCurCount();
		void IncreaseCurCount();

	private:
		void ActivateDashCount();
		void DeactivateDashCount();

	private:
		void CreateRightEndUIAnimation();

	private:
		UINT mDashBaseMaxCount;
		UINT mCurrentDashCanCount;

	private:
		std::vector<DashCountBase*> mDashUIBases;
		Actor* mDashBaseUIRightEnd;
	};
}