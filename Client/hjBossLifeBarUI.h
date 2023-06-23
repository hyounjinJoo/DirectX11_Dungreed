#pragma once
#include "hjActor.h"

namespace hj
{
	class BossLifeBarUI :
		public Actor
	{
	public:
		BossLifeBarUI();
		virtual ~BossLifeBarUI();

	public:
		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	public:
		virtual void Death();

	public:
		void SetMaxHPValue(float value);
		void SetCurrentHPValue(float value);

	private:
		void CreateHPBarAnimation();
		void CreateHPBarBack();
		void CreateHPBarGauge();
		void CreateBossLifeBarImage();
		void InitializeUIPose();

	private:
		class UIProgressBar* mLifeBarGauge;
		Actor* mBossLifeBarImage;
		Actor* mBackground;
	};
}