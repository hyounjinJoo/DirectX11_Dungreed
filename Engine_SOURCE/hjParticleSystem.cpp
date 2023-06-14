#include "pch.h"
#include "hjParticleSystem.h"
#include "hjMesh.h"
#include "hjResources.h"
#include "hjMaterial.h"
#include "hjStructuredBuffer.h"
#include "hjTransform.h"
#include "hjGameObject.h"
#include "hjTexture.h"
#include "hjTime.h"

namespace hj
{
	ParticleSystem::ParticleSystem()
		: BaseRenderer(eComponentType::ParticleSystem)
		, mMaxParticles(1)
		, mStartSize(Vector4(50.0f, 50.0f, 1.0f, 1.0f))
		, mStartColor(Vector4(1.0f, 0.2f, 0.2f, 1.0f))
		, mStartLifeTime(3.0f)
		, mFrequency(1.0f)
		, mTime(0.0f)
		, mCBData{}
		, mSimulationSpace(eSimulationSpace::World)
		, mRadius(500.0f)
		, mStartSpeed(200.0f)
		, mElapsedTime(0.0f)
	{
	}

	ParticleSystem::ParticleSystem(const ParticleSystem& comp)
		: BaseRenderer(comp)
		, mMaxParticles(1)
		, mStartSize(Vector4(50.0f, 50.0f, 1.0f, 1.0f))
		, mStartColor(Vector4(1.0f, 0.2f, 0.2f, 1.0f))
		, mStartLifeTime(3.0f)
		, mFrequency(1.0f)
		, mTime(0.0f)
		, mCBData{}
		, mSimulationSpace(eSimulationSpace::World)
		, mRadius(500.0f)
		, mStartSpeed(200.0f)
		, mElapsedTime(0.0f)
	{

	}

	ParticleSystem::~ParticleSystem()
	{
		delete mParticleBuffer;
		mParticleBuffer = nullptr;

		delete mParticleSharedBuffer;
		mParticleSharedBuffer = nullptr;
	}

	void ParticleSystem::Initialize()
	{
		mCS = Resources::Find<ParticleShader>(L"Shader_ParticleCS");

		std::shared_ptr<Mesh> point = Resources::Find<Mesh>(L"Mesh_Point");
		SetMesh(point);

		// Material ����
		std::shared_ptr<Material> material = Resources::Find<Material>(L"MTRL_Particle");
		SetMaterial(material);

		std::shared_ptr<Texture> tex = Resources::Find<Texture>(L"CartoonSmoke");
		material->SetTexture(eTextureSlot::T0, tex);

		Particle particles[1] = {};
		Vector4 startPos = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
		for (size_t i = 0; i < mMaxParticles; i++)
		{
			particles[i].position = Vector4(0.0f, 0.0f, 0.0f, 0.0f);
			particles[i].active = 0;
			particles[i].direction =
				Vector4(cosf((float)i * (XM_2PI / (float)mMaxParticles))
					, sin((float)i * (XM_2PI / (float)mMaxParticles)), 0.0f, 1.0f);

			particles[i].speed = 100.0f;
		}

		mParticleBuffer = new StructuredBuffer();
		mParticleBuffer->Create(sizeof(Particle), mMaxParticles, eSRVType::UAV, particles, true);

		mParticleSharedBuffer = new StructuredBuffer();
		mParticleSharedBuffer->Create(sizeof(ParticleShared), 1, eSRVType::UAV, nullptr, true);
	}

	void ParticleSystem::Update()
	{
	}

	void ParticleSystem::FixedUpdate()
	{
		//��ƼŬ ���� �ð�
		float aliveTime = 1.0f / mFrequency;
		//�����ð�
		mTime += Time::ActualDeltaTime();
		if (aliveTime < mTime)
		{
			float f = (mTime / aliveTime);
			UINT iAliveCount = (UINT)f;
			mTime = f - std::floor(f);

			ParticleShared shared = { 1, };
			mParticleSharedBuffer->SetData(&shared, 1);
		}
		else
		{
			if (mTime > 0.5f)
				int a = 0;
			Particle particles[100] = {};

			mParticleBuffer->GetData(particles, 0);
			ParticleShared shared = {  };
			mParticleSharedBuffer->SetData(&shared, 1);
		}

		mMaxParticles = mParticleBuffer->GetStride();
		Vector3 pos = GetOwner()->GetComponent<Transform>()->GetPosition();
		mCBData.worldPosition = Vector4(pos.x, pos.y, pos.z, 1.0f);
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

		mCS->SetSharedStructuredBuffer(mParticleSharedBuffer);
		mCS->SetStrcutedBuffer(mParticleBuffer);
		mCS->OnExecute();
	}

	void ParticleSystem::Render()
	{
		GetOwner()->GetComponent<Transform>()->SetConstantBuffer();
		mParticleBuffer->BindSRV(eShaderStage::GS, 15);

		GetMaterial()->Bind();
		GetMesh()->RenderInstanced(mMaxParticles);

		mParticleBuffer->Clear();
	}

	hj::Component* ParticleSystem::Clone() const
	{
		return new ParticleSystem(*this);
	}

}
