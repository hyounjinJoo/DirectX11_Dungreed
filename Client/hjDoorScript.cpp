#include "hjDoorScript.h"
#include "hjRoomDoor.h"
#include "hjPlayer.h"

namespace hj
{
	void DoorScript::OnCollisionEnter(Collider* collider)
	{
		// Player ����ó��
		Player* player = dynamic_cast<Player*>(collider->GetOwner());
		if (!player)
			return;

		RoomDoor* door = dynamic_cast<RoomDoor*>(GetOwner());
		if (!door)
			return;

		// Fade Out ����
		door->FadeOutStart(player);
	}
}