#include "hjAnimator.h"

namespace hj
{
	Animator::Animator()
		: Component(eComponentType::Animator)
		, mAnimations{}
		, mEvents{}
		, mActiveAnimation(nullptr)
		, mbLoop(false)
	{
	}

	Animator::~Animator()
	{
		for (auto anim : mAnimations)
		{
			delete anim.second;
			anim.second = nullptr;
		}

		for (auto event : mEvents)
		{
			delete event.second;
			event.second = nullptr;
		}
	}

	void Animator::Initialize()
	{
	}

	void Animator::Update()
	{
		if (mActiveAnimation == nullptr)
			return;

		Events* events
			= FindEvents(mActiveAnimation->AnimationName());

		if (mActiveAnimation->IsComplete())
		{
			Events* event
				= FindEvents(mActiveAnimation->AnimationName());

			if (event)
			{
				event->mCompleteEvent();
			}

			if (mbLoop)
			{
				mActiveAnimation->Reset();
			}
		}

		UINT spriteIndex = mActiveAnimation->Update();
		if (spriteIndex != -1 && events->mEvents[spriteIndex].mEvent)
		{
			events->mEvents[spriteIndex].mEvent();
		}
	}

	void Animator::FixedUpdate()
	{
		if (mActiveAnimation == nullptr)
			return;

		mActiveAnimation->FixedUpdate();
	}

	void Animator::Render()
	{
		if (mActiveAnimation == nullptr)
			return;

		mActiveAnimation->Render();
	}

	bool Animator::Create(const std::wstring& name, std::shared_ptr<Texture> atlas, Vector2 leftTop, 
		Vector2 size, Vector2 offset, UINT spriteLength, float duration, bool reversePlay)
	{
		if (atlas == nullptr)
			return false;

		Animation* animation = FindAnimation(name);
		if (animation != nullptr)
			return false;

		animation = new Animation();
		animation->Create(name, atlas, leftTop
			, size, offset, spriteLength, duration, reversePlay);

		mAnimations.insert(std::make_pair(name, animation));

		Events* events = new Events();
		events->mEvents.resize(spriteLength);
		mEvents.insert(std::make_pair(name, events));

		return true;
	}

	bool Animator::Create(const std::wstring& name, std::shared_ptr<Texture> atlas, 
		const std::vector<Animation::Sprite>& sprite, Vector2 canvasSize, bool reversePlay)
	{
		Animation* animation = FindAnimation(name);
		if (animation != nullptr)
			return false;

		animation = new Animation();
		animation->Create(name, atlas, sprite, canvasSize, reversePlay);

		mAnimations.insert(std::make_pair(name, animation));

		Events* events = new Events();
		events->mEvents.resize(sprite.size());
		mEvents.insert(std::make_pair(name, events));

		return true;
	}

	Animation* Animator::FindAnimation(const std::wstring& name)
	{
		std::map<std::wstring, Animation*>::iterator iter
			= mAnimations.find(name);

		if (iter == mAnimations.end())
		{
			return nullptr;
		}

		return iter->second;
	}

	Animator::Events* Animator::FindEvents(const std::wstring& name)
	{
		std::map<std::wstring, Events*>::iterator iter
			= mEvents.find(name);

		if (iter == mEvents.end())
		{
			return nullptr;
		}

		return iter->second;
	}

	void Animator::Play(const std::wstring& name, bool loop)
	{
		Animation* prevAnimation = mActiveAnimation;
		Events* events = nullptr;
		
		if(prevAnimation)
			events = FindEvents(prevAnimation->AnimationName());

		if (events)
			events->mEndEvent();

		mActiveAnimation = FindAnimation(name);
		mActiveAnimation->Reset();
		mbLoop = loop;

		events = FindEvents(mActiveAnimation->AnimationName());

		if (events)
			events->mStartEvent();
	}

	void Animator::Binds()
	{
		if (mActiveAnimation == nullptr)
			return;

		mActiveAnimation->BindShader();
	}

	void Animator::Clear()
	{
		if (mActiveAnimation == nullptr)
			return;

		mActiveAnimation->Clear();
	}

	std::optional<std::function<void()>> Animator::GetStartEvent(const std::wstring& name)
	{
		Events* events = FindEvents(name);

		if (events)
			return events->mStartEvent.mEvent;

		return std::nullopt;
	}

	std::optional<std::function<void()>> Animator::GetCompleteEvent(const std::wstring& name)
	{
		Events* events = FindEvents(name);

		if (events)
			return events->mCompleteEvent.mEvent;

		return std::nullopt;
	}

	std::optional<std::function<void()>> Animator::GetEndEvent(const std::wstring& name)
	{
		Events* events = FindEvents(name);

		if (events)
			return events->mEndEvent.mEvent;

		return std::nullopt;
	}

	std::optional<std::function<void()>> Animator::GetEvent(const std::wstring& name, UINT index)
	{
		Events* events = FindEvents(name);

		if (events)
			return events->mEvents[index].mEvent;

		return std::nullopt;
	}
}