#include "hjBoss1BulletColliderScript.h"
#include "hjCollider2D.h"
#include "hjBoss1Bullet.h"
#include "hjPlayer.h"

namespace hj
{
	Boss1BulletColliderScript::Boss1BulletColliderScript()
		: mCircleCollider(nullptr)
	{
	}

	Boss1BulletColliderScript::Boss1BulletColliderScript(const Boss1BulletColliderScript& script)
		: Script(script)
		, mCircleCollider(nullptr)
	{
	}

	Boss1BulletColliderScript::~Boss1BulletColliderScript()
	{
		mCircleCollider = nullptr;
	}

	void Boss1BulletColliderScript::OnCollisionEnter(Collider* collider)
	{
		if (nullptr == mCircleCollider || nullptr == collider->GetOwner())
			return;

		Player* player = dynamic_cast<Player*>(collider->GetOwner());
		if (nullptr == player)
			return;

		float test = 10.f;
		player->Damaged(test);

		Animator* animator = mCircleCollider->GetOwner()->GetComponent<Animator>();
		if (animator)
		{
			std::wstring bulletFXAnimWstr = WIDE("Effect_Bellial_Bullet_FX");
			animator->Play(bulletFXAnimWstr, false);
		}

	}

	void Boss1BulletColliderScript::SetCollider(class Collider2D* collider)
	{
		if (nullptr == collider)
			return;

		mCircleCollider = collider;
	}

	hj::Component* Boss1BulletColliderScript::Clone() const
	{
		return new Boss1BulletColliderScript(*this);
	}

}