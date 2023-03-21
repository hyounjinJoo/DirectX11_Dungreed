#include "hjCameraScript.h"
#include "hjTransform.h"
#include "hjGameObject.h"
#include "hjInput.h"
#include "hjTime.h"
#include "hjSceneManager.h"
#include "hjPlayer.h"

namespace hj
{
	CameraScript::CameraScript()
		: Script()
	{
	}

	CameraScript::~CameraScript()
	{
	}

	void CameraScript::Initialize()
	{
	}

	void CameraScript::Update()
	{
		std::vector<GameObject*> objects = SceneManager::GetActiveScene()->GetGameObjects(eLayerType::Player);
		bool isPlayerExist = false;
		for (auto iter : objects)
		{
			if (dynamic_cast<Player*>(iter))
			{
				isPlayerExist = true;
				GetOwner()->SetPositionXY(iter->GetPositionXY());
			}
		}

		if (!isPlayerExist)
			GetOwner()->SetPositionXY(Vector2(0.f, 0.f));
	}

	void CameraScript::Render()
	{
	}
}