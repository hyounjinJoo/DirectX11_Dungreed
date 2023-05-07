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

    class Item :
        public GameObject
    {          
    public:
        Item() = delete;
        Item(eItemType type);
        virtual ~Item();

        virtual void Initialize() = 0;
        virtual void Update() = 0;
        virtual void FixedUpdate() = 0;
        virtual void Render() = 0;

        void SetItemType(eItemType type) { mType = type; }

    protected:
        eItemType mType;
		std::string mItemName;
        std::string mItemDescription;
    };
}