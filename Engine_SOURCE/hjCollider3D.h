#pragma once
#include "hjCollider.h"

namespace hj
{
    class Collider3D : public Collider
    {
    public:
        Collider3D();
        virtual ~Collider3D();

        void Initialize() override;
        void Update() override;
        void FixedUpdate() override;
        void Render() override;

		virtual void OnCollisionEnter(Collider* collider) override;
		virtual void OnCollisionStay(Collider* collider) override;
		virtual void OnCollisionExit(Collider* collider) override;

		virtual void OnTriggerEnter(Collider* collider) override;
		virtual void OnTriggerStay(Collider* collider) override;
		virtual void OnTriggerExit(Collider* collider) override;
    private:
    };
}

