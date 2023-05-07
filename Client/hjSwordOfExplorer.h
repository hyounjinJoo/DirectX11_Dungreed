#pragma once
#include "hjWeapon.h"

namespace hj::object::item::weapon
{
    class SwordOfExplorer :
        public Weapon
    {
    public:
        SwordOfExplorer();
        virtual ~SwordOfExplorer();

        virtual void Initialize() final;
		virtual void Update() final;
		virtual void FixedUpdate() final;
		virtual void Render() final;

    private:
    };
}