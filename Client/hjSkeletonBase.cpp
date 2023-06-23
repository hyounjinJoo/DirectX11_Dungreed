#include "hjSkeletonBase.h"
#include "hjAnimator.h"
#include "hjSceneManager.h"
#include "hjScene.h"
#include "hjLayer.h"
#include "hjPlayer.h"
#include "hjRigidBody.h"
#include "hjSkeletonHand.h"
#include "hjCollider2D.h"
#include "hjObject.h"
#include "hjWeapon.h"
#include "hjOakBow.h"
#include "hjTime.h"
#include "hjRustyShortSword.h"
#include "hjRustyGreatSword.h"
#include "hjSpriteRenderer.h"

namespace hj
{
	SkeletonBase::SkeletonBase()
		: Monster()
		, mCenterObj(nullptr)
		, mHand(nullptr)
		, mSkelState(eSkeletonState::End)
		, mbIsFlip(false)
		, mAnimator(nullptr)
		, mRigidBody(nullptr)
		, mSkelType(eSkeletonType::End)
		, mWeapon(nullptr)
		, mArmScript(nullptr)
		, mChaseDistance(400.f)
		, mCanAttackDistance(80.f)
		, mReadyAttackTimer(1.f)
		, mReadyAttackTime(1.f)
		, mMonsterBody(nullptr)
	{
		mMonsterStatus.maxHP = 50;
		mMonsterStatus.currentHP = mMonsterStatus.maxHP;

		mAnimator = AddComponent<Animator>();
		if (mAnimator)
		{
			CreateAnimation();
		}

		mMonsterBody = object::Instantiate<Actor>(eLayerType::Monster);
		mMonsterBody->AddComponent<Collider2D>();

		mMonsterBody->SetScaleXY(Vector2(50.f, 85.f));
		mMonsterBody->SetPositionY(-85.f * 0.5f);
		mMonsterBody->GetTransform()->SetParent(this->GetTransform());
		mMonsterBody->SetOwnerActor(this);

		AddComponent<Collider2D>();
		mRigidBody = AddComponent<RigidBody>();

		mCenterObj = object::Instantiate<GameObject>(eLayerType::MonsterHas);
		mCenterObj->SetName(WIDE("½ºÄÌ·¹Åæ ÇÚµå ¼ÒÄÏ"));
		mCenterObj->SetPositionY(-20.f);
		mCenterObj->GetTransform()->SetParent(GetTransform());
		
		mArmScript = mCenterObj->AddComponent<ArmRotatorScript>();
		mArmScript->SetBody(this);

		mHand = object::Instantiate<SkeletonHand>(eLayerType::MonsterHas, Vector3(GetScaleX() * 0.5f, 0.f, 1.f));
		mHand->GetTransform()->SetParent(mCenterObj->GetTransform());
		mHand->GetTransform()->SetInheritParentScale(false);
		mHand->SetHandOwner(this);

		mArmScript->SetTarget(mHand);
		mArmScript->SetUsingMouseRotation(false);
		mArmScript->SetUsingManualRotation(true);
	}

	SkeletonBase::~SkeletonBase()
	{
		if (mMonsterBody)
		{
			mMonsterBody = nullptr;
		}
		if (mCenterObj)
		{
			mCenterObj = nullptr;
		}
		if (mWeapon)
		{
			mWeapon = nullptr;
		}
		if (mHand)
		{
			mHand = nullptr;
		}
		if (mPlayer)
		{
			mPlayer = nullptr;
		}
		if (mArmScript)
		{
			mArmScript = nullptr;
		}
	}

	void SkeletonBase::Initialize()
	{
		Monster::Initialize();
	}

	void SkeletonBase::Update()
	{
		switch (mSkelState)
		{
		case hj::eSkeletonState::Idle:
		{
			Animation* currentAnimation = mAnimator->GetCurrentAnimation();
			if (currentAnimation && (currentAnimation->AnimationName() != WIDE("Idle")))
			{
				mAnimator->Play(WIDE("Idle"));
			}
			
			currentAnimation = nullptr;

			if (mPlayer)
			{
				float distance = CalcDistanceFromPlayer();
				if (distance < mChaseDistance)
				{
					ChangeState(eSkeletonState::Chase);
				}
			}
			else
			{
				FindPlayer();
			}
		}
			break;
		case hj::eSkeletonState::Chase:
		{
			if (mPlayer)
			{
				float distance = CalcDistanceFromPlayer();
				if (distance < mCanAttackDistance)
				{
					ChangeState(eSkeletonState::CanAttack);
				}
				else
				{
					Chase();
				}
			}
			else
			{
				FindPlayer();
			}

			FlipBasedOnPlayerPos();
		}
			break;
		case hj::eSkeletonState::CanAttack:
		{
			FlipBasedOnPlayerPos();
			mReadyAttackTimer -= Time::FixedDeltaTime();

			if (mReadyAttackTimer > 0.2f)
			{
				if ((eSkeletonType::Bow == mSkelType) && mArmScript && mPlayer)
				{
					mArmScript->SetManualTargetPosition(mPlayer->GetWorldPositionXY());
				}
			}

			if (0 >= mReadyAttackTimer)
			{
				mReadyAttackTimer = mReadyAttackTime;

				ChangeState(eSkeletonState::Attack);
			}
		}
			break;
		case hj::eSkeletonState::Attack:
		{
			Animator* weaponAnim = mWeapon->GetComponent<Animator>();
			if (weaponAnim && weaponAnim->GetCurrentAnimation())
			{
				bool isComplete = weaponAnim->GetCurrentAnimation()->IsComplete();
				if (isComplete)
				{
					ChangeState(eSkeletonState::Idle);
				}
			}

			weaponAnim = nullptr;
		}
		break;
		case hj::eSkeletonState::End:
		{
			ChangeState(eSkeletonState::Idle);
		}
			break;
		case hj::eSkeletonState::Death:
			break;
		default:
			break;
		}

		Monster::Update();
	}

	void SkeletonBase::FixedUpdate()
	{
		Monster::FixedUpdate();
	}

	void SkeletonBase::Render()
	{
		if (mDamagedEffectTimer > 0.f)
		{
			mDamagedEffectTimer -= Time::ActualDeltaTime();

			SpriteRenderer* sr = GetComponent<SpriteRenderer>();
			std::shared_ptr<Material> material = sr->GetMaterial();
			int useDiffuse = 1;
			material->SetData(eGPUParam::Int_4, &useDiffuse);
			Vector4 damagedColor = Vector4(1.f, 0.f, 0.f, 1.f);
			material->SetData(eGPUParam::Vector4_1, &damagedColor);

			if (mDamagedEffectTimer <= 0.f)
			{
				mDamagedEffectTimer = 0.f;
				int useDiffuse = 0;
				material->SetData(eGPUParam::Int_4, &useDiffuse);
			}
		}


		Monster::Render();

		SpriteRenderer* sr = GetComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = sr->GetMaterial();
		int useDiffuse = 0;
		material->SetData(eGPUParam::Int_4, &useDiffuse);
		Vector4 damagedColor = Vector4::Zero;
		material->SetData(eGPUParam::Vector4_1, &damagedColor);
	}

	void SkeletonBase::ProcessDamaged(int damage)
	{
		mDamagedEffectTimer = 0.1f;
	}

	void SkeletonBase::SetType(eSkeletonType type)
	{
		if (type == eSkeletonType::End || mWeapon)
		{
			return;
		}
		mSkelType = type;
		switch (type)
		{
		case hj::eSkeletonType::Sword:
			CreateSword();
			break;
		case hj::eSkeletonType::GreatSword:
			CreateGreatSword();
			break;
		case hj::eSkeletonType::Bow:
			CreateBow();
			break;
		case hj::eSkeletonType::End:
		default:
			break;
		}

		if (mWeapon || mOwnerRoom)
		{
			mWeapon->SetOwnerRoom(mOwnerRoom);
		}
	}

	void SkeletonBase::CreateSword()
	{
		mWeapon = object::Instantiate<object::item::weapon::RustyShortSword>(eLayerType::MonsterAttack_NotForeGround);
		mWeapon->SetPositionX(60.f);
		mWeapon->SetPositionZ(-0.1f);
		mWeapon->GetTransform()->SetParent(this->GetTransform());

		mChaseDistance = 800.f;
		mCanAttackDistance = 120.f;
		mMoveSpeed = 500.f;
		mReadyAttackTime = 0.f;
	}

	void SkeletonBase::CreateGreatSword()
	{
		mWeapon = object::Instantiate<object::item::weapon::RustyGreatSword>(eLayerType::MonsterAttack_NotForeGround);
		mWeapon->SetPositionX(60.f);
		mWeapon->SetPositionZ(-0.1f);
		mWeapon->GetTransform()->SetParent(this->GetTransform());

		mChaseDistance = 800.f;
		mCanAttackDistance = 120.f;
		mMoveSpeed = 500.f;
		mReadyAttackTime = 0.f;
	}

	void SkeletonBase::CreateBow()
	{
		mWeapon = object::Instantiate<object::item::weapon::OakBow>(eLayerType::MonsterHas);
		mWeapon->SetPositionX(-7.f);
		mWeapon->SetPositionZ(0.1f);

		if (mHand)
		{
			dynamic_cast<object::item::weapon::Weapon*>(mWeapon)->SetHand(mHand);
			mHand->SetWeapon(mWeapon);
			mChaseDistance = 800.f;
			mCanAttackDistance = mChaseDistance;
		}
	}
	
	void SkeletonBase::Damaged(int damage)
	{
		SubtractCurrentHP(damage);
		ProcessDamaged(damage);
		if (mbIsDeadMonster)
		{
			ChangeState(eSkeletonState::Death);
		}
	}

	void SkeletonBase::SetOwnerRoom(RoomBase* room)
	{
		if (mWeapon)
		{
			mWeapon->SetOwnerRoom(room);
		}

		if (mMonsterBody)
		{
			mMonsterBody->SetOwnerRoom(room);
		}

		if (mHand)
		{
			mHand->SetOwnerRoom(room);
		}

		Monster::SetOwnerRoom(room);
	}

	void SkeletonBase::CreateAnimation()
	{
		SetNameAndCreateSpriteRenderer(GetName(),
			WIDE("MTRL_Stage1"), WIDE("Mesh_Rect"));

		CheckHasMaterialAndTexture(eTextureSlot::T0);

		std::wstring path = WIDE("02_Object/02_Monster/00_Normal/Stage01.xml");
		std::wstring searchWstr = WIDE("/SkelIdle");
		std::wstring animWstr = WIDE("Idle");
		float frameDuration = 1.f;

		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD_OFFSET, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration);
		
		searchWstr = WIDE("SkelWalk");
		animWstr = WIDE("Walk");
		frameDuration = 1.f / 15.f;

		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD_OFFSET, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration, true, 0);

		searchWstr = WIDE("SkelCorpse");
		animWstr = WIDE("Attack");
		frameDuration = 1.f;

		LoadAnimInfoFromFile(eFrameAddMethod::FRAME_ADD_OFFSET, eTextureSlot::T0, path, searchWstr, animWstr, frameDuration);

		Actor::InsertAnimationToAnimator(eTextureSlot::T0);

		Animator* animator = GetComponent<Animator>();
		animator->Play(WIDE("Idle"));
	}

	void SkeletonBase::SpawnBodyParts()
	{

	}

	void SkeletonBase::FlipBasedOnPlayerPos()
	{
		bool isChanged = false;
		bool prevFlip = mbIsFlip;

		if (mChaseDir.x >= 0.f)
		{
			SetRotationY(0.f);
			mCenterObj->SetRotationY(0.f);
			mbIsFlip = false;
		}
		else
		{
			SetRotationY(PI);
			mCenterObj->SetRotationY(PI);
			mbIsFlip = true;
		}

		if (prevFlip != mbIsFlip)
		{
			isChanged = true;
		}

		if (isChanged)
		{
			ArmRotatorScript* armScript = mCenterObj->GetScript<ArmRotatorScript>();

			if (armScript)
			{
				armScript->InverseArmAxis(Axis::Z);
			}
		}
	}

	void SkeletonBase::ChangeState(eSkeletonState state)
	{
		if (mSkelState == state)
		{
			return;
		}

		if (mSkelState == eSkeletonState::Death)
		{
			return;
		}

		mSkelState = state;
		switch (state)
		{
		case hj::eSkeletonState::Idle:
			Idle();
			break;
		case hj::eSkeletonState::Chase:
			StartChase();
			break;
		case hj::eSkeletonState::Attack:
			Attack();
			break;
		case hj::eSkeletonState::Death:
			Death();
			break;
		default:
			break;
		}
	}

	void SkeletonBase::Idle()
	{
		if (mAnimator)
		{
			mAnimator->Play(WIDE("Idle"), true);
		}

		if (eSkeletonType::Bow != mSkelType)
		{
			//mWeapon->GetComponent<Animator>()->Play(WIDE("Idle"), true);
		}
	}

	void SkeletonBase::StartChase()
	{
		if (!mPlayer)
		{	
			FindPlayer();
			if (!mPlayer)
			{
				return;
			}
		}

		CalcChaseDir();
	}

	void SkeletonBase::Chase()
	{
		if (mAnimator)
		{
			Animation* currentAnimation = mAnimator->GetCurrentAnimation();

			if (currentAnimation && currentAnimation->AnimationName() != WIDE("Walk"))
			{
				mAnimator->Play(WIDE("Walk"), true);
			}
		}
		CalcChaseDir();

		Vector2 finalMoveForce = mChaseDir;
		finalMoveForce.x = finalMoveForce.x > 0.f ? 1.f : -1.f;
		finalMoveForce.y = 0.f;

		mRigidBody->AddForce(finalMoveForce * mMoveSpeed);
	}

	void SkeletonBase::Attack()
	{
		if (!mHand)
		{
			return;
		}

		if (mAnimator)
		{
			Animation* currentAnimation = mAnimator->GetCurrentAnimation();

			if (currentAnimation && currentAnimation->AnimationName() != WIDE("Attack"))
			{
				mAnimator->Play(WIDE("Attack"), true);
			}
		}

		if (eSkeletonType::Bow == mSkelType)
		{
			mHand->Attack();
		}
		else
		{
			object::item::weapon::Weapon* weaponPtr = dynamic_cast<object::item::weapon::Weapon*>(mWeapon);
			if (weaponPtr)
			{
				weaponPtr->Attack();
			}
		}
	}

	void SkeletonBase::Death()
	{
		SpawnBodyParts();
		GameObject::Death();
		mHand->Death();
		mCenterObj->Death();
		mWeapon->Death();
		mMonsterBody->Death();
	}
}