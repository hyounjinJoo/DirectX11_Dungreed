#include "hjPrefab.h"

namespace hj
{
	Prefab::Prefab()
		: Resource(eResourceType::Prefab)
	{
	}
	Prefab::~Prefab()
	{
	}
	HRESULT Prefab::Load(const std::wstring& path)
	{
		return E_NOTIMPL;
	}
}