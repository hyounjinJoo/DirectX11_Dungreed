#pragma once
#include "hjEntity.h"
#include "hjTexture.h"
#include "hjRenderer.h"

#define CREATE_SHEET(sheetVarName) std::vector<Animation::Sprite> sheetVarName

// std::vector<Animation::Sprite> �� Animation::Sprite Ÿ�� ������ �������ְ� atlasTexture�� size, frame �� duration, frame�� offset�� �������ִ� ��ũ��
// FRAME_ADD�� �Բ� ����� ���� �����ϴ� ��ũ���̴�.
// sheetVarName : std::vector<Animation::Sprite> Ÿ������ ������ ���� �̸�
// spriteVarName : Animation::Sprite Ÿ������ ������ ���� �̸�
// atlasTexSize : �����ӿ� ����� ��Ʋ�� �ؽ�ó�� ũ��, Vector2 Ÿ��
// frameDuration : �������� ��� �ð�, float Ÿ��
// frameOffset : �������� ���� ������, Vector2 Ÿ��
#define CREATE_ANIM(sheetVarName, spriteVarName, atlasTexSize, frameDuration) CREATE_SHEET(sheetVarName);\
																				Animation::Sprite spriteVarName = {};\
                                                                                Vector2 canvasSize = {};\
																				spriteVarName.atlasSize = atlasTexSize;\
																				spriteVarName.duration = frameDuration;\
																				spriteVarName.offset = Vector2::Zero
// FRAME_ADD : std::vector sheet�� �������� �߰��� �ִ� ��ũ��
// �ݵ�� Animation::Sprite ������ �����ϰ� atlasSize, duration, offset�� �̸� ������ �־�� �Ѵ�.
// �� ������ �ش� ��ũ�θ� �̿��Ͽ� �������� �߰��� �ָ� �ȴ�.
// var : Animation::Sprite ����
// ltX : Frame�� LeftTop.x
// ltX : Frame�� LeftTop.y
// ltX : Frame�� Size.x
// ltX : Frame�� Size.y
// ltX : Frame�� �߰��� std::vector ����
#define FRAME_ADD(var, ltX, ltY, sizeX, sizeY, sheet) \
				var.leftTop = Vector2(ltX, ltY);\
				var.size = Vector2(sizeX, sizeY);\
                canvasSize.x = canvasSize.x >= sizeX ? canvasSize.x : sizeX;\
                canvasSize.y = canvasSize.y >= sizeY ? canvasSize.y : sizeY;\
				sheet.push_back(var)

// offsetX�� 0.5f�� �������� �ش� �������� ũ�⸦ ������ �Ͽ� �̵������� ���� ��Ÿ����.
// ex) �������� ����� �Ǻ����� �ϴ� ���
// offsetX�� 0.6f�̸� (0.6f - 0.5f) * �������� x ũ�Ⱑ �ȴ�.
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
            Vector2 leftTop;    // ���� ��� ��ǥ
            Vector2 size;       // ���� ��ܺ��� �߶� ����/���� �ȼ� ũ��
            Vector2 offset;     // ������ ��ġ�� �����ϱ� ���� ��ǥ
            float duration;     // �����Ӱ��� �ð� ����
            Vector2 atlasSize;  // ������ �̹��� ũ��

			Vector2 trimmedSize;// �̹����� �����ϴ� �κи� üũ�� �ִ� ũ��
			Vector2 trimmedOffset;// �̹����� �����ϴ� �κи� üũ�� �ִ� ũ�Ⱑ ������ ���� offset ��
            
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