#include "hjMonsterSpawner.h"
#include "hjMonster.h"
#include "hjAnimator.h"
#include "hjRoomBase.h"
#include "hjAudioSource.h"
#include "hjObject.h"
#include "hjResources.h"

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

		mSpawnSoundObj = object::Instantiate<Actor>(eLayerType::UI);
		mSpawnSoundObj->GetTransform()->SetParent(this->GetTransform());
		std::shared_ptr<AudioClip> clip = Resources::Load<AudioClip>(WIDE("SpawnMonster"), WIDE("SpawnMonster.mp3"));
		clip->SetLoop(false);
		mSpawnSoundObj->AddComponent<AudioSource>()->SetClip(clip);

	}

	MonsterSpawner::~MonsterSpawner()
	{
		mMonster = nullptr;
		if (mOwnerRoom)
		{
			mOwnerRoom->DelistFromManagedGameObjects(this);
		}

		if (mSpawnSoundObj)
		{
			mSpawnSoundObj->Death();
		}
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
		mMonster->Pause();
		if (mOwnerRoom)
		{
			mMonster->SetOwnerRoom(mOwnerRoom);
		}
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
			mMonster->Activate();
			mbSpawned = true;
			if (mSpawnSoundObj && mSpawnSoundObj->GetComponent<AudioSource>() && mSpawnSoundObj->GetComponent<AudioSource>()->GetClip())
			{
				AudioSource* source = mSpawnSoundObj->GetComponent<AudioSource>();
				source->Play();
			}
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