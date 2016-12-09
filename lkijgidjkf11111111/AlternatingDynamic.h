#pragma once
#include "ITimedDynamic.h"

template<typename T>
class AlternatingDynamic
	: public ITimedDynamic<T>
{
private:
	T value1, value2;
	TimeDiff duration;
public:
	AlternatingDynamic(T v1, T v2, TimeDiff dur)
		: value1(v1), value2(v2), duration(dur)
	{

	}

	virtual T Get() override
	{
		auto time_diff = this->GetTimeDiff();
		T val = ((time_diff / duration) % 2) == 0 ? value1 : value2;

		return val;
	}

};