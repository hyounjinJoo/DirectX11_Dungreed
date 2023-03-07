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
        bool IsTrigger() { return mbTrigger; }

        virtual void OnCollisionEnter(Collider* collider) = 0;
		virtual void OnCollisionStay(Collider* collider) = 0;
		virtual void OnCollisionExit(Collider* collider) = 0;

		virtual void OnTriggerEnter(Collider* collider) = 0;
		virtual void OnTriggerStay(Collider* collider) = 0;
		virtual void OnTriggerExit(Collider* collider) = 0;

    protected:
        bool mbTrigger;
		eColliderType mType;
    };
}