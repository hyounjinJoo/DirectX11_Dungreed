#pragma once
#include "hjResource.h"

namespace hj
{
	class Prefab : public Resource
	{
	public:
		Prefab();
		virtual ~Prefab();
				
		HRESULT Load(const std::wstring& path) override;

	private:
	};
}
