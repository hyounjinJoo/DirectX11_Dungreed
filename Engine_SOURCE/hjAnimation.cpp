#include "hjAnimation.h"
#include "hjTime.h"
#include "hjRenderer.h"

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
	{
	}

	Animation::~Animation()
	{
	}

	void Animation::Update()
	{
		if (mbComplete)
			return;

		// 시간 체크
		mTime += Time::DeltaTime();

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
		}
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
		mbCanvasUsed = false;
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
			mAtlas->BindShader(eShaderStage::PS, 12);

		ConstantBuffer* pCB = renderer::constantBuffers[(UINT)eCBType::Animation];

		renderer::AnimationCB info = {};
		info.type = static_cast<UINT>(eAnimationType::SecondDimension);
		info.leftTop = mSpriteSheet[mIndex].leftTop;
		info.offset = mSpriteSheet[mIndex].offset;
		info.size = mSpriteSheet[mIndex].size;
		info.atlasSize = mSpriteSheet[mIndex].atlasSize;
		info.inverse = mbInverse;
		info.canvasSize = mCanvasSize;
		info.canvasUsed = true;
		info.used = mbUsed;

		pCB->Bind(&info);
		pCB->SetPipeline(eShaderStage::PS);
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
	}
}