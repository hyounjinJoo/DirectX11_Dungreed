#pragma once
#include "hjRoomBase.h"

namespace hj
{
	class Stage1StartLRB :
		public RoomBase
	{
	public:
		Stage1StartLRB();
		virtual ~Stage1StartLRB();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
	};
}
