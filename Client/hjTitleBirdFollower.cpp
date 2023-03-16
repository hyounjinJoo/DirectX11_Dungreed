#include "hjTitleBirdFollower.h"
#include "hjTitleBirdLeader.h"

namespace hj
{
	TitleBirdFollower::TitleBirdFollower()
		: TitleBird()
		, mLeader(nullptr)
		, mDistanceFromLeader(Vector2::One)
	{
	}
	TitleBirdFollower::~TitleBirdFollower()
	{
	}
	void TitleBirdFollower::Initialize()
	{
		TitleBird::Initialize();
	}

	void TitleBirdFollower::Update()
	{
		TitleBird::Update();

		if (nullptr == mLeader)
			return;

		SetPositionXY(mLeader->GetPositionXY() + mDistanceFromLeader);
	}

	void TitleBirdFollower::FixedUpdate()
	{
		TitleBird::FixedUpdate();

		if (nullptr == mLeader)
			return;
	}

	void TitleBirdFollower::Render()
	{
		TitleBird::Render();

		if (nullptr == mLeader)
			return;
	}

	void TitleBirdFollower::SetLeader(TitleBirdLeader* leader)
	{
		if (leader)
		{
			mLeader = leader;
		}
	}

}