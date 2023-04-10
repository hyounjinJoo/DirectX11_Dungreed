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

		// �ð� üũ
		mTime += Time::DeltaTime();

		// ���� �ð��� �ش� �������� �����ð��� �Ѿ�� ���� ���������� �̵�
		if (mSpriteSheet[mIndex].duration < mTime)
		{
			mTime = 0.f;
			
			// �� ������ο� ���� ó���� �и�
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

	Animation* Animation::Clone()
	{
		return new Animation(*this);
	}

}