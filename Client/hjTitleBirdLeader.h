#pragma once
#include "hjTitleBird.h"
#include "hjLinearMoveScript.h"

namespace hj
{
    class TitleBirdLeader :
        public TitleBird
    {
    public:
        TitleBirdLeader();
        virtual ~TitleBirdLeader();

        void Initialize() override;
        void Update() override;
        void FixedUpdate() override;
        void Render() override;

        void SetLoopInterval(float loopIntervalTime);
        bool IsReservedMoveStart() { return mbIsStart; }
        void SetReservedDelayMoveStart(float reservedDelayStartTime);
        bool GetMoveStart();

        LinearMoveScript* GetMoveScript()
        {
            if (mMoveScript)
                return mMoveScript;

            return nullptr;
        }

        void AddEndPosX(float add)
        {
            if (mMoveScript)
                mMoveScript->AddEndPosX(add);
        }

    private:
        float CalcPosYRandom();

    private:
        float mCurTime;

        LinearMoveScript* mMoveScript;
        bool mbIsStart;
        float mReservedDelayStartTime;
        float mLoopInterval;
    };
}
