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

    private:
    };
}

