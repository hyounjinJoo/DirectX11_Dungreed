#pragma once
#include <hjGameObject.h>
#include "hjGraphics.h"
#include "hjAnimation.h"

namespace hj
{
	class RoomBase;
	class Animation;
	struct Animation::Sprite;
	struct SpritesInfo
	{
		std::wstring spritesName;
		std::vector<Animation::Sprite> spriteSheet;
		bool bIsReversePlay;
	};
	enum class eFrameAddMethod
	{
		FRAME_ADD = 0,
		FRAME_ADD_OFFSETX,
		FRAME_ADD_OFFSET,
		FRAME_ADD_OFFSET_TRIM_OFFSET,
		End,
	};

	class Actor :
		public GameObject
	{
	public:
		Actor();
		Actor(const Actor& actor);
		virtual ~Actor();

		virtual GameObject* Clone();

		virtual void Death() override;
	public:

		virtual void SetOwnerRoom(RoomBase* room);
		RoomBase* GetOwnerRoom();

		virtual void PauseAnimation();

		Actor* GetOwnerActor();
		virtual void SetOwnerActor(Actor* owner) { mOwnerActor = owner; }

	public:
		void SetNameAndCreateSpriteRenderer(const std::wstring& name, const std::wstring& mtrlName, const std::wstring& meshName);
		std::shared_ptr<Texture> CheckHasMaterialAndTexture(const graphics::eTextureSlot& slot);
		virtual void LoadAnimInfoFromFile(const eFrameAddMethod& addMethod, const eTextureSlot& slot, const std::wstring& metaDataFilePath,
										const std::wstring& wstrForCreateAnim, const std::wstring& createdAnimNameWstr,
										float frameDuration, bool bUseCount = false, int startCount = 0, bool bReversePlay = false);

		virtual void InsertAnimationToAnimator(const eTextureSlot& slot, bool bScaleSetCanvasSize = true);
		void CalcOffsetAuto(const std::wstring& spriteSheetName);
		void CalcOffsetAutoY(const std::wstring& spriteSheetName);
		void CalcOffsetAutoX(const std::wstring& spriteSheetName);
		void CalcOffsetManualX(const std::wstring& spriteSheetName, float offsetX);
		void CalcOffsetManualY(const std::wstring& spriteSheetName, float offsetY);
		void SetOffsetManual(const std::wstring& spriteSheetName, int index, const Vector2& offset);

		virtual void Damaged(int damage) {};

	protected:
		virtual void ProcessDamaged(int damage) {};

	private:
		void SafeDeleteSprites();

	protected:
		math::Vector2 mCanvasSize;
		std::vector<SpritesInfo> mSprites;

	protected:
		RoomBase* mOwnerRoom;
		Actor* mOwnerActor;
	};
}