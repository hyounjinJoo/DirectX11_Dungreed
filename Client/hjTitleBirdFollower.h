#pragma once
#include "hjTitleBird.h"

namespace hj
{
    class TitleBirdFollower :
        public TitleBird
    {
    public:
        TitleBirdFollower();
        virtual ~TitleBirdFollower();

        void Initialize() override;
        void Update() override;
        void FixedUpdate() override;
        void Render() override;
        void SetLeader(class TitleBirdLeader* leader);
        void SetDistanceFromLeader(const Vector2& dist) { mDistanceFromLeader = dist; }

    private:
        class TitleBirdLeader* mLeader;
        Vector2 mDistanceFromLeader;
    };
}
