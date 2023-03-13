#pragma once
#include "hjEntity.h"
#include "hjTexture.h"
#include "hjRenderer.h"

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

            Sprite()
                : leftTop(0.f, 0.f)
                , size(0.f, 0.f)
                , offset(0.f, 0.f)
                , duration(0.1f)
            {
            }
        };

        Animation();
        virtual ~Animation();

        void Update();
        void FixedUpdate();
        void Render();

        void Create(const std::wstring& name, std::shared_ptr<Texture> atlas
            , Vector2 leftTop, Vector2 size, Vector2 offset
            , UINT columnLength, UINT spriteLength, float duration);

        void Create(const std::wstring& name, std::shared_ptr<Texture> atlas
            , const std::vector<Sprite>& sprite, Vector2 canvasSize, bool reversePlay);

		void SetShader(std::shared_ptr<Shader> shader) { mShader = shader; }
		std::shared_ptr<Shader> GetShader() { return mShader; }

        void BindShader();
        void Reset();
        void Clear();

        bool IsComplete() { return mbComplete; }
        std::wstring& AnimationName() { return mName; }
    private:
        class Animator* mAnimator;
        std::wstring mName;
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

		AnimationCB mCB;
    };
}