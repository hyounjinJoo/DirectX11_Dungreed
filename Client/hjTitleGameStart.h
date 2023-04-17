#pragma once
#include "hjUIButton.h"

namespace hj
{
	class TitleGameStart :
		public UIButton
	{
	public:
		TitleGameStart();
		virtual ~TitleGameStart();

		void Initialize() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() override;

		void StartGame();

	private:
	};
}