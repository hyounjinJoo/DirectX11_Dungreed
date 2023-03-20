#include "hjItem.h"

namespace hj::object::item
{
	Item::Item(eItemType type)
		: GameObject()
		, mType(type)
	{

	}

	Item::~Item()
	{
	}
}