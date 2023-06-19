#include "hjItem.h"

namespace hj::object::item
{
	Item::Item(eItemType type, eItemClass eClass)
		: mType(type)
		, mClass(eClass)
	{

	}

	Item::~Item()
	{
	}
}