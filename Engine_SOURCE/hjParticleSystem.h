#pragma once
#include "hjBaseRenderer.h"
#include "hjParticleShader.h"

namespace hj
{
	enum class eSimulationSpace
	{
		Local,
		World,
	};

	class ParticleSystem : public BaseRenderer
	{
	public:
		ParticleSystem();
		ParticleSystem(const ParticleSystem& comp);
		virtual ~ParticleSystem();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;
		virtual Component* Clone() const override;

	protected:
		class StructuredBuffer* mParticleBuffer;
		class StructuredBuffer* mParticleSharedBuffer;

		std::shared_ptr<graphics::ParticleShader> mCS;
		renderer::ParticleSystemCB mCBData;

		Vector4 mStartSize;
		Vector4 mStartColor;
		eSimulationSpace mSimulationSpace;
		UINT mMaxParticles;
		float mStartLifeTime;
		float mFrequency;
		float mRadius;

		float mStartSpeed;
		float mTime;
		float mElapsedTime; //누적시간
	};
}
