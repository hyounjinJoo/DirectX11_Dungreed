#pragma once
#include "hjGameObject.h"

namespace hj
{
    class TitleCloud :
        public GameObject
    {
    public:
        TitleCloud();
        virtual ~TitleCloud();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void FixedUpdate() override;
        virtual void Render() override;

    private:
    };
}