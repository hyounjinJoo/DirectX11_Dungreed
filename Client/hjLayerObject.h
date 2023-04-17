#pragma once
#include "hjGameObject.h"

namespace hj
{
    class LayerObject :
        public GameObject
    {
    public:
        LayerObject();
        virtual ~LayerObject();

        void Initialize() override;
        void Update() override;
        void FixedUpdate() override;
        void Render() override;

        void SetMove(bool move) { mMove = move; }
        void SetMoveSpeed(float moveSpeed) { mMoveSpeed = moveSpeed; }
        void SetMoveDir(eMoveDir dir) { mMoveDir = static_cast<UINT>(dir); }

		void SetStartUV(const Vector2& uv) { mStartUV = uv; }
		void SetEndUV(const Vector2& uv) { mEndUV = uv; }
	private:
		float   mTotalMove;
        float   mMoveSpeed;
		UINT    mMoveDir;
        bool    mMove;
        Vector2 mStartUV;
        Vector2 mEndUV;
    };
}