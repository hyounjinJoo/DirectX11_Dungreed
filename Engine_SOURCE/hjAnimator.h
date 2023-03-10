#pragma once
#include "hjComponent.h"
#include "hjAnimation.h"

namespace hj
{
    class Animator :
        public Component
    {
    public:
        struct Events
        {
            struct Event
            {
                void operator=(std::function<void()> func)
                {
                    mEvent = std::move(func);
                }
                void operator()()
                {
                    if (mEvent)
                        mEvent();
                }

                std::function<void()> mEvent;
            };

            Event mStartEvent;
            Event mCompleteEvent;
            Event mEndEvent;
        };

        Animator();
        virtual ~Animator();

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void FixedUpdate() override;
        virtual void Render() override;

        bool Create(const std::wstring& name, std::shared_ptr<Texture> atlas
            , Vector2 leftTop, Vector2 size, Vector2 offset
            , UINT spriteLength, float duration, bool reversePlay = false);

        bool Create(const std::wstring& name, std::shared_ptr<Texture> atlas
            , const std::vector<Animation::Sprite>& sprite, Vector2 canvasSize, bool reversePlay);

        Animation* FindAnimation(const std::wstring& name);
        Events* FindEvents(const std::wstring& name);
        void Play(const std::wstring& name, bool loop = true);

        void Binds();
        void Clear();

        std::optional<std::function<void()>> GetStartEvent(const std::wstring& name);
        std::optional<std::function<void()>> GetCompleteEvent(const std::wstring& name);
        std::optional<std::function<void()>> GetEndEvent(const std::wstring& name);

    private:
        std::map<std::wstring, Animation*> mAnimations;
        std::map<std::wstring, Events*> mEvents;
        Animation* mActiveAnimation;
        bool mbLoop;
    };
}