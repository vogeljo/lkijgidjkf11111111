#pragma once
#include "ITimedDynamic.h"

template<typename T>
class TimedSineDynamic : ITimedDynamic<T> {
private:
	T mStartRad;
	T mScale;
public:
	TimedSineDynamic()
		: TimedSineDynamic(0.0f, 1.0f)
	{

	}

	TimedSineDynamic(T start_rad, T rad_per_sec)
		: mStartRad(start_rad), mScale(rad_per_sec)
	{
		
	}

	virtual ~TimedSineDynamic() {

	}

	virtual T Get() override {
		return std::sinf(mStartRad + GetTimeDelta() * mScale * 3.14f);
	}
};