#pragma once
#include "hjRoomBase.h"

namespace hj
{
	class Stage1StartLTB :
		public RoomBase
	{
	public:
		Stage1StartLTB();
		virtual ~Stage1StartLTB();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
	};
}