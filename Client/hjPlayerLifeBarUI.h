#pragma once
#include "hjActor.h"

namespace hj
{
	class PlayerLifeBarUI :
		public Actor
	{
	public:
		PlayerLifeBarUI();
		virtual ~PlayerLifeBarUI();

	public:
		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();

	public:
		void SetMaxHPValue(float value);
		void SetCurrentHPValue(float value);

	private:
		void CreateHPBarAnimation();
		void CreateHPBarBack();
		void CreateHPBarGauge();
		void CreateHPBarWave();
		void InitializeUIPose();

	private:
		class UIProgressBar* mLifeBarGauge;
		Actor* mLifeBarWave;
	};
}
