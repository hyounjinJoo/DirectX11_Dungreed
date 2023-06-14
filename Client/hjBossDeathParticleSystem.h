#pragma once
#include <hjParticleSystem.h>
#include <hjParticleAnimShader.h>

namespace hj
{
	enum class BossDeathParticleState
	{
		Start,
		FirstSingleExplosion,
		SecondMultipleExplosion,
		FinalConcentricCircle,
		End,
	};


	class BossDeathParticleSystem :
		public ParticleSystem
	{
	public:
		BossDeathParticleSystem();
		BossDeathParticleSystem(const BossDeathParticleSystem& comp);
		virtual ~BossDeathParticleSystem();

		virtual void Initialize() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		virtual Component* Clone() const override;
	
		void ChangeState(BossDeathParticleState state);
	private:
		void LoadAnimationData();

	private:
		class StructuredBuffer* mAnimFrameDataBuffer;
		std::shared_ptr<graphics::ParticleAnimShader> mAnimCS;
		AnimationParticleShared* mAnimSpriteInfo;

	private:
		BossDeathParticleState mParticleState;
		float mStatePlayTimer = 0.f;
		float mStatePlayTime = 3.f;
		void SetParticleActiveNum(UINT activeNum, bool loop = false);

		class CameraScript* mCameraScript;
	};
}