#pragma once
#include "hjRoomBase.h"

namespace hj
{
	class Stage1StartLB :
		public RoomBase
	{
	public:
		Stage1StartLB();
		virtual ~Stage1StartLB();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
	};
}
