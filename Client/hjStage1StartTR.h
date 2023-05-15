#pragma once
#include "hjRoomBase.h"

namespace hj
{
	class Stage1StartTR :
		public RoomBase
	{
	public:
		Stage1StartTR();
		virtual ~Stage1StartTR();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
	};
}
