#pragma once
#include "hjWeapon.h"
#include "hjActor.h"
#include "hjSwingFx.h"

namespace hj::object::item::weapon
{
    class SwordOfExplorer :
        public Weapon, public Actor
    {
    public:
        SwordOfExplorer();
        virtual ~SwordOfExplorer();

        virtual void Initialize() final;
		virtual void Update() final;
		virtual void FixedUpdate() final;
		virtual void Render() final;

        virtual void Attack() final;
        Actor* GetFxActor();

    private:
        void CreateAnimation();

        SwingFx* mAttackFx;
    };
}