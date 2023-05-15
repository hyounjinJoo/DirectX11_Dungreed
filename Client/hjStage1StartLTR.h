#pragma once
#include "hjRoomBase.h"

namespace hj
{
	class Stage1StartLTR :
		public RoomBase
	{
	public:
		Stage1StartLTR();
		virtual ~Stage1StartLTR();

		virtual void Initialize() override;
		virtual void Update() override;
		virtual void FixedUpdate() override;
		virtual void Render() override;

	private:
	};
}
