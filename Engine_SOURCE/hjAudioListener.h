#pragma once
#include "hjComponent.h"


namespace hj
{
	class AudioListener : public Component
	{
	public:
		AudioListener();
		~AudioListener();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		virtual Component* Clone() const;

	private:
	};
}