#include "pch.h"
#include "hjAudioSource.h"
#include "hjAudioClip.h"
#include "hjTransform.h"
#include "hjGameObject.h"

namespace hj
{
	AudioSource::AudioSource()
		: Component(eComponentType::AudioSource)
		, mAudioClip(nullptr)
	{

	}

	AudioSource::~AudioSource()
	{
	}

	void AudioSource::Initialize()
	{
	}

	void AudioSource::Update()
	{
	}

	void AudioSource::FixedUpdate()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();
		Vector3 forward = tr->Forward();

		mAudioClip->Set3DAttributes(pos, forward);
	}

	void AudioSource::Render()
	{
	}

	hj::Component* AudioSource::Clone() const
	{
		return new AudioSource(); 
	}

	void AudioSource::Play()
	{
		mAudioClip->Play();
	}
	void AudioSource::Stop()
	{
		mAudioClip->Stop();
	}
	void AudioSource::SetLoop(bool loop)
	{
		mAudioClip->SetLoop(loop);
	}
}