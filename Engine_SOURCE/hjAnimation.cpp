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
		if (mIndex < mSpriteSheet.size())
		{
			Sprite test = mSpriteSheet[mIndex];
			
			mCB.leftTop = test.leftTop;
			mCB.size = test.size;
			mCB.offset = test.offset;
			mCB.atlasSize = mAtlas->GetTexSize();
			mCB.canvasSize = mCanvasSize;
			mCB.used = mbUsed;
			mCB.canvasUsed = mbCanvasUsed;
		}
	}

	void Animation::Render()
	{
	}

	void Animation::Create(const std::wstring& name, std::shared_ptr<Texture> atlas, Vector2 leftTop, Vector2 size, Vector2 offset, UINT columnLength, UINT spriteLength, float duration)
	{

		mbUsed = true;
		mbCanvasUsed = false;
	}

	void Animation::Create(const std::wstring& name, std::shared_ptr<Texture> atlas, const std::vector<Sprite>& sprite, Vector2 canvasSize, bool reversePlay)
	{
		mName = name;
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
			mAtlas->BindShader(eShaderStage::PS, 0);

		ConstantBuffer* pCB = renderer::constantBuffers[(UINT)eCBType::Animation];
		pCB->Bind(&mCB);
		pCB->SetPipeline(eShaderStage::VS);
		pCB->SetPipeline(eShaderStage::PS);

		mShader->Binds();
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