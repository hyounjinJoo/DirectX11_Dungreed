#include "hjPlayerHand.h"
#include "hjPlayer.h"
#include "hjCollider2D.h"
#include "hjObject.h"
#include "hjSpriteRenderer.h"
#include "hjResourceManager.h"
#include "hjItem.h"
#include "hjArmRotatorScript.h"
#include "hjSwordOfExplorer.h"
#include "hjActor.h"
#include "hjOakBow.h"
#include "hjInput.h"

namespace hj
{
	PlayerHand::PlayerHand()
		: Hand()
		, mWeaponMelee(nullptr)
		, mWeaponRanged(nullptr)
	{
		SetName(WIDE("Player Hand Obj"));

		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = MTRL_FIND_STR("MTRL_Char_Adventurer");
		std::shared_ptr<Mesh> mesh = MESH_FIND_STR("Mesh_Rect");
		sr->SetMaterial(material);
		sr->SetMesh(mesh);

		if (material)
		{
			std::shared_ptr<Texture> texture = material->GetTexture(eTextureSlot::T0);
			if (texture)
			{
				CreateAnimation();
			}
		}

		mWeaponMelee = object::Instantiate<object::item::weapon::SwordOfExplorer>(eLayerType::PlayerHas);
		mWeaponMelee->GetTransform()->SetParent(this->GetTransform());
		mWeaponMelee->SetPositionY(24.f);
		mWeaponMelee->SetPositionZ(0.1f);
		
		mWeaponRanged = object::Instantiate<object::item::weapon::OakBow>(eLayerType::PlayerHas);
		mWeaponRanged->GetTransform()->SetParent(this->GetTransform());
		mWeaponRanged->SetPositionX(-7.f);
		mWeaponRanged->SetPositionZ(0.1f);
		mWeaponRanged->SetNotActiveByRoom();

		mWeapon = mWeaponMelee;
		dynamic_cast<object::item::weapon::Weapon*>(mWeapon)->SetHand(this);
	}

	PlayerHand::~PlayerHand()
	{
	}

	void PlayerHand::FixedUpdate()
	{
		Hand::FixedUpdate();

		if (Input::GetKeyDown(eKeyCode::N_1))
		{
			if (mWeapon != mWeaponMelee)
			{
				mWeapon = mWeaponMelee;
				mWeapon->Activate();
				mWeaponRanged->SetNotActiveByRoom();
				ResetMeleeWeaponInfo();
				InitRotatorScript(); 
				GetTransform()->FixedUpdate(); 

			}
		}
		else if (Input::GetKeyDown(eKeyCode::N_2))
		{
			if (mWeapon != mWeaponRanged)
			{
				mWeapon = mWeaponRanged;
				mWeapon->Activate();
				mWeaponMelee->SetNotActiveByRoom(); 
				ResetMeleeWeaponInfo();
				InitRotatorScript();
				GetTransform()->FixedUpdate();
			}
		}
	}

	void PlayerHand::SetHandOwner(Actor* owner)
	{
		Player* player = dynamic_cast<Player*>(owner);
		if (!player)
			return;

		mHandOwner = player;
		mHandOwnerTR = player->GetCenter()->GetTransform();

		mWeaponMelee->SetOwnerActor(owner);
		mWeaponRanged->SetOwnerActor(owner);

		std::vector<Script*> scripts = player->GetCenter()->GetScripts();

		for (auto script : scripts)
		{
			if (dynamic_cast<ArmRotatorScript*>(script))
			{
				mRotatorScript = dynamic_cast<ArmRotatorScript*>(script);

				InitRotatorScript();
			}
		}
	}

	void PlayerHand::Attack()
	{
		if (mWeapon)
		{
			object::item::weapon::Weapon* weaponPtr = dynamic_cast<object::item::weapon::Weapon*>(mWeapon);
			
			if (weaponPtr)
			{
				weaponPtr->Attack();
			}
		}
	}

	void PlayerHand::CreateAnimation()
	{
		std::shared_ptr<Material> material = MTRL_FIND_STR("MTRL_Char_Adventurer");
		std::shared_ptr<Texture> texture = material->GetTexture(eTextureSlot::T0);
		Vector2 atlasTexSize = texture->GetTexSize();

		float duration = 0.f;
		duration = 1.f;
		CREATE_ANIM(charHand0, frame, atlasTexSize, duration);
		FRAME_ADD(frame, 560.f, 615.f, 15.f, 15.f, charHand0);

		CREATE_SHEET(charHand1);
		FRAME_ADD(frame, 575.f, 615.f, 15.f, 15.f, charHand1);

		CREATE_SHEET(charHand2);
		FRAME_ADD(frame, 590.f, 615.f, 15.f, 15.f, charHand2);

		CREATE_SHEET(charHand3);
		FRAME_ADD(frame, 605.f, 615.f, 15.f, 15.f, charHand3);

		CREATE_SHEET(charHand4);
		FRAME_ADD(frame, 620.f, 615.f, 15.f, 15.f, charHand4);

		CREATE_SHEET(charHand5);
		FRAME_ADD(frame, 635.f, 615.f, 20.f, 20.f, charHand5);

		CREATE_SHEET(charHand6);
		FRAME_ADD(frame, 655.f, 615.f, 15.f, 15.f, charHand6);

		CREATE_SHEET(charHand7);
		FRAME_ADD(frame, 670.f, 615.f, 15.f, 15.f, charHand7);

		canvasSize = Vector2(15.f, 15.f);
		AUTO_OFFSET_CALC(charHand0);
		AUTO_OFFSET_CALC(charHand1);
		AUTO_OFFSET_CALC(charHand2);
		AUTO_OFFSET_CALC(charHand3);
		AUTO_OFFSET_CALC(charHand4);
		canvasSize = Vector2(20.f, 20.f);
		AUTO_OFFSET_CALC(charHand5);
		canvasSize = Vector2(15.f, 15.f);
		AUTO_OFFSET_CALC(charHand6);
		AUTO_OFFSET_CALC(charHand7);


		mAnimator->Create(WIDE("charHand0"), texture, charHand0, canvasSize, false);
		mAnimator->Create(WIDE("charHand1"), texture, charHand1, canvasSize, false);
		mAnimator->Create(WIDE("charHand2"), texture, charHand2, canvasSize, false);
		mAnimator->Create(WIDE("charHand3"), texture, charHand3, canvasSize, false);
		mAnimator->Create(WIDE("charHand4"), texture, charHand4, canvasSize, false);
		mAnimator->Create(WIDE("charHand5"), texture, charHand5, Vector2(20.f, 20.f), false);
		mAnimator->Create(WIDE("charHand6"), texture, charHand6, canvasSize, false);
		mAnimator->Create(WIDE("charHand7"), texture, charHand7, canvasSize, false);
		mAnimator->Play(WIDE("charHand5"), true);
	}

	void PlayerHand::ResetMeleeWeaponInfo()
	{
		SetPosition(Vector3(mHandOwner->GetScaleX() * 0.5f, 0.f, 1.f));
		SetHandOwner(mHandOwner);
		SetRotationX(0.f);
		SetRotationY(0.f);
		SetRotationZ(0.f);
		GetHandOwnerTR()->SetRotationX(0.f);
		GetHandOwnerTR()->SetRotationY(0.f);
		GetHandOwnerTR()->SetRotationZ(0.f);

		float firstOffsetAngle = 0.f;
		Vector2 nextMinDistance;
		Vector2 nextMaxDistance;


		object::item::weapon::Weapon* weaponPtr = dynamic_cast<object::item::weapon::Weapon*>(mWeapon);
		if (weaponPtr)
		{
			firstOffsetAngle = weaponPtr->GetFirstOffsetAngle();
			nextMinDistance = weaponPtr->GetFirstMinDistance();
			nextMaxDistance = weaponPtr->GetFirstMaxDistance();

		}

		mRotatorScript->SetOffsetAngle(firstOffsetAngle);		
		mRotatorScript->SetDistanceInfo(nextMinDistance, nextMaxDistance);

		mWeaponMelee->SetPositionY(24.f);
		mWeaponMelee->SetPositionZ(0.1f);
	}

}