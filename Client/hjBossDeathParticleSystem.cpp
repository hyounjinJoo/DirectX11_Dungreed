#include "hjBossDeathParticleSystem.h"
#include "hjResources.h"
#include "hjXmlParser.h"
#include "hjAnimation.h"
#include "hjTime.h"
#include "hjTransform.h"
#include "hjGameObject.h"
#include <random>
#include "hjCameraScript.h"
#include "hjStage1Boss.h"
#include "hjAnimator.h"

namespace hj
{

#define NUM_DEATHPARTICLE 96
	BossDeathParticleSystem::BossDeathParticleSystem()
		: ParticleSystem()
		, mAnimFrameDataBuffer(nullptr)
		, mAnimSpriteInfo(nullptr)
		, mParticleState(BossDeathParticleState::Start)
	{
		mMaxParticles = NUM_DEATHPARTICLE;
		mFrequency = 0.5f;
		mRadius = 400.f;
		mbActive = false;
	}

	BossDeathParticleSystem::BossDeathParticleSystem(const BossDeathParticleSystem& comp)
		: ParticleSystem(comp)
		, mAnimFrameDataBuffer(nullptr)
		, mAnimSpriteInfo(nullptr)
	{
		mMaxParticles = comp.mMaxParticles;
		mFrequency = comp.mFrequency;
		mRadius = comp.mRadius;
		mbActive = false;
	}

	BossDeathParticleSystem::~BossDeathParticleSystem()
	{
		delete[] mAnimSpriteInfo;

		delete mAnimFrameDataBuffer;
		mAnimFrameDataBuffer = nullptr;
	}

	void BossDeathParticleSystem::Initialize()
	{
		mAnimCS = Resources::Find<ParticleAnimShader>(L"Shader_ParticleAnimCS");

		std::shared_ptr<Mesh> point = MESH_FIND_STR("Mesh_Point");
		SetMesh(point);

		// Material 세팅
		std::shared_ptr<Material> material = MTRL_FIND_STR("MTRL_ParticleAnim");
		SetMaterial(material);

		LoadAnimationData();

		AnimationParticle particles[NUM_DEATHPARTICLE] = {};
		
		mParticleBuffer = new StructuredBuffer();
		size_t Size = sizeof(AnimationParticle);
		mParticleBuffer->Create(sizeof(AnimationParticle), mMaxParticles, eSRVType::UAV, particles, true);
		
		mParticleSharedBuffer = new StructuredBuffer();
		mParticleSharedBuffer->Create(sizeof(ParticleShared), 1, eSRVType::UAV, nullptr, true);

		Camera* camera = renderer::mainCamera;
		GameObject* cameraObj = camera->GetOwner();
		std::vector<Script*> scripts = cameraObj->GetScripts();

		for (auto iter : scripts)
		{
			if (dynamic_cast<CameraScript*>(iter))
			{
				mCameraScript = static_cast<CameraScript*>(iter);
			}
		}
	}

	void BossDeathParticleSystem::FixedUpdate()
	{
		if (!mbActive)
		{
			return;
		}

		if (BossDeathParticleState::Start == mParticleState)
		{
			ChangeState(BossDeathParticleState::FirstSingleExplosion);
			return;
		}

		//파티클 생성 간격
		float aliveTime = 1.f / mFrequency;

		//누적시간
		mTime += Time::ActualDeltaTime();
		mStatePlayTimer += Time::ActualDeltaTime();

		if (aliveTime < mTime)
		{
			switch (mParticleState)
			{
			case hj::BossDeathParticleState::FirstSingleExplosion:
				if (mStatePlayTimer > mStatePlayTime)
				{
					mStatePlayTimer = 0.f;
					ChangeState(BossDeathParticleState::SecondMultipleExplosion);
				}
				break;
			case hj::BossDeathParticleState::SecondMultipleExplosion:
				if (mStatePlayTimer > mStatePlayTime)
				{
					mStatePlayTimer = 0.f;
					ChangeState(BossDeathParticleState::FinalConcentricCircle);
				}
				else
				{					
					float f = (mTime / aliveTime);
					UINT iAliveCount = (UINT)f;
					mTime = f - std::floor(f);
				}
				break;
			case hj::BossDeathParticleState::FinalConcentricCircle:
				if (mStatePlayTimer > mStatePlayTime)
				{
					mStatePlayTimer = 0.f;
					ChangeState(BossDeathParticleState::End);
				}
				else
				{
					if (mCameraScript)
					{
						mCameraScript->Shake();
					}
				}
				break;
			case hj::BossDeathParticleState::End:
				break;
			default:
				break;
			}
		}

		//mMaxParticles = mBuffer->GetStride();
		Vector3 pos = dynamic_cast<Stage1Boss*>(GetOwner())->GetBodyPos();
		mCBData.worldPosition = Vector4(pos.x, pos.y, pos.z - 10.f, 1.0f);
		mCBData.maxParticles = mMaxParticles;
		mCBData.radius = mRadius;
		mCBData.simulationSpace = (UINT)mSimulationSpace;
		mCBData.startSpeed = mStartSpeed;
		mCBData.startSize = mStartSize;
		mCBData.startColor = mStartColor;
		mCBData.startLifeTime = mStartLifeTime;
		mCBData.deltaTime = Time::ActualDeltaTime();
		mCBData.elapsedTime += Time::ActualDeltaTime();
				
		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::ParticleSystem];
		cb->SetData(&mCBData);
		cb->Bind(eShaderStage::ALL);

		mAnimCS->SetStrcutedBuffer(mParticleBuffer);
		mAnimCS->SetSharedStructuredBuffer(mParticleSharedBuffer);
		mAnimCS->SetAnimStructuredBuffer(mAnimFrameDataBuffer);
		mAnimCS->OnExecute();
	}
	void BossDeathParticleSystem::Render()
	{
		if (!mbActive)
		{
			return;
		}

		GetOwner()->GetComponent<Transform>()->SetConstantBuffer();
		mParticleBuffer->BindSRV(eShaderStage::GS, 16);
		mParticleBuffer->BindSRV(eShaderStage::PS, 16);

		GetMaterial()->Bind();

		GetMesh()->RenderInstanced(mMaxParticles);

		mParticleBuffer->Clear();
	}

	hj::Component* BossDeathParticleSystem::Clone() const
	{
		return new BossDeathParticleSystem(*this);
	}

	void BossDeathParticleSystem::LoadAnimationData()
	{
		std::shared_ptr<Texture> texture = GetMaterial()->GetTexture(eTextureSlot::T0);
		Vector2 atlasTexSize = texture->GetTexSize();

		XmlParser* parser = new XmlParser;
		bool parseResult = parser->LoadFile("08_FX/08_FX.xml");

		if (!parseResult)
		{
			delete parser;
			return;
		}

		parseResult = parser->FindElem(WIDE("TextureAtlas"));
		parseResult = parser->IntoElem();

		std::wstring targetSpriteNameWstr;
		int count = 0;

		Vector2 leftTop = Vector2::Zero;
		Vector2 size = Vector2::One;
		Vector2 offset = Vector2::Zero;
		Vector2 originSize = Vector2::Zero;
		Vector2 trimmedSize = Vector2::One;
		Vector2 trimmedOffset = Vector2::Zero;

		float frameDuration = 1.f / 24.f;
		std::wstring searchWstr = WIDE("DieFX/Die0");
		size_t checkStringParseValue = std::wstring::npos;
		{
			CREATE_ANIM(anim, frame, atlasTexSize, frameDuration);
			while (parseResult)
			{
				parseResult = parser->FindElem(WIDE("sprite"));
				if (!parseResult)
				{
					parseResult = true;
					break;
				}

				targetSpriteNameWstr = searchWstr + std::to_wstring(count);

				checkStringParseValue = parser->CheckAttributeIncludeWstr(WIDE("n"), targetSpriteNameWstr);
				if (std::wstring::npos != checkStringParseValue)
				{
					leftTop.x = static_cast<float>(parser->GetIntAttribute(WIDE("x")));
					leftTop.y = static_cast<float>(parser->GetIntAttribute(WIDE("y")));
					size.x = static_cast<float>(parser->GetIntAttribute(WIDE("w")));
					size.y = static_cast<float>(parser->GetIntAttribute(WIDE("h")));
					offset.x = static_cast<float>(parser->GetFloatAttribute(WIDE("pX")));
					offset.y = static_cast<float>(parser->GetFloatAttribute(WIDE("pY")));
					trimmedSize.x = static_cast<float>(parser->GetFloatAttribute(WIDE("tW")));
					trimmedSize.y = static_cast<float>(parser->GetFloatAttribute(WIDE("tH")));
					trimmedOffset.x = static_cast<float>(parser->GetFloatAttribute(WIDE("tOX")));
					trimmedOffset.y = static_cast<float>(parser->GetFloatAttribute(WIDE("tOY")));

					FRAME_ADD_OFFSET(frame, leftTop.x, leftTop.y, size.x, size.y, offset.x, offset.y, anim);
					++count;
				}
			}
			mStartSize.x = canvasSize.x;
			mStartSize.y = canvasSize.y;
			assert(anim.size() > 0);

			std::vector<ParticleFrame> frames;
			ParticleFrame frameInfo = {};

			size_t Size = anim.size();
			for (size_t iter = 0; iter < Size; ++iter)
			{
				frameInfo.animLeftTop = anim[iter].leftTop;
				frameInfo.animSpriteSize = anim[iter].size;
				frameInfo.animOffset = anim[iter].offset;
				frameInfo.animAtlasSize = atlasTexSize;

				frameInfo.animCanvasSize = canvasSize;
				frameInfo.animUsed = 1;
				frameInfo.animCanvasUsed = 1;

				frameInfo.animInverse = 0;
				frameInfo.animType = 1;
				frameInfo.animpadding = Vector2::Zero;

				frames.push_back(frameInfo);
			}

			mAnimSpriteInfo = new AnimationParticleShared[Size];
			memcpy(mAnimSpriteInfo, frames.data(), sizeof(ParticleFrame) * Size);

			int a = 0;

			mAnimFrameDataBuffer = new StructuredBuffer();
			mAnimFrameDataBuffer->Create(sizeof(AnimationParticleShared), static_cast<UINT>(Size), eSRVType::UAV, mAnimSpriteInfo, true);
		}
		delete parser;
	}

	void BossDeathParticleSystem::ChangeState(BossDeathParticleState nextState)
	{
		if (mParticleState == nextState)
			return;

		mParticleState = nextState;

		switch (nextState)
		{
		case hj::BossDeathParticleState::FirstSingleExplosion:
			mStatePlayTimer = 0.f;
			mStatePlayTime = 1.f;
			mMaxParticles = 1;
			SetParticleActiveNum(1);
			dynamic_cast<Stage1Boss*>(GetOwner())->ChangeBoss1State(Boss1State::Dead);
			break;
		case hj::BossDeathParticleState::SecondMultipleExplosion:
			mStatePlayTimer = 0.f;
			mStatePlayTime = 3.f;
			mMaxParticles = 40;
			SetParticleActiveNum(mMaxParticles, true);
			break;
		case hj::BossDeathParticleState::FinalConcentricCircle:
			mStatePlayTimer = 0.f;
			mStatePlayTime = 2.f;
			mMaxParticles = 96;
			SetParticleActiveNum(mMaxParticles, false);
			break;
		case hj::BossDeathParticleState::End:
			mCameraScript->FollowPlayer(true);
			dynamic_cast<Stage1Boss*>(GetOwner())->EndDead();
			DeActivate();
		default:
			break;
		}
	}

	void BossDeathParticleSystem::SetParticleActiveNum(UINT activeNum, bool loop)
	{
		Vector3 pos = dynamic_cast<Stage1Boss*>(GetOwner())->GetBodyPos();
		Vector4 worldPos = Vector4(pos.x, pos.y, pos.z - 10.f, 1.0f);

		ParticleShared shared = {};
		if (1 == activeNum)
		{
			AnimationParticle particle = {};
			Vector4 startPos = Vector4(0.f, 0.0f, 0.0f, 0.0f);
			
			mParticleBuffer->GetData(&particle, sizeof(AnimationParticle));

			particle.particleDefaultInfo.position = startPos + worldPos;

			float frameDuration = 1.f / 24.f;
			particle.duration = frameDuration;

			mParticleBuffer->SetData(&particle, 1);
		}
		else if(loop)
		{
			AnimationParticle particles[NUM_DEATHPARTICLE] = {};

			Vector4 startPos = Vector4(0.f, 0.0f, -10.0f, 0.0f);
			float frameDuration = 1.f / 24.f;

			std::random_device rd;
			std::mt19937 randomSeed(rd());

			std::uniform_real_distribution<float> patternRange(-mRadius, mRadius);
			std::uniform_real_distribution<float> thresholdRange(1.f, 3.f);

			for (size_t i = 0; i < NUM_DEATHPARTICLE; i++)
			{
				if (1 != particles[i].particleDefaultInfo.active)
				{
					particles[i].particleDefaultInfo.position = Vector4(patternRange(randomSeed), patternRange(randomSeed), 0.f, 0.f) + worldPos;
					particles[i].animInfo.animUsed = 1;
					particles[i].duration = frameDuration;
					particles[i].particleDefaultInfo.delayThres = thresholdRange(randomSeed);
					particles[i].loop = loop ? 1 : 0;
				}
			}

			mParticleBuffer->SetData(&particles, mMaxParticles);
		}
		else
		{
			AnimationParticle particles[NUM_DEATHPARTICLE] = {};

			Vector4 startPos = Vector4(0.f, 0.0f, -10.0f, 0.0f);
			float frameDuration = 1.f / 24.f;

			std::random_device rd;
			std::mt19937 randomSeed(rd());


			//std::uniform_real_distribution<float> patternRange(-mRadius, mRadius);
			//std::uniform_real_distribution<float> thresholdRange(1.f, 2.f);
			float delayThres = 0.3f;
			float radius = 0.f;

			constexpr float initialDegree_30 = 30.0f * (XM_PI / 180.0f);

			for (size_t i = 0; i < 8; i++)
			{
				Vector2 initialDirection = Vector2::Down * (radius);

				for (size_t j = 0; j < 12; ++j)
				{
					float finalAxis = i * initialDegree_30 + j * (2 * XM_PI / 12);

					Vector2 rotatedDirection = initialDirection.Rotate(RadianToDegree(finalAxis));
					particles[i * 12 + j].particleDefaultInfo.position = Vector4(rotatedDirection.x, rotatedDirection.y, 0.f, 0.f) + worldPos;;
					particles[i * 12 + j].animInfo.animUsed = 1;
					particles[i * 12 + j].duration = frameDuration;
					particles[i * 12 + j].particleDefaultInfo.delayThres = delayThres;
				}
				delayThres += 0.2f;
				radius += 80.f;
			}

			mParticleBuffer->SetData(&particles, mMaxParticles);
		}

		shared.activeCount = activeNum;
		mParticleSharedBuffer->SetData(&shared, 1);
	}
}