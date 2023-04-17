#pragma once
#include "hjGameObject.h"

namespace hj
{
    class TitleMidCloud :
        public GameObject
    {
    public:
        TitleMidCloud();
        virtual ~TitleMidCloud();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void FixedUpdate() override;
        virtual void Render() override;

    private:
    };
}