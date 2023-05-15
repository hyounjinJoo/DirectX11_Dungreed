#pragma once
#include "hjRoomBase.h"

namespace hj
{
	class Stage1StartLT :
		public RoomBase
	{
	public:
		Stage1StartLT();
		virtual ~Stage1StartLT();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
	};
}
