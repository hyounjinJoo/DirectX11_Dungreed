#pragma once
#include "hjRoomBase.h"

namespace hj
{
	class Stage1StartLTRB :
		public RoomBase
	{
	public:
		Stage1StartLTRB();
		virtual ~Stage1StartLTRB();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
	};
}
