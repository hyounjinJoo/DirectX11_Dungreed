#pragma once
#include "hjEntity.h"
#include "hjTexture.h"
#include "hjRenderer.h"

#define CREATE_SHEET(sheetVarName) std::vector<Animation::Sprite> sheetVarName

// std::vector<Animation::Sprite> 와 Animation::Sprite 타입 변수를 선언해주고 atlasTexture의 size, frame 당 duration, frame의 offset을 설정해주는 매크로
// FRAME_ADD와 함께 사용할 것을 가정하는 매크로이다.
// sheetVarName : std::vector<Animation::Sprite> 타입으로 선언할 변수 이름
// spriteVarName : Animation::Sprite 타입으로 선언할 변수 이름
// atlasTexSize : 프레임에 사용할 아틀라스 텍스처의 크기, Vector2 타입
// frameDuration : 프레임의 재생 시간, float 타입
// frameOffset : 프레임이 가질 오프셋, Vector2 타입
#define CREATE_ANIM(sheetVarName, spriteVarName, atlasTexSize, frameDuration) CREATE_SHEET(sheetVarName);\
																				Animation::Sprite spriteVarName = {};\
                                                                                Vector2 canvasSize = {};\
																				spriteVarName.atlasSize = atlasTexSize;\
																				spriteVarName.duration = frameDuration;\
																				spriteVarName.offset = Vector2::Zero
// FRAME_ADD : std::vector sheet에 프레임을 추가해 주는 매크로
// 반드시 Animation::Sprite 변수를 생성하고 atlasSize, duration, offset을 미리 지정해 주어야 한다.
// 그 다음은 해당 매크로를 이용하여 프레임을 추가해 주면 된다.
// var : Animation::Sprite 변수
// ltX : Frame의 LeftTop.x
// ltX : Frame의 LeftTop.y
// ltX : Frame의 Size.x
// ltX : Frame의 Size.y
// ltX : Frame이 추가될 std::vector 변수
#define FRAME_ADD(var, ltX, ltY, sizeX, sizeY, sheet) \
				var.leftTop = Vector2(ltX, ltY);\
				var.size = Vector2(sizeX, sizeY);\
                canvasSize.x = canvasSize.x >= sizeX ? canvasSize.x : sizeX;\
                canvasSize.y = canvasSize.y >= sizeY ? canvasSize.y : sizeY;\
				sheet.push_back(var)

// offsetX는 0.5f를 기준으로 해당 프레임의 크기를 비율로 하여 이동시켜줄 지를 나타낸다.
// ex) 프레임이 가운데를 피봇으로 하는 경우
// offsetX가 0.6f이면 (0.6f - 0.5f) * 프레임의 x 크기가 된다.
#define FRAME_ADD_OFFSETX(var, ltX, ltY, sizeX, sizeY, offsetX, sheet) \
				var.leftTop = Vector2(ltX, ltY);\
				var.size = Vector2(sizeX, sizeY);\
                var.offset = Vector2((static_cast<float>(offsetX) - 0.5f) * sizeX, 0.f);\
                canvasSize.x = canvasSize.x >= sizeX ? canvasSize.x : sizeX;\
                canvasSize.y = canvasSize.y >= sizeY ? canvasSize.y : sizeY;\
				sheet.push_back(var)

#define FRAME_ADD_OFFSET(var, ltX, ltY, sizeX, sizeY, offsetX, offsetY, sheet) \
				var.leftTop = Vector2(ltX, ltY);\
				var.size = Vector2(sizeX, sizeY);\
                var.offset = Vector2((static_cast<float>(offsetX) - 0.5f) * sizeX, (static_cast<float>(offsetY) - 1.f) * sizeY);\
                canvasSize.x = canvasSize.x >= sizeX ? canvasSize.x : sizeX;\
                canvasSize.y = canvasSize.y >= sizeY ? canvasSize.y : sizeY;\
				sheet.push_back(var)

#define FRAME_ADD_OFFSET_TRIM_OFFSET(var, ltX, ltY, sizeX, sizeY, offsetX, offsetY, trimmedSizeX, trimmedSizeY, trimmedOffsetX, trimmedOffsetY, sheet) \
				var.leftTop = Vector2(ltX, ltY);\
				var.size = Vector2(sizeX, sizeY);\
                var.offset = Vector2((static_cast<float>(offsetX) - 0.5f) * sizeX, (static_cast<float>(offsetY) - 1.f) * sizeY);\
                var.trimmedSize = Vector2(trimmedSizeX, trimmedSizeY);\
                var.trimmedOffset = Vector2(trimmedOffsetX, trimmedOffsetY);\
                canvasSize.x = canvasSize.x >= sizeX ? canvasSize.x : sizeX;\
                canvasSize.y = canvasSize.y >= sizeY ? canvasSize.y : sizeY;\
				sheet.push_back(var)

#define AUTO_OFFSET_CALC(spriteSheetVarName) \
		for (auto& spriteFrame : spriteSheetVarName)\
		{\
			if (spriteFrame.size.x != canvasSize.x)\
			{\
				spriteFrame.offset.x = (canvasSize.x - spriteFrame.size.x) / 2.f - canvasSize.x * 0.13f;\
			}\
			else\
				spriteFrame.offset.x = 0.f;\
			if (spriteFrame.size.y != canvasSize.y)\
			{\
				spriteFrame.offset.y = -(canvasSize.y - spriteFrame.size.y) / 2.f;\
			}\
			else\
				spriteFrame.offset.y = 0.f;\
		}\

#define AUTO_OFFSET_CALC_X(spriteSheetVarName) \
		for (auto& spriteFrame : spriteSheetVarName)\
		{\
			if (spriteFrame.size.x != canvasSize.x)\
			{\
				spriteFrame.offset.x = (canvasSize.x - spriteFrame.size.x) / 2.f - canvasSize.x * 0.13f;\
			}\
			else\
				spriteFrame.offset.x = 0.f;\
		}\

#define AUTO_OFFSET_CALC_Y(spriteSheetVarName) \
		for (auto& spriteFrame : spriteSheetVarName)\
		{\
			if (spriteFrame.size.y != canvasSize.y)\
            {\
            spriteFrame.offset.y = -(canvasSize.y - spriteFrame.size.y) / 2.f; \
            }\
            else\
            spriteFrame.offset.y = 0.f; \
		}\

#define MANUAL_OFFSET_CALC_Y(spriteSheetVarName, offsetY) \
		for (auto& spriteFrame : spriteSheetVarName)\
		{\
			if (spriteFrame.size.y != canvasSize.y)\
            {\
            spriteFrame.offset.y = (static_cast<float>(offsetY) - 1.f) * -(canvasSize.y - spriteFrame.size.y) / 2.f; \
            }\
            else\
            spriteFrame.offset.y = 0.f; \
		}\

namespace hj
{
    using namespace hj::math;
	using namespace hj::graphics;
	using namespace hj::renderer;

    class Animation :
        public Entity
    {
    public:
        struct Sprite
        {
            Vector2 leftTop;    // 좌측 상단 좌표
            Vector2 size;       // 좌측 상단부터 잘라낼 가로/세로 픽셀 크기
            Vector2 offset;     // 렌더링 위치를 조정하기 위한 좌표
            float duration;     // 프레임간의 시간 간격
            Vector2 atlasSize;  // 보유한 이미지 크기

			Vector2 trimmedSize;// 이미지가 존재하는 부분만 체크한 최대 크기
			Vector2 trimmedOffset;// 이미지가 존재하는 부분만 체크한 최대 크기가 존재할 때의 offset 값
            
            Sprite()
                : leftTop(0.f, 0.f)
                , size(0.f, 0.f)
                , offset(0.f, 0.f)
                , duration(0.1f)
                , atlasSize(Vector2::One)
                , trimmedSize(Vector2::Zero)
                , trimmedOffset(Vector2::Zero)
            {
            }
        };

        Animation();
        Animation(const Animation&);
        virtual ~Animation();

        UINT Update();
        void FixedUpdate();
        void Render();

    public:
        void SetPause(bool pause) { mbPause = pause; }
        bool IsPause() { return mbPause; }

    public:
        void Create(const std::wstring& name, std::shared_ptr<Texture> atlas
            , Vector2 leftTop, Vector2 size, Vector2 offset
            , UINT spriteLength, float duration, bool reversePlay = false);

        void Create(const std::wstring& name, std::shared_ptr<Texture> atlas
            , const std::vector<Sprite>& sprite, Vector2 canvasSize, bool reversePlay);

		void SetShader(std::shared_ptr<Shader> shader) { mShader = shader; }
		std::shared_ptr<Shader> GetShader() { return mShader; }

        void Inverse(bool inverse) { mbInverse = inverse; }
        void BindShader();
        void Reset();
        void Clear();

        void UseCanvas(bool use) { mbCanvasUsed = use; }
        bool IsInverse() { return mbInverse; }
        bool IsComplete() { return mbComplete; }
        std::wstring& AnimationName() { return mAnimationName; }
        int GetCurrentFrameNumber() { return mIndex; }
        void SetAnimator(class Animator* animator);
        int GetSheetSize() { return static_cast<int>(mSpriteSheet.size()); }
        float GetDuration() 
        {
            if (mSpriteSheet.empty()) 
                return 0.f;

            return mSpriteSheet[0].duration;
        }

        void ChangePlayDuration(float duration);
        const Vector2& GetCanvasSize() { return mCanvasSize; }
		float GetCanvasSizeX() { return mCanvasSize.x; }
        float GetCanvasSizeY() { return mCanvasSize.y; }
        Vector2 GetCurrentSpriteSize();
        Vector2 GetCurrentSpriteOffset();
        Vector2 GetCurrentSpriteTrimSize();
        Vector2 GetCurrentSpriteTrimOffset();
		Vector2 GetSpriteSize(UINT index);
		Vector2 GetSpriteTrimSize(UINT index);

        Animation* Clone();

	private:
        class Animator* mAnimator;
        std::wstring mAnimationName;
		std::shared_ptr<Texture> mAtlas;
		std::shared_ptr<Shader> mShader;
        std::vector<Sprite> mSpriteSheet;
        Vector2 mCanvasSize;
        int mIndex;
        float mTime;
        bool mbComplete;
        bool mbReversePlay;
        bool mbUsed;
        bool mbCanvasUsed;
        bool mbInverse;
        bool mbPause;
    };
}