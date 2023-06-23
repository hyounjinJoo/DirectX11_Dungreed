#include "hjMonster.h"
#include "hjSceneManager.h"
#include "hjPlayer.h"

namespace hj
{
	Monster::Monster()
		: mDamagedEffectTimer(0.1f)
		, mbIsDeadMonster(false)
		, mPlayer(nullptr)
		, mMoveSpeed(200.f)
	{
	}

	Monster::~Monster()
	{
	}

	void Monster::Initialize()
	{
		GameObject::Initialize();
	}

	void Monster::Update()
	{
		GameObject::Update();
	}

	void Monster::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}

	void Monster::Render()
	{
		GameObject::Render();
	}

	void Monster::SubtractCurrentHP(int value)
	{
		mMonsterStatus.currentHP -= value;

		if (0 > mMonsterStatus.currentHP)
		{
			mbIsDeadMonster = true;
		}
	}

	void Monster::FindPlayer()
	{
		std::vector<GameObject*> objects = SceneManager::GetActiveScene()->GetGameObjects(eLayerType::Player);
		for (auto iter : objects)
		{
			if (dynamic_cast<Player*>(iter))
			{
				mPlayer = dynamic_cast<Player*>(iter);
				return;
			}
		}
	}

	void Monster::CalcChaseDir()
	{
		if (!mPlayer)
		{
			FindPlayer();
			if (!mPlayer)
			{
				return;
			}
		}

		Vector2 playerPos = mPlayer->GetWorldPositionXY();
		Vector2 myPos = GetWorldPositionXY();

		Vector2 chaseDir = playerPos - myPos;

		mChaseDir = chaseDir;
	}

	float Monster::CalcDistanceFromPlayer()
	{
		if (!mPlayer)
		{
			FindPlayer();
			if (!mPlayer)
			{
				return FLT_MAX;
			}
		}

		Vector2 playerPos = mPlayer->GetPositionXY();
		Vector2 monsterPos = GetPositionXY();

		float result = 0.f;
		result = Vector2::Distance(playerPos, monsterPos);

		return result;
	}

}