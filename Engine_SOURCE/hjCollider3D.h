#pragma once
#include "hjCollider.h"

namespace hj
{
    class Collider3D : public Collider
    {
    public:
        Collider3D();
        Collider3D(const Collider3D& collider3D);
        virtual ~Collider3D();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void FixedUpdate() override;
        virtual void Render() override;

		virtual Component* Clone() const override;

	public:
		virtual void OnCollisionEnter(Collider* collider) override;
		virtual void OnCollisionStay(Collider* collider) override;
		virtual void OnCollisionExit(Collider* collider) override;

		virtual void OnTriggerEnter(Collider* collider) override;
		virtual void OnTriggerStay(Collider* collider) override;
		virtual void OnTriggerExit(Collider* collider) override;
    private:
    };
}

