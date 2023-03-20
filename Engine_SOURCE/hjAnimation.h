#pragma once
#include "hjEntity.h"
#include "hjTexture.h"
#include "hjRenderer.h"

// std::vector<Animation::Sprite> 와 Animation::Sprite 타입 변수를 선언해주고 atlasTexture의 size, frame 당 duration, frame의 offset을 설정해주는 매크로
// FRAME_ADD와 함께 사용할 것을 가정하는 매크로이다.
// sheetVarName : std::vector<Animation::Sprite> 타입으로 선언할 변수 이름
// spriteVarName : Animation::Sprite 타입으로 선언할 변수 이름
// atlasTexSize : 프레임에 사용할 아틀라스 텍스처의 크기, Vector2 타입
// frameDuration : 프레임의 재생 시간, float 타입
// frameOffset : 프레임이 가질 오프셋, Vector2 타입
#define CREATE_ANIM(sheetVarName, spriteVarName, atlasTexSize, frameDuration, frameOffset) std::vector<Animation::Sprite> sheetVarName;\
																				Animation::Sprite spriteVarName = {};\
																				spriteVarName.atlasSize = atlasTexSize;\
																				spriteVarName.duration = frameDuration;\
																				spriteVarName.offset = frameOffset
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
				sheet.push_back(var)
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
            
            Sprite()
                : leftTop(0.f, 0.f)
                , size(0.f, 0.f)
                , offset(0.f, 0.f)
                , duration(0.1f)
                , atlasSize(Vector2::One)
            {
            }
        };

        Animation();
        virtual ~Animation();

        UINT Update();
        void FixedUpdate();
        void Render();

        void Create(const std::wstring& name, std::shared_ptr<Texture> atlas
            , Vector2 leftTop, Vector2 size, Vector2 offset
            , UINT spriteLength, float duration, bool reversePlay = false);

        void Create(const std::wstring& name, std::shared_ptr<Texture> atlas
            , const std::vector<Sprite>& sprite, Vector2 canvasSize, bool reversePlay);

		void SetShader(std::shared_ptr<Shader> shader) { mShader = shader; }
		std::shared_ptr<Shader> GetShader() { return mShader; }

        void BindShader();
        void Reset();
        void Clear();

        bool IsComplete() { return mbComplete; }
        std::wstring& AnimationName() { return mAnimationName; }

        void SetAnimator(class Animator* animator);

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
    };
}