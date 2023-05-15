#pragma once
#include "hjRoomBase.h"

namespace hj
{
	class Stage1StartLR :
		public RoomBase
	{
	public:
		Stage1StartLR();
		virtual ~Stage1StartLR();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
	};
}
