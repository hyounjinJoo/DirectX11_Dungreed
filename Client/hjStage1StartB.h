#pragma once
#include "hjRoomBase.h"

namespace hj
{
	class Stage1StartB :
		public RoomBase
	{
	public:
		Stage1StartB();
		virtual ~Stage1StartB();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
	};
}
