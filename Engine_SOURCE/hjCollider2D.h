#pragma once
#include "hjCollider.h"

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

    private:
    };
}
