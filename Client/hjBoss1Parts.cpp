#include "hjBoss1Parts.h"
#include "hjTime.h"
#include "hjRigidBody.h"
#include "hjCollider2D.h"
#include "hjAnimation.h"
#include "hjAnimator.h"


namespace hj
{
	Boss1Parts::Boss1Parts()
		: Actor()
		, mMoveStartTimer(0.f)
		, mMoveStartThreshold(0.5f)
		, mBodyType(bodyType::End)
	{
		AddComponent<RigidBody>();
		AddComponent<Collider2D>();

		SetNameAndCreateSpriteRenderer(WIDE("Boss1_Body"),
			WIDE("MTRL_Monster_Boss_Bellial"), WIDE("Mesh_Rect"));
	}

	Boss1Parts::~Boss1Parts()
	{
	}

	void Boss1Parts::Update()
	{
		mMoveStartTimer += Time::ActualDeltaTime();
		if (mMoveStartTimer < mMoveStartThreshold)
		{
			return;
		}

		GameObject::Update();
	}
	
	void Boss1Parts::SetBodyType(bodyType type)
	{
		mBodyType = type;

		Animator* animator = AddComponent<Animator>();
		if (animator)
		{
			CreateAnimation();
		}
		
		//Vector2 spriteSize = animator->FindAnimation(animName)->GetSpriteSize(0);
		////SetScaleXY(spriteSize);
		//Vector2 scaleRatio = spriteSize / animator->FindAnimation(animName)->GetCanvasSize();
		//GetComponent<Collider2D>()->SetSize(scaleRatio);
	}

	void Boss1Parts::CreateAnimation()
	{
		CheckHasMaterialAndTexture(eTextureSlot::T0);

		std::wstring path = WIDE("02_Object/02_Monster/01_Boss/Monster_Boss_1_Bellial.xml");
		std::wstring searchWstr;
		std::wstring animWstr;

		switch (mBodyType)
		{
		case hj::bodyType::HEAD:
			searchWstr = WIDE("SkellBossDead_Head");
			animWstr = WIDE("SkellBossBodyParts_Head");
			break;
		case hj::bodyType::LEFT_UPPER_ZAYGOMA_BONE:
			searchWstr = WIDE("SkellBossDead_LeftBigPart");
			animWstr = WIDE("SkellBossBodyParts_LeftUpperZaygomaBone");
			break;
		case hj::bodyType::LEFT_LOWER_ZAYGOMA_BONE:
			searchWstr = WIDE("SkellBossDead_LeftSmallPart");
			animWstr = WIDE("SkellBossBodyParts_LeftLowerZaygomaBone");
			break;
		case hj::bodyType::RIGHT_UPPER_ZAYGOMA_BONE:
			searchWstr = WIDE("SkellBossDead_RightBigPart");
			animWstr = WIDE("SkellBossBodyParts_RightUpperZaygomaBone");
			break;
		case hj::bodyType::RIGHT_LOWER_ZAYGOMA_BONE:
			searchWstr = WIDE("SkellBossDead_RightSmallPart");
			animWstr = WIDE("SkellBossBodyParts_RightLowerZaygomaBone");
			break;
		case hj::bodyType::JAW:
			searchWstr = WIDE("SkellBossDead_Jaw");
			animWstr = WIDE("SkellBossBodyParts_Jaw");
			break;
		case hj::bodyType::End:
			searchWstr = WIDE("SkellBossDead_Whole");
			animWstr = WIDE("SkellBossBodyParts_Whole");
			break;
		default:
			break;
		}

		float frameDuration = 1.f;
		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD_OFFSET, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration, false, 0, false);
		Actor::CreateAnimation(eTextureSlot::T0, true);

		Animator* animator = GetComponent<Animator>();
		animator->Play(animWstr);
	}
}
