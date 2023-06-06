#include "pch.h"
#include "hjComponent.h"

namespace hj
{   
    Component::Component(eComponentType type)
        : mType(type)
        , mOwner(nullptr)
        , mbActive(true)
    {
    }

	Component::~Component()
    {
    }
}