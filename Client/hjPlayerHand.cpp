#include "hjPlayerHand.h"
#include "hjPlayer.h"
#include "hjCollider2D.h"
#include "hjObject.h"
#include "hjSpriteRenderer.h"
#include "hjResources.h"
#include "hjItem.h"

namespace hj
{
	PlayerHand::PlayerHand()
		: GameObject()
		, mHandOwner(nullptr)
		, mHandOwnerTR(nullptr)
		, mHandTransform(nullptr)
		, mHandState(handState::Normal)
	{
		AddComponent<Collider2D>();
		SetScaleXY(Vector2(12.f, 12.f));

		SpriteRenderer* sr = AddComponent<SpriteRenderer>();
		std::shared_ptr<Material> material = MTRL_FIND("MTRL_Char_Adventurer");
		std::shared_ptr<Mesh> mesh = MESH_FIND("Mesh_Rect");
		sr->SetMaterial(material);
		sr->SetMesh(mesh);

		mAnimator = AddComponent<Animator>();
		if (material && mAnimator)
		{
			std::shared_ptr<Texture> texture = material->GetTexture();
			if (texture)
			{
				CreateAnimation();
			}
		}

		GameObject* testWeapon = object::Instantiate<GameObject>(eLayerType::Player);
		testWeapon->SetName(L"Test Monster for Collision");

		sr = testWeapon->AddComponent<SpriteRenderer>();
		material = MTRL_FIND("MTRL_Weapon_Legendary_DemonSword");
		sr->SetMaterial(material);
		sr->SetMesh(mesh);

		Vector2 texSize = material->GetTexture()->GetTexSize();
		texSize *= 4.f;
		testWeapon->GetComponent<Transform>()->SetScale(Vector3(texSize.x, texSize.y, 1.f));
		testWeapon->GetTransform()->AddPositionX(9.f);
		testWeapon->GetTransform()->SetPositionY(texSize.y * 0.4f);
		testWeapon->GetTransform()->SetParent(GetTransform());
		//testMonster->GetTransform()->SetInheritParentPosition(true);
		mWeapon = testWeapon;

		Collider2D* collider = testWeapon->AddComponent<Collider2D>();
		collider->SetType(eColliderType::Circle);
		collider->SetSize(texSize);
	}

	PlayerHand::~PlayerHand()
	{
	}
	
	void PlayerHand::Initialize()
	{
		GameObject::Initialize();
	}
	
	void PlayerHand::Update()
	{
		GameObject::Update();
	}
	
	void PlayerHand::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}
	
	void PlayerHand::Render()
	{
		GameObject::Render();
	}

	void PlayerHand::SetHandOwner(Player* owner)
	{
		Player* player = owner;
		if (!player)
			return;

		mHandOwner = player;
		mHandOwnerTR = player->GetTransform();
	}

	void PlayerHand::InverseHandPosZ(bool inverse)
	{
		float pos = GetPositionZ();

		if (inverse) pos *= -1.f;
		else pos *= -1.f;

		SetPositionZ(pos);
	}

	void PlayerHand::CreateAnimation()
	{
		std::shared_ptr<Material> material = MTRL_FIND("MTRL_Char_Adventurer");
		std::shared_ptr<Texture> texture = material->GetTexture();
		Vector2 atlasTexSize = texture->GetTexSize();

		float duration = 0.f;
		duration = 1.f;
		CREATE_ANIM(charHand0, frame, atlasTexSize, duration);
		FRAME_ADD(frame, 320.f, 412.f, 12.f, 12.f, charHand0);

		CREATE_SHEET(charHand1);
		FRAME_ADD(frame, 332.f, 412.f, 12.f, 12.f, charHand1);

		CREATE_SHEET(charHand2);
		FRAME_ADD(frame, 344.f, 412.f, 12.f, 12.f, charHand2);

		CREATE_SHEET(charHand3);
		FRAME_ADD(frame, 356.f, 412.f, 12.f, 12.f, charHand3);

		CREATE_SHEET(charHand4);
		FRAME_ADD(frame, 368.f, 412.f, 12.f, 12.f, charHand4);

		CREATE_SHEET(charHand5);
		FRAME_ADD(frame, 380.f, 412.f, 16.f, 16.f, charHand5);

		CREATE_SHEET(charHand6);
		FRAME_ADD(frame, 396.f, 412.f, 12.f, 12.f, charHand6);

		CREATE_SHEET(charHand7);
		FRAME_ADD(frame, 408.f, 412.f, 12.f, 12.f, charHand7);

		canvasSize = Vector2(12.f, 12.f);
		AUTO_OFFSET_CALC(charHand0);
		AUTO_OFFSET_CALC(charHand1);
		AUTO_OFFSET_CALC(charHand2);
		AUTO_OFFSET_CALC(charHand3);
		AUTO_OFFSET_CALC(charHand4);
		canvasSize = Vector2(16.f, 16.f);
		AUTO_OFFSET_CALC(charHand5);
		canvasSize = Vector2(12.f, 12.f);
		AUTO_OFFSET_CALC(charHand6);
		AUTO_OFFSET_CALC(charHand7);


		mAnimator->Create(WIDE("charHand0"), texture, charHand0, canvasSize, false);
		mAnimator->Create(WIDE("charHand1"), texture, charHand1, canvasSize, false);
		mAnimator->Create(WIDE("charHand2"), texture, charHand2, canvasSize, false);
		mAnimator->Create(WIDE("charHand3"), texture, charHand3, canvasSize, false);
		mAnimator->Create(WIDE("charHand4"), texture, charHand4, canvasSize, false);
		mAnimator->Create(WIDE("charHand5"), texture, charHand5, Vector2(16.f, 16.f), false);
		mAnimator->Create(WIDE("charHand6"), texture, charHand6, canvasSize, false);
		mAnimator->Create(WIDE("charHand7"), texture, charHand7, canvasSize, false);
		mAnimator->Play(WIDE("charHand5"), true);
	}

}