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

        void SetSize(Vector2 size) { mSize = size;}
        void SetCenter(Vector2 center) { mCenter = center; }

		virtual void OnCollisionEnter(Collider* collider) override;
		virtual void OnCollisionStay(Collider* collider) override;
		virtual void OnCollisionExit(Collider* collider) override;

		virtual void OnTriggerEnter(Collider* collider) override;
		virtual void OnTriggerStay(Collider* collider) override;
		virtual void OnTriggerExit(Collider* collider) override;

    private:
        Transform* mTransform;

        Vector2 mSize;
        Vector2 mCenter;
    };
}
