#pragma once
#include <hjGameObject.h>

namespace hj
{
    class TestMonster :
        public GameObject
    {
    public:
        TestMonster();
        virtual ~TestMonster();

        void Initialize() override;
        void Update() override;
        void FixedUpdate() override;
        void Render() override;

    private:
    };
}