#pragma once
#include "hjComponent.h"

namespace hj
{
    class Collider : public Component
    {
    public:
        Collider();
        virtual ~Collider();

        void Initialize() override;
        void Update() override;
        void FixedUpdate() override;
        void Render() override;

		void SetType(eColliderType type) { mType = type; };
        eColliderType GetType() { return mType; }

    protected:
        bool mbTrigger;
		eColliderType mType;
    };
}