#pragma once
#include "hjUIButton.h"

namespace hj
{
	class TitleOption :
		public UIButton
	{
	public:
		TitleOption();
		virtual ~TitleOption();

		void Initialize() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() override;

	private:
	};
}