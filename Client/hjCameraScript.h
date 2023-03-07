#pragma once
#include "hjScript.h"

namespace hj
{
	class CameraScript : public Script
	{
	public:
		CameraScript();
		virtual ~CameraScript();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void Render() override;

	private:
	};
}