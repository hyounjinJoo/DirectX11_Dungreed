#pragma once
#include "hjComponent.h"

namespace hj
{
    using namespace math;
    class Collider : public Component
    {
    public:
        Collider();
        virtual ~Collider();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void FixedUpdate() override;
        virtual void Render() override;

		void SetType(eColliderType type) { mType = type; };
        eColliderType GetType() { return mType; }
        bool IsTrigger() { return mbTrigger; }
        void SetState(eCollisionState state) { mState = state; }
		eCollisionState GetState() { return mState; }
        void UseMouseCollision(bool use){ mbUseMouseCollision = use; }
        bool IsUsingMouseCollision() { return mbUseMouseCollision; }
		bool IsMouseTrigger() { return mbMouseTrigger; }
        void SetMouseState(eCollisionState mouseState) 
        { 
            mMouseState = mouseState;

            if(mbUseMouseCollision)
                mState = mouseState; 
        }
		eCollisionState GetMouseState() { return mMouseState; }

        virtual void OnCollisionEnter(Collider* collider) = 0;
		virtual void OnCollisionStay(Collider* collider) = 0;
		virtual void OnCollisionExit(Collider* collider) = 0;

		virtual void OnTriggerEnter(Collider* collider) = 0;
		virtual void OnTriggerStay(Collider* collider) = 0;
		virtual void OnTriggerExit(Collider* collider) = 0;

        virtual void OnCollisionMouseEnter(const Vector2& mousePos) = 0;
        virtual void OnCollisionMouseStay(const Vector2& mousePos) = 0;
        virtual void OnCollisionMouseExit(const Vector2& mousePos) = 0;

		virtual void OnTriggerMouseEnter(const Vector2& mousePos) = 0;
        virtual void OnTriggerMouseStay(const Vector2& mousePos) = 0;
        virtual void OnTriggerMouseExit(const Vector2& mousePos) = 0;

        static UINT32 colliderID;

        UINT32 GetColliderID() { return mColliderID; }

    protected:
        bool mbTrigger;
		bool mbMouseTrigger;
        bool mbUseMouseCollision;
		eColliderType mType;
        const UINT32 mColliderID;
		eCollisionState mState;
		eCollisionState mMouseState;
    };
}