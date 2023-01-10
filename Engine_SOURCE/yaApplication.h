#pragma once
#include "yaMath.h"

namespace ya
{
	class Application
	{
	public:
		int a;
		int b;

		void test()
		{
			math::Matrix matrix;
			matrix.CreateRotationZ(1.f);
		}
	};
}
