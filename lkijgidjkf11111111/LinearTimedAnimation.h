#pragma once
#include "ITimedAnimation.h"

class LinearTimedAnimation :
	public ITimedAnimation
{
public:
	using ITimedAnimation::ITimedAnimation;
	virtual ~LinearTimedAnimation();

	virtual float CalculateValue(float progress) override;
};

