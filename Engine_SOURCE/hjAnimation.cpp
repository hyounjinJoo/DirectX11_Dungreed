#include "hjAnimation.h"
#include "hjTime.h"
#include "hjRenderer.h"
#include "hjTexture.h"

namespace hj
{
	Animation::Animation()
		: mAnimator(nullptr)
		, mAtlas(nullptr)
		, mShader(nullptr)
		, mSpriteSheet{}
		, mCanvasSize(Vector2::Zero)
		, mIndex(-1)
		, mTime(0.f)
		, mbComplete(false)
		, mbReversePlay(false)
		, mbUsed(false)
		, mbCanvasUsed(false)
		, mbInverse(false)
	{
	}

	Animation::Animation(const Animation& anim)
		: mAnimator(nullptr)
		, mAtlas(anim.mAtlas)
		, mShader(anim.mShader)
		, mCanvasSize(anim.mCanvasSize)
		, mIndex(-1)
		, mTime(0.f)
		, mbComplete(false)
		, mbReversePlay(anim.mbReversePlay)
		, mbUsed(anim.mbUsed)
		, mbCanvasUsed(anim.mbCanvasUsed)
		, mbInverse(false)
	{
		mAnimationName = anim.mAnimationName;
		mSpriteSheet.resize(anim.mSpriteSheet.size());
		std::copy(anim.mSpriteSheet.begin(), anim.mSpriteSheet.end(), mSpriteSheet.begin());
	}

	Animation::~Animation()
	{
	}

	UINT Animation::Update()
	{
		if (mbComplete)
			return -1;

		// 시간 체크
		mTime += Time::ActualDeltaTime();

		// 누적 시간이 해당 프레임의 유지시간을 넘어가면 다음 프레임으로 이동
		if (mSpriteSheet[mIndex].duration < mTime)
		{
			mTime = 0.f;
			
			// 역 재생여부에 따라 처리를 분리
			if (mbReversePlay)
			{
				--mIndex;

				if (0 == mIndex)
				{
					mbComplete = true;
					mIndex = 0;
				}
			}
			else
			{
				++mIndex; 
				
				if (mSpriteSheet.size() <= mIndex)
				{
					mbComplete = true;
					mIndex = static_cast<int>(mSpriteSheet.size() - 1);
				}
			}

			return mIndex;
		}

		return -1;
	}

	void Animation::FixedUpdate()
	{
	}

	void Animation::Render()
	{
	}

	void Animation::Create(const std::wstring& name, std::shared_ptr<Texture> atlas, 
		Vector2 leftTop, Vector2 size, Vector2 offset, UINT spriteLength, float duration, bool reversePlay)
	{
		mAnimationName = name;

		mAtlas = atlas;
		Vector2 atlasSize = atlas->GetTexSize();
		float width = static_cast<float>(atlasSize.x);
		float height = static_cast<float>(atlasSize.y);
		mbReversePlay = reversePlay;
		mCanvasSize = size;

		for (size_t i = 0; i < spriteLength; ++i)
		{
			Sprite sprite = {};
			sprite.leftTop = Vector2(leftTop.x + (size.x * static_cast<float>(i)), leftTop.y);
			sprite.size = size;
			sprite.offset = offset;
			sprite.duration = duration;
			sprite.atlasSize = atlasSize;

			mSpriteSheet.push_back(sprite);
		}
		mbUsed = true;
		mbCanvasUsed = true;
	}

	void Animation::Create(const std::wstring& name, std::shared_ptr<Texture> atlas,
		const std::vector<Sprite>& sprite, Vector2 canvasSize, bool reversePlay)
	{
		mAnimationName = name;
		mAtlas = atlas;
		mCanvasSize = canvasSize;
		mbReversePlay = reversePlay;

		for (const auto& iter : sprite)
		{
			mSpriteSheet.emplace_back(iter);
		}

		mbUsed = true;
		mbCanvasUsed = true;
	}

	void Animation::BindShader()
	{
		if (mAtlas)
			mAtlas->BindShaderResource(eShaderStage::PS, 12);

		ConstantBuffer* pCB = renderer::constantBuffers[(UINT)eCBType::Animation];

		renderer::AnimationCB info = {};
		info.type = static_cast<UINT>(eAnimationType::SecondDimension);
		info.leftTop = mSpriteSheet[mIndex].leftTop;
		info.offset = mSpriteSheet[mIndex].offset;
		info.size = mSpriteSheet[mIndex].size;
		info.atlasSize = mSpriteSheet[mIndex].atlasSize;
		info.inverse = mbInverse;
		info.canvasSize = mCanvasSize;
		info.canvasUsed = mbCanvasUsed;
		info.used = mbUsed;

		pCB->SetData(&info);
		pCB->Bind(eShaderStage::PS);
	}

	void Animation::Reset()
	{
		mTime = 0.f;
		mbComplete = false;
		if (mbReversePlay)
		{
			mIndex = static_cast<int>(mSpriteSheet.size() - 1);
		}
		else
		{
			mIndex = 0;
		}
	}

	void Animation::Clear()
	{
		Texture::Clear(12);

		ConstantBuffer* pCB = renderer::constantBuffers[(UINT)eCBType::Animation];
		pCB->Clear();
	}
	void Animation::SetAnimator(Animator* animator)
	{
		mAnimator = animator;
	}

	void Animation::ChangePlayDuration(float duration)
	{
		if (mSpriteSheet.empty())
			return;

		size_t sheetSize = mSpriteSheet.size();
		for (size_t iter = 0; iter < sheetSize; ++iter)
		{
			mSpriteSheet[iter].duration = duration;
		}
	}

	hj::math::Vector2 Animation::GetCurrentSpriteSize()
	{
		Vector2 spriteSize = Vector2::Zero;
		if (-1 != mIndex && mIndex < mSpriteSheet.size())
		{
			spriteSize = mSpriteSheet[mIndex].size;
		}

		return spriteSize;
	}

	hj::math::Vector2 Animation::GetCurrentSpriteOffset()
	{
		Vector2 spriteOffset = Vector2::Zero;
		if (-1 != mIndex && mIndex < mSpriteSheet.size())
		{
			spriteOffset = mSpriteSheet[mIndex].offset;
		}

		return spriteOffset;
	}

	Vector2 Animation::GetCurrentSpriteTrimSize()
	{
		Vector2 spriteTrimSize = Vector2::Zero;
		if (-1 != mIndex && mIndex < mSpriteSheet.size())
		{
			spriteTrimSize = mSpriteSheet[mIndex].trimmedSize;
		}

		return spriteTrimSize;
	}

	Vector2 Animation::GetCurrentSpriteTrimOffset()
	{
		Vector2 spriteTrimOffset = Vector2::Zero;
		if (-1 != mIndex && mIndex < mSpriteSheet.size())
		{
			spriteTrimOffset = mSpriteSheet[mIndex].trimmedOffset;
		}

		return spriteTrimOffset;
	}

	hj::math::Vector2 Animation::GetSpriteSize(UINT index)
	{
		Vector2 spriteSize = Vector2::Zero;
		if (index < mSpriteSheet.size())
		{
			spriteSize = mSpriteSheet[index].size;
		}

		return spriteSize;
	}

	Animation* Animation::Clone()
	{
		return new Animation(*this);
	}

}