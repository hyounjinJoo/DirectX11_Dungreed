#pragma once
#include "hjComponent.h"
#include "hjAudioClip.h"

namespace hj
{
	class AudioSource : public Component
	{
	public:
		AudioSource();
		~AudioSource();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;
		virtual Component* Clone() const;
		void Play();
		void Stop();
		void SetLoop(bool loop);

		void SetClip(std::shared_ptr<AudioClip> clip) { mAudioClip = clip; }
		std::shared_ptr<AudioClip> GetClip() { return mAudioClip; }

	private:
		std::shared_ptr<AudioClip> mAudioClip;
	};
}
