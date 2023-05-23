#pragma once
#include <hjGameObject.h>

namespace hj
{
    enum class Boss1LaserType
    {
        Body,
        SecondBody,
        Head,
        End,
    };

    enum class Boss1LaserDir
    {
        Left,
        Right,
        End,
    };
    
    class Boss1Laser :
        public GameObject
    {
    public:
        Boss1Laser();
        virtual ~Boss1Laser();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

        void SetLaserDir(Boss1LaserDir dir);
        void ShotLaser(Boss1LaserDir dir);

    private:
        Boss1LaserDir mLaserDir;
        std::vector<class Boss1LaserPart*> mLaserParts;
    };

    class Boss1LaserPart :
		public GameObject
	{
		public:
            Boss1LaserPart();
			virtual ~Boss1LaserPart();

			virtual void Initialize() override;
			virtual void Update() override;
			virtual void FixedUpdate() override;
			virtual void Render() override;

			void CreateAnimation();

			void SetLaserType(Boss1LaserType type);
            void Play(bool play) { mbNeedPlay = play; }
            void PlayEnd() { mbPlaying = false; };

		private:
            void PlayAnimation();

			Boss1LaserType mLaserType;
            bool mbNeedPlay;
            bool mbPlaying;
	};
}