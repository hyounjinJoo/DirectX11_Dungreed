#pragma once
#include "hjCollider.h"
#include "hjTransform.h"

namespace hj
{
    class Collider2D : public Collider
    {
    public:
        Collider2D();
        virtual ~Collider2D();

        void Initialize() override;
        void Update() override;
        void FixedUpdate() override;
        void Render() override;

        void SetSize(Vector2 size);
        void SetCenter(Vector2 center) { mCenter = center; }

		virtual void OnCollisionEnter(Collider* collider) override;
		virtual void OnCollisionStay(Collider* collider) override;
		virtual void OnCollisionExit(Collider* collider) override;

		virtual void OnTriggerEnter(Collider* collider) override;
		virtual void OnTriggerStay(Collider* collider) override;
		virtual void OnTriggerExit(Collider* collider) override;

		virtual void OnCollisionMouseEnter(const Vector2& mousePos) override;
		virtual void OnCollisionMouseStay(const Vector2& mousePos) override;
		virtual void OnCollisionMouseExit(const Vector2& mousePos) override;

		virtual void OnTriggerMouseEnter(const Vector2& mousePos) override;
		virtual void OnTriggerMouseStay(const Vector2& mousePos) override;
		virtual void OnTriggerMouseExit(const Vector2& mousePos) override;

        const Vector3& GetPosition() { return mPosition; }
        const Vector2& GetSize()
        {
            if (eColliderType::Circle == mType)
            {
                static Vector2 circleSize;
                circleSize = Vector2(mRadius, mRadius);
                return circleSize;
            }

            return mSize; 
        }

    private:
        Transform* mTransform;

        Vector3 mPosition;
        Vector2 mSize;
        Vector2 mCenter;
        float   mRadius;
    };
}
