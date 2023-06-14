#pragma once
#include <hjGameObject.h>
#include "hjRoomDoor.h"

namespace hj
{
	class RoomBase :
		public GameObject
	{
	public:
		RoomBase();
		virtual ~RoomBase();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void AddObjectsPosXY(const Vector2& pos);
		void SettingFadeObject();
		void SettingDoorOwner();

		void ConnectDoor(DoorPlaced doorPlace, RoomDoor* Dest);
		RoomDoor* GetDoor(DoorPlaced doorPlace);
		void LimitCameraSpace();

		void Pause();
		virtual void Activate();

		UINT FindObjectIndexInManagedObjects(GameObject* object);
		bool DelistFromManagedGameObjects(GameObject* object);
		bool AddObjectToManagedGameObjects(GameObject* object);

	protected:
		std::vector<RoomDoor*> mDoors;
		std::vector<GameObject*> mGameObjects;

		Vector4 mMapLTRBlimit;
	};
}

