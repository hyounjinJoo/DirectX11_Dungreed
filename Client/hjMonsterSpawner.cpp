#include "hjMonsterSpawner.h"
#include "hjMonster.h"
#include "hjAnimator.h"

namespace hj
{
	MonsterSpawner::MonsterSpawner()
		: mMonster(nullptr)
		, mbSpawned(false)
	{
		SetNameAndCreateSpriteRenderer(WIDE("MonsterSpawner"),
			WIDE("MTRL_FX"), WIDE("Mesh_Rect"));

		AddComponent<Animator>();

		CreateAnimation();
	}

	MonsterSpawner::~MonsterSpawner()
	{
		mMonster = nullptr;
	}

	void MonsterSpawner::Initialize()
	{
		Animator* animator = GetComponent<Animator>();
		animator->Play(WIDE("MagicCircleFx"), false);

		Actor::Initialize();
	}

	void MonsterSpawner::SetSpawnMonster(Monster* monster)
	{
		mMonster = monster;
		mMonster->SetPositionXY(GetPositionXY());
		mMonster->SetNotActiveByRoom();
	}

	void MonsterSpawner::SpawnMonster()
	{
		if (mbSpawned)
		{
			return;
		}

		if (mMonster)
		{
			mMonster->Initialize();
			mbSpawned = true;
		}

		Death();
	}

	void MonsterSpawner::CreateAnimation()
	{
		CheckHasMaterialAndTexture(eTextureSlot::T0);

		std::wstring path = WIDE("08_FX/08_FX.xml");
		std::wstring searchWstr = WIDE("MagicCircleFx/MagicCircleFx");
		std::wstring animWstr = WIDE("MagicCircleFx");
		float frameDuration = 1.f / 15.f;

		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD_OFFSET_TRIM_OFFSET, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration, false, 0, true);
		CalcOffsetAutoY(animWstr);
		Actor::InsertAnimationToAnimator(eTextureSlot::T0);

		Animator* animator = GetComponent<Animator>();
		animator->GetCompleteEvent(animWstr) = std::bind(&MonsterSpawner::SpawnMonster, this);
	}
}