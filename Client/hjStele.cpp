#include "hjStele.h"
#include "hjAnimator.h"
#include "hjRoomNotPass.h"
#include "hjCollider2D.h"
#include "hjRoomDoor.h"
#include "hjObject.h"
#include "hjRoomNotPass.h"
#include "hjAudioClip.h"
#include "hjResources.h"
#include "hjAudioSource.h"

namespace hj
{
	Stele::Stele()
		: mSteleState(eSteleState::None)
		, mType(eDirType::End)
		, mDoor(nullptr)
	{
		CreateAnimation();

		mRoomNotPass = object::Instantiate<RoomNotPass>(eLayerType::ForeGround);
		mRoomNotPass->SetNotActiveByRoom();

		mSoundObj = object::Instantiate<GameObject>(eLayerType::UI);
		std::shared_ptr<AudioClip> clip = Resources::Load<AudioClip>(WIDE("Door"), WIDE("stoneDoor.ogg"));
		AudioSource* audioSrc = mSoundObj->AddComponent<AudioSource>();
		clip->SetLoop(false);
		audioSrc->SetClip(clip);
	}
	Stele::~Stele()
	{
		mDoor = nullptr;
		if (mSoundObj)
		{
			mSoundObj->Death();
		}
	}

	void Stele::ChangeSteleState(eSteleState state)
	{
		if (mSteleState == state)
		{
			return;
		}

		if (!GetComponent<Animator>())
		{
			return;
		}

		mSteleState = state;

		Animator* animator = GetComponent<Animator>();

		switch (mSteleState)
		{
		case hj::eSteleState::Closed:
			animator->Play(WIDE("Closed"), false);
			mSoundObj->GetComponent<AudioSource>()->Play();
			break;
		case hj::eSteleState::Idle:
			animator->Play(WIDE("Idle"), true);
			break;
		case hj::eSteleState::Opened:
			animator->Play(WIDE("Opened"), false); 
			mSoundObj->GetComponent<AudioSource>()->Play();
			mRoomNotPass->Pause();
			break;
		case hj::eSteleState::None:
			break;
		default:
			break;
		}
	}

	void Stele::Closed()
	{
		ChangeSteleState(eSteleState::Idle);
		mRoomNotPass->Activate();
	}

	void Stele::Opened()
	{
		if (mDoor)
		{
			mDoor->DeRegistStele();
		}

		if (mRoomNotPass)
		{
			mRoomNotPass->Death();
		}
		Death();
	}

	void Stele::ChangeDirType(eDirType type)
	{
		if (mType == type)
		{
			return;
		}

		mType = type;

		Vector2 scaleXY = GetScaleXY();
		mRoomNotPass->SetPosition(GetPosition());
		switch (type)
		{
		case hj::eDirType::Vertical:
			SetRotationZ(XM_PIDIV2);
			mRoomNotPass->SetScaleXY(Vector2(scaleXY.y, scaleXY.x));
			break;
		case hj::eDirType::Horizon:
		case hj::eDirType::End:
		default:
			SetRotationZ(0.f);
			mRoomNotPass->SetScaleXY(scaleXY);
			break;
		}
	}

	void Stele::CreateAnimation()
	{
		SetNameAndCreateSpriteRenderer(GetName(),
			WIDE("MTRL_Stage1"), WIDE("Mesh_Rect"));

		CheckHasMaterialAndTexture(eTextureSlot::T0);

		Animator* animator = AddComponent<Animator>();

		std::wstring path = WIDE("02_Object/02_Monster/00_Normal/Stage01.xml");
		std::wstring searchWstr = WIDE("Closed/Stele");
		std::wstring animWstr = WIDE("Closed");
		float frameDuration = 1.f / 10.f;
		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD_OFFSET, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration);


		searchWstr = WIDE("Idle/Stele");
		animWstr = WIDE("Idle");
		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD_OFFSET, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration);

		searchWstr = WIDE("Opened/Stele");
		animWstr = WIDE("Opened");
		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD_OFFSET, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration);

		Actor::InsertAnimationToAnimator(eTextureSlot::T0);

		animator->GetCompleteEvent(WIDE("Opened")) = std::bind(&Stele::Opened, this);
		animator->GetCompleteEvent(WIDE("Closed")) = std::bind(&Stele::Closed, this);
	}
}