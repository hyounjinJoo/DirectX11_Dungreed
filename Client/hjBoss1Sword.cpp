#include "hjBoss1Sword.h"
#include "hjSpriteRenderer.h"
#include "hjResources.h"
#include "hjAnimator.h"
#include "hjXmlParser.h"
#include "hjObject.h"
#include "hjPlayer.h"
#include "hjTime.h"
#include "hjCollider2D.h"
#include "hjBoss1SwordStuckCollider.h"
#include "hjBoss1SwordAttackCollider.h"
#include "hjInput.h"
#include "hjBoss1SwordCreateDestroyFx.h"

namespace hj
{
	Boss1Sword::Boss1Sword()
		: mPlayer(nullptr)
		, mChargeEffectObj(nullptr)
		, mHitEffectObj(nullptr)
		, mSwordMoveCollider(nullptr)
		, mSwordAttackCollider(nullptr)
		, mCreateDestroyFx(nullptr)
		, mSwordState(Boss1SwordState::End)
		, mCurSpawnedTime(0.f)
		, mSpawnEndTime(0.5f)
		, mCurAimingTime(0.f)
		, mAimingLimitTime(1.5f)
		, mMoveDir(Vector2::Zero)
		, mMoveSpeed(3200.f)
		, mMoveVelocity(Vector2::Zero)
		, mSpawnPosXY(Vector2::Zero)
	{
		SetName(WIDE("Effect_Boss1_Sword"));

		// 1. Sprite Renderer 생성
		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = MTRL_FIND_STR("MTRL_Monster_Boss_Bellial");
		std::shared_ptr<Mesh> mesh = MESH_FIND_STR("Mesh_Rect");
		sr->SetMaterial(material);
		sr->SetMesh(mesh);

		// 2. Animator 생성 및 Animation 추가
		Animator* animator = AddComponent<Animator>();
		if (material && animator)
		{
			std::shared_ptr<Texture> texture = material->GetTexture(eTextureSlot::T0);
			if (texture)
			{
				mChargeEffectObj = object::Instantiate<Actor>(eLayerType::MonsterHas);
				sr = mChargeEffectObj->AddComponent<SpriteRenderer>();
				sr->SetMaterial(material);
				sr->SetMesh(mesh);
				mChargeEffectObj->AddPositionZ(-1.f);
				mChargeEffectObj->AddComponent<Animator>();
				mHitEffectObj = object::Instantiate<Actor>(eLayerType::MonsterHas);
				sr = mHitEffectObj->AddComponent<SpriteRenderer>();
				sr->SetMaterial(material);
				sr->SetMesh(mesh);
				mHitEffectObj->AddPositionZ(-1.f);
				mHitEffectObj->AddComponent<Animator>();

				CreateAnimation();
			}
		}

		// 충돌체 생성
		mSwordMoveCollider = object::Instantiate<Boss1SwordStuckCollider>(eLayerType::MonsterAttack_ForeGround);
		mSwordMoveCollider->SetOwnerObject(this);

		mSwordAttackCollider = object::Instantiate<Boss1SwordAttackCollider>(eLayerType::MonsterAttack_ForeGround);
		mSwordAttackCollider->SetOwnerObject(this);		

		Transform* thisTransform = this->GetTransform();
		mChargeEffectObj->GetTransform()->SetParent(thisTransform);
		mHitEffectObj->GetTransform()->SetParent(thisTransform);

		Vector2 thisScaleXY = thisTransform->GetScaleXY();
		Vector2 ColliderScale = thisScaleXY;
		
		ColliderScale.x *= 0.4f;
		mSwordAttackCollider->SetScaleXY(ColliderScale);
		mSwordAttackCollider->GetTransform()->SetParent(thisTransform);

		ColliderScale.x = thisScaleXY.x;
		ColliderScale.x *= 0.1f;
		ColliderScale.y *= 0.1f;

		Transform* moveColliderTR = mSwordMoveCollider->GetTransform();
		moveColliderTR->SetScaleXY(ColliderScale);
		moveColliderTR->SetPositionY(GetScaleY() * 0.2f);
		moveColliderTR->SetParent(this->GetTransform()); 

		mCreateDestroyFx = object::Instantiate<Boss1SwordCreateDestroyFx>(eLayerType::MonsterHas);
		mCreateDestroyFx->SetPositionZ(-0.1f);

		mSwordMoveCollider->SetNotActiveByRoom();
		mSwordAttackCollider->SetNotActiveByRoom();
		mHitEffectObj->SetNotActiveByRoom();
		mChargeEffectObj->SetNotActiveByRoom();
		mCreateDestroyFx->SetNotActiveByRoom();
		this->SetNotActiveByRoom();
	}

	Boss1Sword::Boss1Sword(const Boss1Sword& sword)
		: Actor(sword)
		, mPlayer(sword.mPlayer)
		, mChargeEffectObj(nullptr)
		, mHitEffectObj(nullptr)
		, mSwordMoveCollider(nullptr)
		, mSwordAttackCollider(nullptr)
		, mCreateDestroyFx(nullptr)
		, mSwordState(Boss1SwordState::End)
		, mCurSpawnedTime(0.f)
		, mSpawnEndTime(sword.mSpawnEndTime)
		, mCurAimingTime(0.f)
		, mAimingLimitTime(sword.mAimingLimitTime)
		, mMoveDir(Vector2::Zero)
		, mMoveSpeed(3200.f)
		, mMoveVelocity(Vector2::Zero)
		, mSpawnPosXY(Vector2::Zero)
	{
		Transform* thisTransform = this->GetTransform();
		
		mChargeEffectObj = object::Clone<Actor>(sword.mChargeEffectObj);
		mChargeEffectObj->GetTransform()->SetParent(thisTransform);
		
		mHitEffectObj = object::Clone<Actor>(sword.mHitEffectObj);
		mHitEffectObj->GetTransform()->SetParent(thisTransform);

		mSwordMoveCollider = object::Clone<Boss1SwordStuckCollider>(sword.mSwordMoveCollider);
		mSwordMoveCollider->SetOwnerObject(this);
		mSwordMoveCollider->GetTransform()->SetParent(thisTransform);

		mSwordAttackCollider = object::Clone<Boss1SwordAttackCollider>(sword.mSwordAttackCollider);
		mSwordAttackCollider->SetOwnerObject(this);
		mSwordAttackCollider->GetTransform()->SetParent(thisTransform);

		mCreateDestroyFx = object::Clone<Boss1SwordCreateDestroyFx>(sword.mCreateDestroyFx);

		Animator* animator = mHitEffectObj->GetComponent<Animator>();
		std::wstring animWstr = WIDE("Effect_Bellial_Sword_Hit");
		animator->GetCompleteEvent(animWstr) = std::bind(&Boss1Sword::PlayEndHitFX, this);

		mSwordMoveCollider->SetNotActiveByRoom();
		mSwordAttackCollider->SetNotActiveByRoom();
		mHitEffectObj->SetNotActiveByRoom();
		mChargeEffectObj->SetNotActiveByRoom();
		mCreateDestroyFx->SetNotActiveByRoom();
		this->SetNotActiveByRoom();
	}

	Boss1Sword::~Boss1Sword()
	{
		mCreateDestroyFx = nullptr;
		mChargeEffectObj = nullptr;
		mHitEffectObj = nullptr;
		mSwordAttackCollider = nullptr;
		mSwordMoveCollider = nullptr;
		mPlayer = nullptr;
	}

	void Boss1Sword::Initialize()
	{
		GameObject::Initialize();

		if (!mOwnerRoom)
			return;

		mSwordMoveCollider->SetOwnerRoom(mOwnerRoom);
		mSwordAttackCollider->SetOwnerRoom(mOwnerRoom);
		mHitEffectObj->SetOwnerRoom(mOwnerRoom);
		mChargeEffectObj->SetOwnerRoom(mOwnerRoom);
		mCreateDestroyFx->SetOwnerRoom(mOwnerRoom);
	}

	void Boss1Sword::Update()
	{
		if (eState::Paused == mState || eState::NotActiveByRoom == mState)
			return;

		switch (mSwordState)
		{
		case Boss1SwordState::Spawn:
			CheckSpawnEnd();
			break;
		case Boss1SwordState::Aim:
			Aim();
			break;
		case Boss1SwordState::Shot:
			Move();
			break;
		case Boss1SwordState::Stuck:
		{
			mCurEndTime += Time::ActualDeltaTime();
			if (mCurEndTime > mEndLimitTime)
				ChangeSwordState(Boss1SwordState::End);
		}
			break;
		case Boss1SwordState::End:
		default:
			break;
		}

		GameObject::Update();
	}
	void Boss1Sword::FixedUpdate()
	{
		if (eState::Paused == mState || eState::NotActiveByRoom == mState)
			return;

		GameObject::FixedUpdate();
	}
	void Boss1Sword::Render()
	{
		if (eState::Paused == mState || eState::NotActiveByRoom == mState)
			return;

		GameObject::Render();
	}

	void Boss1Sword::Death()
	{
		if (mPlayer)
		{
			mPlayer = nullptr;
		}

		if (mChargeEffectObj)
		{
			mChargeEffectObj->Death();
		}

		if (mHitEffectObj)
		{
			mHitEffectObj->Death();
		}

		if (mCreateDestroyFx)
		{
			mCreateDestroyFx->Death();
		}

		if (mSwordMoveCollider)
		{
			mSwordMoveCollider->Death();
		}

		if (mSwordAttackCollider)
		{
			mSwordAttackCollider->Death();
		}

		Actor::Death();
	}

	GameObject* Boss1Sword::Clone() const
	{
		return new Boss1Sword(*this);
	}

	void Boss1Sword::PauseAnimation()
	{
		Actor::PauseAnimation();

		if (mChargeEffectObj)
			mChargeEffectObj->PauseAnimation();
		
		if(mHitEffectObj)
			mHitEffectObj->PauseAnimation();

		if (mCreateDestroyFx)
			mCreateDestroyFx->PauseAnimation();

		if (mSwordMoveCollider)
			mSwordMoveCollider->Pause();

		if (mSwordAttackCollider)
			mSwordAttackCollider->Pause();
	}

	void Boss1Sword::PauseAll()
	{
		if (mChargeEffectObj)
			mChargeEffectObj->Pause();

		if (mHitEffectObj)
			mHitEffectObj->Pause();

		if (mCreateDestroyFx)
			mCreateDestroyFx->Pause();

		this->Pause();
	}

	// HitEffect의 위치와 회전을 조정
	void Boss1Sword::SetHitEffectPosAndRot(const Vector2& newPos, float rotZ)
	{
		if (!mHitEffectObj)
		{
			return;
		}

		Transform* hitTR = mHitEffectObj->GetTransform();
		hitTR->SetParent(nullptr);
		hitTR->SetPositionXY(newPos);
		hitTR->SetRotationZ(rotZ);
	}

	// HitEffect의 위치와 회전을 조정해서 설정하기 위해 Scale을 반환해주는 함수
	hj::math::Vector2 Boss1Sword::GetHitEffectScaleXY()
	{
		Vector2 hitEffectSize = Vector2::One;

		if (!mHitEffectObj)
			return hitEffectSize;

		Transform* hitTR = mHitEffectObj->GetTransform();
		hitEffectSize = hitTR->GetScaleXY();

		return hitEffectSize;
	}

	void Boss1Sword::ChangeSwordState(Boss1SwordState nextState)
	{
		if (mSwordState == nextState)
			return;

		switch (nextState)
		{
		case Boss1SwordState::Spawn:
			mSwordState = Boss1SwordState::Spawn;
			AttackEnd();
			Spawn();
			break;
		case Boss1SwordState::Aim:
			mSwordState = Boss1SwordState::Aim;
			break;
		case Boss1SwordState::Shot:
			mSwordState = Boss1SwordState::Shot;
			Shot();
			break;
		case Boss1SwordState::Stuck:
			mSwordState = Boss1SwordState::Stuck;
			Stuck();
			break;
		case Boss1SwordState::End:
			mSwordState = Boss1SwordState::End;
			AttackEnd();
			break;
		case Boss1SwordState::Dead:
			mSwordState = Boss1SwordState::Dead;
			DeadProcess();
		default:
			break;

		}
	}

	void Boss1Sword::PlayEndHitFX()
	{
		mHitEffectObj->Pause();
	}

	void Boss1Sword::Spawn()
	{
		// Spawn 시 해 주어야 할 이펙트 발생 등...
		this->Activate();
		SetRotationZ(XM_PI);
		GetTransform()->FixedUpdate();
		mChargeEffectObj->Activate();
	}

	void Boss1Sword::CheckSpawnEnd()
	{
		// Aim이 되었을 때 처리해야 할 일들...
		if (mCurSpawnedTime < mSpawnEndTime)
		{
			mCurSpawnedTime += Time::ActualDeltaTime();
			if (mCurSpawnedTime > mSpawnEndTime)
			{
				SpawnComplete();
			}
		}
	}

	void Boss1Sword::Aim()
	{
		// Aim이 되었을 때 처리해야 할 일들...
		if (mCurAimingTime < mAimingLimitTime)
		{
			mCurAimingTime += Time::ActualDeltaTime();
			AimPlayer();
			if (mCurAimingTime > mAimingLimitTime)
			{
				AimComplete();
			}
		}
	}

	// MoveCollider 활성화, Charge Effect 비활성화
	void Boss1Sword::Shot()
	{
		// Charge 비활성화
		mChargeEffectObj->Pause();
		mSwordMoveCollider->Activate();
		mSwordAttackCollider->Activate();
	}

	// Shot 상태일 때 이동 처리
	void Boss1Sword::Move()
	{
		AddPositionXY(mMoveVelocity * Time::FixedDeltaTime());
	}

	// 벽에 도달했을 때 Body를 변경. Hit Effect 활성화 및 재생
	void Boss1Sword::Stuck()
	{
		Animator* animator = GetComponent<Animator>();
		if (!animator)
			return;

		Animation* animation = animator->FindAnimation(WIDE("Effect_Bellial_Sword_Body"));
		if (!animation)
			return;

		animator->Play(animation->AnimationName(), true);

		// Hit Effect 활성화 및 재생
		mHitEffectObj->Activate();
		animator = mHitEffectObj->GetComponent<Animator>();

		if (!animator)
			return;

		animation = animator->FindAnimation(WIDE("Effect_Bellial_Sword_Hit"));
		if (!animation)
			return;

		animator->Play(animation->AnimationName(), false);

		mSwordAttackCollider->Pause();
	}

	// Sword 관련 애니메이션 생성
	void Boss1Sword::CreateAnimation()
	{
		XmlParser* parser = new XmlParser;
		std::wstring path = WIDE("02_Object/02_Monster/01_Boss/Monster_Boss_1_Bellial.xml");
		bool parseResult = parser->LoadFile(path);

		if (!parseResult)
		{
			delete parser;
			return;
		}
		parseResult = parser->FindElem(WIDE("TextureAtlas"));
		parseResult = parser->IntoElem();

		std::wstring targetSpriteNameWstr;
		int count = 0;


		SpriteRenderer* sr = mChargeEffectObj->GetComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = sr->GetMaterial();
		std::shared_ptr<Texture> texture = material->GetTexture(eTextureSlot::T0);
		Vector2 atlasTexSize = texture->GetTexSize();

		float duration = 0.f;
		duration = 1.f / 10.f;

		Vector2 leftTop = Vector2::Zero;
		Vector2 size = Vector2::One;
		Vector2 offset = Vector2::Zero;

		size_t checkStringParseValue = std::wstring::npos;

		{
			CREATE_ANIM(animBellialSwordCharge, frame, atlasTexSize, duration);
			while (parseResult)
			{
				parseResult = parser->FindElem(WIDE("sprite"));
				if (!parseResult)
				{
					parseResult = true;
					break;
				}

				targetSpriteNameWstr = WIDE("BossSwordCharge") + std::to_wstring(count);
				checkStringParseValue = parser->CheckAttributeIncludeWstr(WIDE("n"), targetSpriteNameWstr);
				if (std::wstring::npos != checkStringParseValue)
				{
					leftTop.x = static_cast<float>(parser->GetIntAttribute(WIDE("x")));
					leftTop.y = static_cast<float>(parser->GetIntAttribute(WIDE("y")));
					size.x = static_cast<float>(parser->GetIntAttribute(WIDE("w")));
					size.y = static_cast<float>(parser->GetIntAttribute(WIDE("h")));
					offset.x = static_cast<float>(parser->GetFloatAttribute(WIDE("pX")));
					offset.y = static_cast<float>(parser->GetFloatAttribute(WIDE("pY")));

					FRAME_ADD_OFFSET(frame, leftTop.x, leftTop.y, size.x, size.y, offset.x, offset.y, animBellialSwordCharge);

					++count;
				}
			}

			Animator* animator = mChargeEffectObj->GetComponent<Animator>();
			std::wstring animWstr = WIDE("Effect_Bellial_Sword_Charge");
			animator->Create(animWstr, texture, animBellialSwordCharge, canvasSize, false);
			animator->Play(animWstr);
			mChargeEffectObj->SetScaleXY(canvasSize);
		}
		parser->ResetMainPos();
		count = 0;

		sr = mHitEffectObj->GetComponent<SpriteRenderer>();
		material = sr->GetMaterial();
		texture = material->GetTexture(eTextureSlot::T0);
		atlasTexSize = texture->GetTexSize();

		{
			duration = 1.f / 10.f;
			CREATE_ANIM(animBellialSwordHit, frame, atlasTexSize, duration);

			while (parseResult)
			{
				parseResult = parser->FindElem(WIDE("sprite"));
				if (!parseResult)
				{
					parseResult = true;
					break;
				}

				targetSpriteNameWstr = WIDE("BossSwordHitFX") + std::to_wstring(count);
				checkStringParseValue = parser->CheckAttributeIncludeWstr(WIDE("n"), targetSpriteNameWstr);
				if (std::wstring::npos != checkStringParseValue)
				{
					leftTop.x = static_cast<float>(parser->GetIntAttribute(WIDE("x")));
					leftTop.y = static_cast<float>(parser->GetIntAttribute(WIDE("y")));
					size.x = static_cast<float>(parser->GetIntAttribute(WIDE("w")));
					size.y = static_cast<float>(parser->GetIntAttribute(WIDE("h")));
					offset.x = static_cast<float>(parser->GetFloatAttribute(WIDE("pX")));
					offset.y = static_cast<float>(parser->GetFloatAttribute(WIDE("pY")));

					FRAME_ADD_OFFSET(frame, leftTop.x, leftTop.y, size.x, size.y, offset.x, offset.y, animBellialSwordHit);

					++count;
				}
			}

			Animator* animator = mHitEffectObj->GetComponent<Animator>();
			std::wstring animWstr = WIDE("Effect_Bellial_Sword_Hit");
			animator->Create(animWstr, texture, animBellialSwordHit, canvasSize, false);
			animator->Play(animWstr);
			mHitEffectObj->SetScaleXY(canvasSize);

			animator->GetCompleteEvent(animWstr) = std::bind(&Boss1Sword::PlayEndHitFX, this);
		}
		parser->ResetMainPos();
		count = 0;

		sr = this->GetComponent<SpriteRenderer>();
		material = sr->GetMaterial();
		texture = material->GetTexture(eTextureSlot::T0);
		atlasTexSize = texture->GetTexSize();
		{
			duration = 1.f / 10.f;
			CREATE_ANIM(animBellialSwordBody, frame, atlasTexSize, duration);

			while (parseResult)
			{
				parseResult = parser->FindElem(WIDE("sprite"));
				if (!parseResult)
				{
					parseResult = true;
					break;
				}

				targetSpriteNameWstr = WIDE("SwordBody/BossSword");
				checkStringParseValue = parser->CheckAttributeIncludeWstr(WIDE("n"), targetSpriteNameWstr);
				if (std::wstring::npos != checkStringParseValue)
				{
					leftTop.x = static_cast<float>(parser->GetIntAttribute(WIDE("x")));
					leftTop.y = static_cast<float>(parser->GetIntAttribute(WIDE("y")));
					size.x = static_cast<float>(parser->GetIntAttribute(WIDE("w")));
					size.y = static_cast<float>(parser->GetIntAttribute(WIDE("h")));
					offset.x = static_cast<float>(parser->GetFloatAttribute(WIDE("pX")));
					offset.y = static_cast<float>(parser->GetFloatAttribute(WIDE("pY")));

					FRAME_ADD_OFFSET(frame, leftTop.x, leftTop.y, size.x, size.y, offset.x, offset.y, animBellialSwordBody);

					break;
				}
			}
			parser->ResetMainPos();

			CREATE_SHEET(animBellialSwordBodyFx);
			while (parseResult)
			{
				parseResult = parser->FindElem(WIDE("sprite"));
				if (!parseResult)
				{
					parseResult = true;
					break;
				}

				targetSpriteNameWstr = WIDE("BossSwordFX");
				checkStringParseValue = parser->CheckAttributeIncludeWstr(WIDE("n"), targetSpriteNameWstr);
				if (std::wstring::npos != checkStringParseValue)
				{
					leftTop.x = static_cast<float>(parser->GetIntAttribute(WIDE("x")));
					leftTop.y = static_cast<float>(parser->GetIntAttribute(WIDE("y")));
					size.x = static_cast<float>(parser->GetIntAttribute(WIDE("w")));
					size.y = static_cast<float>(parser->GetIntAttribute(WIDE("h")));
					offset.x = static_cast<float>(parser->GetFloatAttribute(WIDE("pX")));
					offset.y = static_cast<float>(parser->GetFloatAttribute(WIDE("pY")));

					FRAME_ADD_OFFSET(frame, leftTop.x, leftTop.y, size.x, size.y, offset.x, offset.y, animBellialSwordBodyFx);
				}
			}

			Animator* animator = this->GetComponent<Animator>();
			std::wstring animBodyWstr = WIDE("Effect_Bellial_Sword_Body");
			std::wstring animBodyFxWstr = WIDE("Effect_Bellial_Sword_BodyFX");

			animator->Create(animBodyWstr, texture, animBellialSwordBody, canvasSize, false);
			animator->Create(animBodyFxWstr, texture, animBellialSwordBodyFx, canvasSize, false);
			animator->Play(animBodyWstr);
			this->SetScaleXY(canvasSize);
		}
		
		delete parser;
	}

	// 플레이어 방향으로 회전
	void Boss1Sword::AimPlayer()
	{
		if (!mPlayer)
		{
			std::vector<GameObject*> objects = SceneManager::GetActiveScene()->GetGameObjects(eLayerType::Player);

			for (GameObject* obj : objects)
			{
				if (dynamic_cast<Player*>(obj))
				{
					mPlayer = obj;
					break;
				}
			}

			if (!mPlayer)
				return;
		}
		Vector3 playerPos = mPlayer->GetWorldPosition();
		Vector3 swordPos = this->GetWorldPosition();

		Vector3 dir = Vector3(playerPos.x, playerPos.y, 0.f) - swordPos;
		dir.z = 0.f;
		dir.Normalize();
		mMoveDir = Vector2(dir.x, dir.y);

		float dot = Vector3::Right.Dot(dir);
		float angle = 1.5f * XM_PI - acosf(dot);
		Vector3 cross = Vector3::Right.Cross(dir);

		if (dir.y > 0.f)
		{
			angle = XM_PI - angle;
		}

		float nextAngle = GetRotationZ();
		float targetAngle = angle;
		float angleDiff = targetAngle - nextAngle;
		float rotationSpeed = 0.05f;
		if (std::fabsf(angleDiff) > rotationSpeed)
		{
			nextAngle += angleDiff > 0 ? rotationSpeed : -rotationSpeed;
		}
		else
		{
			nextAngle = targetAngle;
		}

		this->SetRotationZ(nextAngle);
	}

	// 조준이 끝나고 난 뒤 Shot으로 상태변경
	void Boss1Sword::AimComplete()
	{
		Animator* animator = GetComponent<Animator>();
		if (!animator)
			return;

		Animation* animation = animator->FindAnimation(WIDE("Effect_Bellial_Sword_BodyFX"));
		if (!animation)
			return;

		animator->Play(animation->AnimationName(), true);
		
		Vector3 moveDir = this->Up();
		mMoveDir = Vector2(moveDir.x, moveDir.y);
		mMoveDir.Normalize();
		mMoveVelocity = mMoveDir * mMoveSpeed;
		ChangeSwordState(Boss1SwordState::Shot);
	}

	// Aim으로 상태 변경 처리
	void Boss1Sword::SpawnComplete()
	{
		ChangeSwordState(Boss1SwordState::Aim);
	}

	void Boss1Sword::AttackEnd()
	{
		mCurSpawnedTime = 0.f;
		mCurAimingTime = 0.f;
		mCurEndTime = 0.f;

		// 다른 이펙트 정지 처리
		mChargeEffectObj->Pause();
		mSwordMoveCollider->Pause();
		mSwordAttackCollider->Pause();
		mHitEffectObj->Pause();

		if (Boss1SwordState::Spawn == mSwordState)
		{
			mCreateDestroyFx->SetPositionXY(mSpawnPosXY);
		}
		else
		{
			mCreateDestroyFx->SetPositionXY(GetPositionXY());
			mCreateDestroyFx->SetRotationZ(GetRotationZ() + XM_PI);
		}
		mCreateDestroyFx->GetTransform()->FixedUpdate();
		mCreateDestroyFx->PlayFx();
		mCreateDestroyFx->Activate();

		SetPosition(Vector3(mSpawnPosXY, 0.f));
		SetRotationZ(XM_PI);
		GetTransform()->FixedUpdate();
		mChargeEffectObj->FixedUpdate();
		mSwordMoveCollider->FixedUpdate();
		mSwordAttackCollider->FixedUpdate();
		mHitEffectObj->FixedUpdate();
		Transform* hitTR = mHitEffectObj->GetTransform();
		hitTR->SetParent(this->GetTransform());
		hitTR->FixedUpdate();

		this->Pause();
	}

	void Boss1Sword::DeadProcess()
	{
		PauseAnimation();
	}

}