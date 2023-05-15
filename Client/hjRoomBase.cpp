#include "hjRoomBase.h"
#include "hjScene.h"
#include "hjSceneManager.h"
#include "hjRoomDoor.h"
#include "hjFadeObject.h"
#include "hjRenderer.h"
#include "hjCameraScript.h"

namespace hj
{
	RoomBase::RoomBase()
		: mMapLTRBlimit(Vector4(-800.f, 450.f, 800.f, -450.f))
	{
		mDoors.resize(static_cast<UINT>(DoorPlaced::End));
	}

	RoomBase::~RoomBase()
	{
		size_t Size = mGameObjects.size();

		for (size_t iter = 0; iter < Size; ++iter)
		{
			mGameObjects[iter] = nullptr;
		}

		Size = mDoors.size();

		for (size_t iter = 0; iter < Size; ++iter)
		{
			mDoors[iter] = nullptr;
		}
	}

	void RoomBase::Initialize()
	{
		GameObject::Initialize();
	}
	void RoomBase::Update()
	{
		GameObject::Update();
	}
	void RoomBase::FixedUpdate()
	{
		GameObject::FixedUpdate();
	}
	void RoomBase::Render()
	{
		GameObject::Render();
	}

	void RoomBase::AddObjectsPosXY(const Vector2& pos)
	{
		size_t Size = mGameObjects.size();

		for (size_t iter = 0; iter < Size; ++iter)
		{
			mGameObjects[iter]->AddPositionXY(pos);
		}
	}

	void RoomBase::SettingFadeObject()
	{
		Scene* currentScene = SceneManager::GetActiveScene();
		std::vector<GameObject*> objects = currentScene->GetGameObjects(eLayerType::UI);
		FadeObject* fadeObject = nullptr;

		size_t Size = objects.size();
		for (size_t iter = 0; iter < Size; ++iter)
		{
			if(dynamic_cast<FadeObject*>(objects[iter]))
			{
				fadeObject = dynamic_cast<FadeObject*>(objects[iter]);
			}
		}

		if (!fadeObject)
			return;

		Size = mDoors.size();

		for (size_t iter = 0; iter < Size; ++iter)
		{
			if(mDoors[iter])
				mDoors[iter]->SetFadeScript(fadeObject->GetFadeScript());
		}
	}

	void RoomBase::SettingDoorOwner()
	{
		size_t Size = mDoors.size();

		for (size_t iter = 0; iter < Size; ++iter)
		{
			if (mDoors[iter])
				mDoors[iter]->SetOwnerRoom(this);
		}

	}

	void RoomBase::ConnectDoor(DoorPlaced doorPlace, RoomDoor* Dest)
	{
		if (doorPlace == DoorPlaced::End || !Dest)
			return;

		if (!mDoors[static_cast<UINT>(doorPlace)])
			return;

		mDoors[static_cast<UINT>(doorPlace)]->SetExitDoor(Dest);
	}

	RoomDoor* RoomBase::GetDoor(DoorPlaced doorPlace)
	{
		RoomDoor* door = nullptr;

		if (mDoors[static_cast<UINT>(doorPlace)])
			door = mDoors[static_cast<UINT>(doorPlace)];

		return door;
	}

	void RoomBase::LimitCameraSpace()
	{
		if (renderer::mainCamera)
		{
			std::vector<Script*> scripts = renderer::mainCamera->GetOwner()->GetScripts();
			for (auto script : scripts)
			{
				CameraScript* cameraScript = dynamic_cast<CameraScript*>(script);
				if (cameraScript)
				{
					cameraScript->SetLimitSpace(mMapLTRBlimit);
				}
			}
		}
	}

	void RoomBase::Pause()
	{
		GameObject::Pause();

		size_t Size = mGameObjects.size();

		for (size_t iter = 0; iter < Size; ++iter)
		{
			mGameObjects[iter]->Pause();
		}

		Size = mDoors.size();

		for (size_t iter = 0; iter < Size; ++iter)
		{
			if(mDoors[iter])
				mDoors[iter]->Pause();
		}
	}

	void RoomBase::Activate()
	{
		GameObject::Activate();

		LimitCameraSpace();

		size_t Size = mGameObjects.size();

		for (size_t iter = 0; iter < Size; ++iter)
		{
			mGameObjects[iter]->Activate();
		}

		Size = mDoors.size();

		for (size_t iter = 0; iter < Size; ++iter)
		{
			if (mDoors[iter])
				mDoors[iter]->Activate();
		}
	}

}