#include "hjResources.h"

namespace hj
{
	std::map<std::wstring, Resource*> Resources::mResources;

	void Resources::Release()
	{
		std::map<std::wstring, Resource*>::iterator iter = mResources.begin();
		for (; iter != mResources.end(); ++iter)
		{
			delete iter->second;
			iter->second = nullptr;
		}
	}
}
