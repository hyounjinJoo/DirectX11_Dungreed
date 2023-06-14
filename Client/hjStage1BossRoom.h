#pragma once
#include "hjRoomBase.h"

namespace hj
{
	class Stage1BossRoom :
		public RoomBase
	{
	public:
		Stage1BossRoom();
		virtual ~Stage1BossRoom();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		virtual void Activate() override;

		void AddObjectsPosXY(const Vector2& pos);

		void SpawnBoss();
	private:
		class Stage1Boss* mBoss;
		bool mbBossSpawned;
	};
}