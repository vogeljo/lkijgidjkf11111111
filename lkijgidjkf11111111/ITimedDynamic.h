#pragma once
#include "IDynamic.h"

template<typename T>
class ITimedDynamic : IDynamic<T> {
protected:
	TimeStamp mStart;

public:
	ITimedDynamic() {
		this->Reset();
	}

	virtual ~ITimedDynamic() {

	}

	void Reset() {
		mStart = Util::GetTime();
	}

	T GetTimeDelta() {
		return (T)this->GetTimeDiff() / (T)1000;
	}

	TimeDiff GetTimeDiff() {
		return Util::GetTime() - mStart;
	}
};