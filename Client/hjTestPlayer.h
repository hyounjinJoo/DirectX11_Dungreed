#pragma once
#include <hjGameObject.h>

namespace hj
{
    class TestPlayer :
        public GameObject
    {
    public:
        TestPlayer();
        virtual ~TestPlayer();

        void Initialize() override;
        void Update() override;
        void FixedUpdate() override;
        void Render() override;

    private:
    };
}
