#include "hjEntity.h"

namespace hj
{
	Entity::Entity()
		: mID(PtrToUint(this))
	{
	}
	Entity::Entity(const Entity& other)
		: mID(PtrToUint(this))
		, mName(other.mName)
	{
	}
	Entity::~Entity()
	{
	}
}