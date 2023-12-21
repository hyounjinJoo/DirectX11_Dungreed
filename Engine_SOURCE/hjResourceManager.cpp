#include "pch.h"
#include "hjResourceManager.h"

namespace hj
{
	std::map<std::wstring, std::shared_ptr<Resource>> ResourceManager::mResources;
}
