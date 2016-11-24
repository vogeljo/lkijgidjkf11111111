#pragma once
#include "ITimedDynamic.h"
#include "Util.h"

// Linear dynamic, expressed in initial value and altitude per second
template<typename T>
class LinearDynamic :
	public ITimedDynamic<T>
{
private:
	T mInitValue;
	T mAltitudePerSec;
public:
	LinearDynamic(T initValue, T altitudePerSec) : mInitValue(initValue), mAltitudePerSec(altitudePerSec) {

	}

	virtual ~LinearDynamic() {

	}

	virtual T Get() override
	{
		return mInitValue + this->GetTimeDelta() * mAltitudePerSec;
	}
};

typedef LinearDynamic<float> LinearDynamicF;