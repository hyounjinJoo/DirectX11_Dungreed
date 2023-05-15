#pragma once
#include "hjRoomBase.h"

namespace hj
{
	class Stage1StartRB :
		public RoomBase
	{
	public:
		Stage1StartRB();
		virtual ~Stage1StartRB();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
	};
}
