#pragma once
#include "IDynamic.h"
#include "Util.h"

// Linear dynamic, expressed in initial value and altitude per second
template<typename T>
class LinearDynamic :
	public IDynamic<T>
{
private:
	T mInitValue;
	T mAltitudePerSec;
	TimeStamp mStart;
public:
	LinearDynamic(T initValue, T altitudePerSec) : mInitValue(initValue), mAltitudePerSec(altitudePerSec) {
		this->Reset();
	}

	virtual ~LinearDynamic() {

	}

	void Reset() {
		mStart = Util::GetTime();
	}

	T GetTimeDelta() {
		return (T)(Util::GetTime() - mStart) / (T)1000;
	}

	virtual T Get() override
	{
		return mInitValue + this->GetTimeDelta() * mAltitudePerSec;
	}
};

typedef LinearDynamic<float> LinearDynamicF;