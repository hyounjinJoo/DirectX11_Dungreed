#include "hjLinearMoveScript.h"
#include "hjTime.h"
#include "hjApplication.h"
#include "hjGameObject.h"

extern hj::Application application;

namespace hj
{

	LinearMoveScript::LinearMoveScript()
		: Script()
		, mbLoop(false)
		, mbStartMove(false)
		, mbCompleteMove(false)
		, mStartPos(Vector2::Zero)
		, mEndPos(Vector2::Zero)
		, mSpeed(0.f)
		, mCurTime(0.f)
		, mMoveTime(0.f)
		, mLoopIntervalTime(1.f)
		, mOwnerTr(nullptr)
	{
	}

	LinearMoveScript::~LinearMoveScript()
	{
	}
	
	void LinearMoveScript::Initialize()
	{
		Script::Initialize();
		mOwnerTr = GetOwner()->GetTransform();
	}
	
	void LinearMoveScript::Update()
	{
		Script::Update();

		if (!mbStartMove)
		{
			return;
		}

		if (mbCompleteMove)
		{
			if (mbLoop)
			{
				float deltaTime = Time::DeltaTime();

				mCurTime += deltaTime;
				if (mCurTime >= mLoopIntervalTime)
				{
					Reset();
				}

				return;
			}
			else
			{
				return;
			}
		}
		
		if (mEndPos != mOwnerTr->GetPositionXY())
		{
			float deltaTime = Time::DeltaTime();

			mMoveTime += deltaTime;
			float step = mMoveTime * mSpeed;
			Vector2 calcedPos = Vector2::SmoothStep(mStartPos, mEndPos, step);
		
			mOwnerTr->SetPositionXY(calcedPos);
			
			if (mEndPos == mOwnerTr->GetPositionXY())
			{
				mbCompleteMove = true;
			}
		}
	}
	
	void LinearMoveScript::FixedUpdate()
	{
		Script::FixedUpdate();
	}
	
	void LinearMoveScript::Render()
	{
		Script::Render();
	}

	void LinearMoveScript::SetMoveDir(eMoveDir dir)
	{
		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);

		float halfWidth = static_cast<float>(winRect.right - winRect.left) * 0.5f;
		float halfHeight = static_cast<float>(winRect.bottom - winRect.top) * 0.5f;

		switch (dir)
		{
		case enums::eMoveDir::Left:
			mStartPos= Vector2(halfWidth, 0.f);
			mEndPos= Vector2(-halfWidth, 0.f);
			break;

		case enums::eMoveDir::Top:
			mStartPos = Vector2(0.f, -halfHeight);
			mEndPos = Vector2(0.f, halfHeight);
			break;

		case enums::eMoveDir::Right:
			mStartPos = Vector2(-halfWidth, 0.f);
			mEndPos = Vector2(halfWidth, 0.f);
			break;

		case enums::eMoveDir::Bottom:
			mStartPos = Vector2(0.f, halfHeight);
			mEndPos = Vector2(0.f, -halfHeight);
			break;

		case enums::eMoveDir::End:
		default:
			break;
		}
	}

	void LinearMoveScript::SetMoveDir(eMoveDir dir, const Vector2& size)
	{
		SetMoveDir(dir);

		switch (dir)
		{
		case enums::eMoveDir::Left:
			mStartPos += Vector2(size.x, 0.f);
			mEndPos += Vector2(-size.x, 0.f);

		case enums::eMoveDir::Right:
			mStartPos += Vector2(-size.x, 0.f);
			mEndPos += Vector2(size.x, 0.f);
			break;

		case enums::eMoveDir::Top:
			mStartPos += Vector2(0.f, -size.y);
			mEndPos += Vector2(0.f, size.y);
		case enums::eMoveDir::Bottom:
			mStartPos += Vector2(0.f, size.y);
			mEndPos += Vector2(0.f, -size.y);
			break;

		case enums::eMoveDir::End:
		default:
			break;
		}
	}

	void LinearMoveScript::Reset()
	{
		mCurTime = 0.f;
		mbCompleteMove = false;
		mOwnerTr->SetPositionXY(mStartPos);
		mMoveTime = 0.f;
	}

}