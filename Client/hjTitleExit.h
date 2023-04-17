#pragma once
#include "hjUIButton.h"

namespace hj
{
	class TitleExit :
		public UIButton
	{
	public:
		TitleExit();
		virtual ~TitleExit();

		void Initialize() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() override;

	private:
	};
}