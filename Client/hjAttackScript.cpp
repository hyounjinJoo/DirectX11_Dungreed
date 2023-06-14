#include "hjAttackScript.h"
#include "hjPlayerScript.h"
#include "hjPlayer.h"
#include "hjActor.h"
#include "hjMonster.h"

namespace hj
{
	AttackScript::AttackScript()
		: mOwnerPlayer(nullptr)
		, mPlayerScript(nullptr)
		, mDashDamage(0.f)
		, mbDamageOn(false)
	{
	}

	AttackScript::~AttackScript()
	{
		mOwnerPlayer = nullptr;
		mPlayerScript = nullptr;
	}

	void AttackScript::Update()
	{
	}

	void AttackScript::FixedUpdate()
	{
	}
	
	void AttackScript::OnCollisionStay(Collider* collider)
	{	
		if (!mbDamageOn)
		{
			return;
		}

		Actor* monsterBody = dynamic_cast<Actor*>(collider->GetOwner());
		if (monsterBody)
		{
			Actor* monster = monsterBody->GetOwnerActor();
			if (monster && false == IsExistDamagedObjectInSet(monster))
			{
				monster->Damaged(mDashDamage);
				AddDamagedObject(monster);
			}
		}
	}

	void AttackScript::SetPlayer(Player* ownerPlayer)
	{
		if (ownerPlayer)
		{
			mOwnerPlayer = ownerPlayer;
			mDashDamage = ownerPlayer->GetDashDamage();
			if (ownerPlayer->GetPlayerScript())
			{
				mPlayerScript = ownerPlayer->GetPlayerScript();
			}
		}
	}

	void AttackScript::AddDamagedObject(Actor* damagedObject)
	{
		if (mDamagedObjects.find(damagedObject) == mDamagedObjects.end())
		{
			mDamagedObjects.insert(damagedObject);
		}
	}

	void AttackScript::DeleteDamagedObject(Actor* damagedObject)
	{
		mDamagedObjects.erase(damagedObject);
	}

	void AttackScript::ClearDamagedObjects()
	{
		mDamagedObjects.clear();
		mbDamageOn = false;
	}

	bool AttackScript::IsExistDamagedObjectInSet(Actor* damagedObject)
	{
		bool result = false;
		if (damagedObject && mDamagedObjects.find(damagedObject) != mDamagedObjects.end())
		{
			result = true;
		}

		return result;
	}


}