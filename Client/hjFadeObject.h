#pragma once
#include <hjGameObject.h>

namespace hj
{
	class FadeObject :
		public GameObject
	{
	public:
		FadeObject();
		virtual ~FadeObject();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

		class FadeScript* GetFadeScript();

	private:
		FadeScript* mFadeScript;
	};
}