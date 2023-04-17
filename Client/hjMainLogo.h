#pragma once
#include <hjUIBase.h>

namespace hj
{
	class MainLogo :
		public UIBase
	{
	public:
		MainLogo();
		virtual ~MainLogo();

		void Initialize() final;
		void Update()  final;
		void FixedUpdate()  final;
		void Render()  final;

	private:
	};
}
