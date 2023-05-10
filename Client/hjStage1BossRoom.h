#pragma once
#include <hjGameObject.h>
#include "hjRoomDoor.h"

namespace hj
{
	class Stage1BossRoom :
		public GameObject
	{
	public:
		Stage1BossRoom();
		virtual ~Stage1BossRoom();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
		std::vector<RoomDoor> mDoors;
	};
}