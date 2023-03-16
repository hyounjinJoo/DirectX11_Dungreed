#pragma once
#include <hjScript.h>
#include "hjTransform.h"
namespace hj
{
    class LinearMoveScript :
        public Script
	{
	public:
		LinearMoveScript();
		virtual ~LinearMoveScript();

		virtual void Initialize();
		virtual void Update();
		virtual void FixedUpdate();
		virtual void Render();


		void SetOwnerTransform(Transform* tr) { mOwnerTr = tr; }

		void SetStartPos(const Vector2& pos) { mStartPos = pos; }
		void SetStartPosX(const float posX) { mStartPos.x = posX; }
		void SetStartPosY(const float posY) { mStartPos.y = posY; }

		void SetEndPos(const Vector2& pos) { mEndPos = pos; }
		void SetEndPosX(const float posX) { mEndPos.x = posX; }
		void SetEndPosY(const float posY) { mEndPos.y = posY; }

		void AddStartPos(const Vector2& size) { mStartPos += size; }
		void AddStartPosX(const float sizeX) { mStartPos.x += sizeX; }
		void AddStartPosY(const float sizeY) { mStartPos.y += sizeY; }

		void AddEndPos(const Vector2& size) { mEndPos += size; }
		void AddEndPosX(const float sizeX) { mEndPos.x += sizeX; }
		void AddEndPosY(const float sizeY) { mEndPos.y += sizeY; }

		void SetStartEndPos(const Vector2& pos1, const Vector2& pos2) { mStartPos = pos1; mEndPos = pos2; }
		void SetSpeed(const float speed) { mSpeed = static_cast<float>(speed * 0.001); }

		void SetStart(bool start) { mbStartMove = start; }
		void SetLoop(bool loop) { mbLoop = loop; }

		void SetLoopIntervalTime(float time) { mLoopIntervalTime = time; }

		void SetMoveDir(eMoveDir dir);
		void SetMoveDir(eMoveDir dir, const Vector2& size);
		void SetStartEndX(float posX) { mStartPos.x = posX; mEndPos.x = posX; }
		void SetStartEndY(float posY) { mStartPos.y = posY; mEndPos.y = posY; }

		bool IsComplete() { return mbCompleteMove; }
		bool IsStart() { return mbStartMove; }
	private:
		void Reset();
	private:
		bool mbStartMove;
		bool mbCompleteMove;
		bool mbLoop;

		float mCurTime;
		float mLoopIntervalTime;
		float mMoveTime;

		Transform* mOwnerTr;
		Vector2 mStartPos;
		Vector2 mEndPos;
		float mSpeed;
    };
}
