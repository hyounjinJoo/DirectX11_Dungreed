#pragma once
#include "hjActor.h"

namespace hj
{
	enum class bodyType
	{
		HEAD,
		LEFT_UPPER_ZAYGOMA_BONE,
		LEFT_LOWER_ZAYGOMA_BONE,
		RIGHT_UPPER_ZAYGOMA_BONE,
		RIGHT_LOWER_ZAYGOMA_BONE,
		JAW,
		End,
	};

	class Boss1Parts :
		public Actor
	{
	public:
		Boss1Parts();
		virtual ~Boss1Parts();

	public:
		virtual void Update() override;

		void SetBodyType(bodyType type);

	private:
		void CreateAnimation();

	private:
		float mMoveStartTimer;
		float mMoveStartThreshold;

		bodyType mBodyType;
	};
}