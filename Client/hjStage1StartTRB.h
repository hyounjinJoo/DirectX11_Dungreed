#pragma once
#include "hjRoomBase.h"

namespace hj
{
	class Stage1StartTRB :
		public RoomBase
	{
	public:
		Stage1StartTRB();
		virtual ~Stage1StartTRB();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
	};
}
