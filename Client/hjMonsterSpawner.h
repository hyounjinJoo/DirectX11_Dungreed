#pragma once
#include "hjActor.h"

namespace hj
{
	class Monster;

	class MonsterSpawner :
		public Actor
	{
	public:
		MonsterSpawner();
		virtual ~MonsterSpawner();
				
		virtual void Initialize() override;

	public:
		void SetSpawnMonster(Monster* monster);

	private:
		void SpawnMonster();
		void CreateAnimation();

	private:
		class Monster* mMonster;
		class Actor* mSpawnSoundObj;
		bool mbSpawned;
	};
}
