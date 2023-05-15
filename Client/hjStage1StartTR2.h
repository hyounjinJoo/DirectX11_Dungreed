#pragma once
#include "hjRoomBase.h"

namespace hj
{
	class Stage1StartTR2 :
		public RoomBase
	{
	public:
		Stage1StartTR2();
		virtual ~Stage1StartTR2();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
	};
}
