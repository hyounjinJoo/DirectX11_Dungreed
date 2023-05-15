#pragma once
#include "hjRoomBase.h"

namespace hj
{
	class Stage1StartL :
		public RoomBase
	{
	public:
		Stage1StartL();
		virtual ~Stage1StartL();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
	};
}
