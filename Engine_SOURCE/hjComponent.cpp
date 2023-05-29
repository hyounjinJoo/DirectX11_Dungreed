#include "hjComponent.h"

namespace hj
{   
    Component::Component(eComponentType type)
        : mType(type)
        , mOwner(nullptr)
    {
    }

	Component::~Component()
    {
    }
}