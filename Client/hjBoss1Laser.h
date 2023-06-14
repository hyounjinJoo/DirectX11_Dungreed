#pragma once
#include "hjActor.h"

namespace hj
{
    enum class Boss1LaserType
    {
        Body,
        SecondBody,
        Head,
        End,
    };

    class Boss1LaserCollider;
    class Boss1LaserPart;

    class Boss1Laser :
        public Actor
    {
    public:
        Boss1Laser();
        virtual ~Boss1Laser();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;
		void ShotLaser();
        void ResetLaser();
        bool IsLaserEnd();

        Boss1LaserCollider* GetLaserCollider() const { return mLaserCollider; }
        void SetLaserColliderPosZ(float posZ);

        void PauseLaserAnimation();
        void DeactivateLaser();

    private:
        Boss1LaserCollider* mLaserCollider;
        std::vector<Boss1LaserPart*> mLaserParts;
    };

    class Boss1LaserPart :
		public Actor
	{
		public:
            Boss1LaserPart();
			virtual ~Boss1LaserPart();

			virtual void Initialize() override;
			virtual void Update() override;
			virtual void FixedUpdate() override;
			virtual void Render() override;

			void CreateAnimation();
            Vector2 GetLaserRealSize();

			void SetLaserType(Boss1LaserType type);
			void Play(bool play);
			void OnDamageEnter();
			void OnDamageExit();
            void ResetLaser() { mbIsEnd = false; }
            void EndLaser() { mbIsEnd = true; }
            bool IsEndLaser() { return mbIsEnd; }

            void SetOwnerLaser(Boss1Laser* laser);

		private:
            void PlayAnimation();

            Boss1Laser* mOwnerLaser;
			Boss1LaserType mLaserType;
            bool mbPlaying;
            bool mbIsEnd;
	};
}