#pragma once
#include "hjRoomBase.h"

namespace hj
{
	class Stage1StartT :
		public RoomBase
	{
	public:
		Stage1StartT();
		virtual ~Stage1StartT();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
	};
}
