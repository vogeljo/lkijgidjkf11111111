#include "LinearTimedAnimation.h"

LinearTimedAnimation::~LinearTimedAnimation()
{
}

float LinearTimedAnimation::CalculateValue(float progress)
{
	return mStartValue + progress * (mEndValue - mStartValue);
}
