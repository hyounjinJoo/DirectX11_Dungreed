#pragma once
#include <hjGameObject.h>

namespace hj
{
	enum class DoorPlaced
	{
		L,
		T,
		R,
		B,
		None,
	};

	class RoomDoor :
		public GameObject
	{
	public:
		RoomDoor();
		virtual ~RoomDoor();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		void SetDoorPlaced(UINT place) { SetDoorPlaced(static_cast<DoorPlaced>(place)); }
		void SetDoorPlaced(DoorPlaced place);
		DoorPlaced GetDoorPlacedType() { return mDoorPlaced; }

		void SetExitDoor(RoomDoor* exitDoor) { mExitDoor = exitDoor; }
		Vector2 GetExitPos() { return mExitPos; }
		
		void SetFadeScript(class FadeScript* fadeScript);

		void FadeOutStart(class Player* player);
		void FadeInStart();

		void CheckFadeOutComplete();
		void CheckFadeInComplete();

		void ProcessFadeInEnd();

	private:
		DoorPlaced mDoorPlaced;
		class Collider2D* mDoorCollider;
		class FadeScript* mFadeScript;
		RoomDoor* mExitDoor;
		Vector2 mExitPos;
		bool mbEntryStart;
		bool mbExitStart;

		bool mbFadeOutComplete;
		bool mbFadeInComplete;
	};
}