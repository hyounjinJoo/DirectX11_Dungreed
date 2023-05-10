#include "hjDoorScript.h"
#include "hjRoomDoor.h"
#include "hjPlayer.h"

namespace hj
{
	void DoorScript::OnCollisionEnter(Collider* collider)
	{
		// Player 예외처리
		Player* player = dynamic_cast<Player*>(collider->GetOwner());
		if (!player)
			return;

		RoomDoor* door = dynamic_cast<RoomDoor*>(GetOwner());
		if (!door)
			return;

		// Fade Out 시작
		door->FadeOutStart(player);
	}
}