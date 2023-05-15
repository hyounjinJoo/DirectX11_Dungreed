#pragma once
#include "hjRoomBase.h"

namespace hj
{
	class Stage1StartTB :
		public RoomBase
	{
	public:
		Stage1StartTB();
		virtual ~Stage1StartTB();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
	};
}
