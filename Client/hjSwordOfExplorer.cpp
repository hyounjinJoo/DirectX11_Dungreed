#include "hjSwordOfExplorer.h"

namespace hj::object::item::weapon
{
	SwordOfExplorer::SwordOfExplorer()
		: Weapon(eWeaponType::Melee)
	{
		SetName(WIDE("WEAPON_MEELEE_01_모험가의 검"));

		mItemName = std::string("모험가의 검");
		mItemDescription = std::string("잘 손질된 모험가의 검");
		mWeaponInfo.mWeaponID = 0;
		mWeaponInfo.mMinAttack = 10;
		mWeaponInfo.mMaxAttack = 11;
		mWeaponInfo.mAttackPerSec = 3.3f;

	}
	SwordOfExplorer::~SwordOfExplorer()
	{
	}
	void SwordOfExplorer::Initialize()
	{
		//sr = testWeapon->AddComponent<SpriteRenderer>();
		//material = MTRL_FIND("MTRL_Weapon_Legendary_DemonSword");
		//sr->SetMaterial(material);
		//sr->SetMesh(mesh);
		//
		//Vector2 texSize = material->GetTexture()->GetTexSize();
		//texSize *= 4.f;
		//testWeapon->GetComponent<Transform>()->SetScale(Vector3(texSize.x, texSize.y, 1.f));
		//testWeapon->GetTransform()->AddPositionX(9.f);
		//testWeapon->GetTransform()->SetPositionY(texSize.y * 0.4f);
		//testWeapon->GetTransform()->SetParent(GetTransform());
		////testMonster->GetTransform()->SetInheritParentPosition(true);
		//Collider2D* collider = testWeapon->AddComponent<Collider2D>();
		//collider->SetType(eColliderType::Circle);
		//collider->SetSize(texSize);
	}
	void SwordOfExplorer::Update()
	{
	}
	void SwordOfExplorer::FixedUpdate()
	{
	}
	void SwordOfExplorer::Render()
	{
	}
}