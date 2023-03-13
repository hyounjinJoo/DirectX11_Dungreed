#pragma once
#include "hjGameObject.h"

namespace hj
{
	class TitleBird :
		public GameObject
	{
	public:
		TitleBird();
		virtual ~TitleBird();


		void Initialize() override;
		void Update() override;
		void FixedUpdate() override;
		void Render() override;

		void SetMoveDir(eMoveDir dir) { mMoveDir = static_cast<UINT>(dir); }
		void SetMoveSpeed(float moveSpeed) { mMoveSpeed = moveSpeed; }
		void SetStartX(float startX) { mStartX = startX; }
		void SetEndX(float startX) { mStartX = startX; }
		void SetPosY(float posY) { mPosY = posY; }
		void Reset();

	private:
		bool	mbMove;
		
		float	mStartX;
		float	mEndX;
		float	mPosY;
		float	mMoveSpeed;
		UINT	mMoveDir;
	};
}