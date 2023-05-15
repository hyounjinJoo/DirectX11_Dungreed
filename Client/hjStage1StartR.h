#pragma once
#include "hjRoomBase.h"

namespace hj
{
	class Stage1StartR :
		public RoomBase
	{
	public:
		Stage1StartR();
		virtual ~Stage1StartR();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
	};
}
