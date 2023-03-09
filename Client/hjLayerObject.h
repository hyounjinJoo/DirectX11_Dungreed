#pragma once
#include "hjGameObject.h"

namespace hj
{
    class LayerObject :
        public GameObject
    {
        enum class MoveDir
        {
            Left = 0,
            Top,
            Right,
            Bottom,
            End,
        };

    public:
        LayerObject();
        virtual ~LayerObject();

        void Initialize() override;
        void Update() override;
        void FixedUpdate() override;
        void Render() override;

        void SetMove(bool move) { mMove = move; }
        void SetMoveSpeed(float moveSpeed) { mMoveSpeed = moveSpeed; }
        void SetMoveDir(MoveDir dir) { mMoveDir = static_cast<UINT>(dir); }

	private:
		float   mTotalMove;
        float   mMoveSpeed;
		UINT    mMoveDir;
        bool    mMove;
    };
}