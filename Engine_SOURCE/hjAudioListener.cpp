#include "pch.h"
#include "hjAudioListener.h"
#include "hjTransform.h"
#include "hjGameObject.h"
#include "hjFmod.h"

namespace hj
{
	AudioListener::AudioListener()
		: Component(eComponentType::AudioListener)
	{

	}

	AudioListener::~AudioListener()
	{

	}

	void AudioListener::Initialize()
	{

	}

	void AudioListener::Update()
	{

	}

	void AudioListener::FixedUpdate()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();
		Vector3 forward = tr->Forward();
		Vector3 up = tr->Up();

		Vector3 vel = { 0.0f, 0.0f, 0.0f };
		Fmod::Set3DListenerAttributes(&pos, &vel, &forward, &up);
	}

	void AudioListener::Render()
	{

	}

	hj::Component* AudioListener::Clone() const
	{
		return new AudioListener();
	}

}