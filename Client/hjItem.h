#pragma once
#include "hjGameObject.h"

namespace hj::object::item
{
	enum class eItemType
	{
		None,
		Weapon,
		Accessory,
		End,
	};

    enum class eItemClass
    {
        Common,
        UnCommon,
        Rare,
        Legendary,
    };

    class Item
    {          
    public:
        Item() = delete;
        Item(eItemType type, eItemClass eClass);
        virtual ~Item();

        void SetItemType(eItemType type) { mType = type; }
        void SetItemClass(eItemClass eClass) { mClass = eClass; }

    protected:
        eItemType mType;
        eItemClass mClass;
		std::string mItemName;
        std::string mItemDescription;

        bool mbIsSpawned;
    };
}